/*
 * =====================================================================================
 *
 *       Filename:  pomme_cimp.c
 *
 *    Description:  the implement of client
 *
 *        Version:  1.0
 *        Created:  06/01/2012 07:57:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcc.h"
#include "pomme_type.h"
#include "pomme_client.h"
#include "pomme_utils.h"
#include "pomme_path.h"
#include "pomme_client_meta.h"
#include "pomme_mapping.h"

pomme_client_t GLOBAL_CLIENT;

/**
 * @brief send_data runtine for data send thread
 *
 * @param data
 *
 * @return 
 */
void * send_data(void *data);

static int get_ds(pomme_client_t *client, 
        u_int32 msid,	
	u_int32 id,
       	u_int32 *ip, u_int16 *port);

static int get_ms(pomme_client_t *client, u_int32 id,
       	u_int32 *ip, u_int16 *port);

static int get_inode(pomme_client_t *client,
	char *path, u_int64 *inode,
	int create);
static PFILE * get_pfile(pomme_client_t *client);

static rpcc_t * inode2rpcc(pomme_client_t *client,
	u_int64 inode);

static rpcc_t * inode2rpcc(
	pomme_client_t *client,
	u_int64 inode);

static int get_dsgroup(pomme_client_t *client,
       	u_int32 gid,
	int *dsnum,
       	u_int32 **dsids);

int cmp_dsnode(void *node1, void *node2)
{
    ds_node *n1 = node1;
    ds_node *n2 = node2;
    if(n1->ip == n2->ip && n1->port == n2->port)
    {
	return 0;
    }
    if( n1->ip < n2->ip ) return -1;

    return 1;
}

int cmp_msnode(void *node1, void *node2)
{
    ms_node *n1 = node1;
    ms_node *n2 = node2;
    if(n1->ip == n2->ip && n1->port == n2->port)
    {
	return 0;
    }
    if( n1->ip < n2->ip ) return -1;

    return 1;
}
int pomme_init()
{
    if( GLOBAL_CLIENT.inited != 1 )
    {
	return pomme_client_init(&GLOBAL_CLIENT,0,0);
    }
    return RET_SUCCESS;
}


int pomme_client_init(pomme_client_t *client, 
	u_int32 mip, 
	u_int16 mport)
{
    int ret = 0;
    assert( NULL != client );
    debug("Init client lib");
    client->mip = mip;
    client->mport = mport;

    if ( (ret = pomme_hash_init(POMME_CLIENT_BUFFER_DS,
	    &hash_int, &cmp_dsnode,&client->ds_nodes)) != 0)
    {
	debug("Hash init failure");
	goto err;
    }

    if ( (ret = pomme_hash_init(POMME_CLIENT_BUFFER_MS,
	    &hash_int, &cmp_msnode,&client->ms_nodes)) != 0)
    {
	debug("Hash init failure");
	goto m_err;
    }	    
    if( ( ret = pomme_hash_str_uint64(POMME_CLIENT_BUFFER_IMAP,
		    &client->imap)) != 0 )
    {
	debug("Hash init failure");
	goto i_err;
    }

    client->count = 0;
    client->nextfd = 0;

    if(( ret = pthread_mutex_init(&client->mutex,
		    NULL)) != 0 )
    {
	err_exit("pthread_mutex_init failure");
    }
    client->max_count = POMME_MAX_OPEN_FILE;

    init_queue(&client->open_files,"Opened files",client->max_count);
    init_queue(&client->closed_files, "Clised files",client->max_count);

    pomme_mapping_init(&client->msmap);

    client->inited = 1;
    debug("Here");

    client->get_ds_info = get_ds;
    client->get_ms_info = get_ms;
    client->get_pfile = get_pfile;
    client->get_dsgroup = get_dsgroup;

    return ret;
i_err:
    pomme_hash_distroy(&client->imap);
m_err:
    pomme_hash_distroy(&client->ds_nodes);
err:
    return ret;
}

PFILE *pomme_open(const char *spath, int mode)
{
    int ret = 0;
    char *path = NULL;
    PFILE *file = NULL;

    int create = 0;
    assert( spath != NULL );
    if( (ret = pomme_init()) < 0 )
    {
	debug("Client init error");
	return NULL;
    }
    pomme_client_t *client = &GLOBAL_CLIENT;
    file = malloc(sizeof(PFILE));
    u_int64 inode;

    if( file == NULL )
    {
	debug("malloc error");
	return file;
    }

    path = make_path((char *)spath);
    

    if( ( mode & POMME_CREATE ) != 0 )
    {
	debug("Create File");
	create = 1;
    }	
    u_int64 pinode;
    rpcc_t *rct = NULL;
    char *fpath = get_parrent(path);
    if( ( ret = get_inode(client,fpath,&pinode,0 )) < 0 )
    {
	debug("Get Parrent inode failure");
	goto clear;
    }

    if( (ret = get_inode(client,path,&inode,
	create) ) < 0 )
    {
	debug("Get inode info failure");
	if( ret == POMME_META_FILE_NOT_FOUND )
	{
	    debug("Parrent dir not exist");
	}
	return NULL;
    }
    file = client->get_pfile(client);
    file->inode = inode;
    file->pinode = pinode;

    if( ( rct = inode2rpcc(client
		    ,pinode)) == NULL )
    {
	debug("Get rpc handle error");
	goto clear;
    }
    file->rct = rct;
    
    debug("the inode is:%llu pinode is:%llu",file->inode,file->pinode);

    if( create == 1)
    {
	if( ( ret = pomme_sync_create_file(rct,
			mode,file)) < 0 )
	{
	    debug("Create file failure");
	    goto clear;
	}
    }else{
	if( ( ret = pomme_sync_read_file_meta(rct,
		       file)) < 0 )
	{
	    debug("Open file failure");
	    goto clear;
	}	    

    }
    return file;
clear:
    free(fpath);
    free(file);
    free(rct);
    return NULL;
}
int pomme_write(const void *ptr, 
	size_t size,
       	size_t nmemb,
	PFILE *file)
{
    // TODO need add lock ?
    int ret = 0, i = 0;
    u_int64 len = 0, off = 0;
    u_int32 dsgroup = 0;
    uuid_t id;
    send_arg_t *arg = NULL;

    void *tret = NULL;
    int failure = 0;

    int dsnum = 0;
    u_int32 *dsids = NULL;

    assert( file != NULL );
    assert( ptr != NULL );

    pomme_client_t *client = &GLOBAL_CLIENT;
    len = size * nmemb;
    off = file->off;

    dsgroup = file->meta->dsgroup;

    if( (ret = client->get_dsgroup(client, 
		    dsgroup,&dsnum,&dsids)) < 0 )
    {
	debug("Get data node group infomation error");
	return -1;
    }

    uuid_generate_time(id); 

    pthread_t tids[dsnum];
    memset(tids, 0, sizeof(pthread_t)*dsnum);
    for( i = 0; i < dsnum; i++ )
    {
	 arg = malloc(sizeof(send_arg_t));
	
	assert(arg != NULL);
	memset(arg, 0, sizeof(send_arg_t));

	arg->dsid = dsids[i];
	uuid_copy(arg->objectid, id);

	arg->file = file;
	arg->len = len;
	arg->data = ptr;

	if( (ret = pthread_create(&tids[i],NULL,
			send_data,(void *)arg) ) != 0 )
	{
	    debug("Create send thread failure");
	    goto clear;
	}
    }


    for( i = 0; i < dsnum; i++ )
    {
	if( ( ret  = pthread_join(tids[i],&tret) ) != 0 )
	{
	    debug("Pthread join failure");
	    ret = -1;
	    goto clear;
	} 
	if( (int)tret != 0 )
	{
	    // TODO 
	    debug("Put Data failure");
	    failure = 1;
	}
    }

    if( failure == 1 )
    {
	ret = -1;
	goto clear;
    }

    if(( ret = pomme_client_write_file(file->rct,
		    file->inode,id, off, len )) < 0 )
    {
	debug("Create meta object failure");
	goto clear;
    }
    file->off += len;

   debug("Write success");

   return len; 

clear:
    return ret;
}
int pomme_read(void *ptr,
	size_t size,
	size_t nmemb,
	PFILE *file)
{
    int ret = 0;
    assert( ptr != NULL );
    assert( file != NULL );

    u_int64 off = file->off;
    u_int64 len = size * nmemb;

    return ret;
clear:
    return ret;
}

static int get_dsgroup(pomme_client_t *client,
    u_int32 gid,
    int *dsnum, 
    u_int32 **dsids)
{
    *dsnum = 1;
    *dsids = malloc(*dsnum * sizeof(int));
    *dsids[0] = 0;
    return 0;
}

static int get_ds(pomme_client_t *client, 
        u_int32 msid,	
	u_int32 id,
       	u_int32 *ip, u_int16 *port)
{
    int ret = 0;
    pomme_data_t key,*value;
    memset(&key, 0, sizeof(key));
    key.size = sizeof(u_int32);
    key.data = &id;

    pomme_data_init(&value, sizeof(ds_node));

    if( 0 == pomme_hash_get(client->ds_nodes, &key, value) )
    {
	ds_node *p = value->data;
	*ip = p->ip;
	*port = p->port;
	pomme_data_distroy(&value);
	goto exit;
    }
    u_int32 mip;
    u_int16  mport;
    if ( 0 != ( ret = get_ms(client, msid, &mip, &mport) ))
    {
	goto exit;
    }
    rpcc_t rpcc;

    if ( ( ret = pomme_rpcc_init(&rpcc, mip, mport,0) ) 
	    != 0 )
    {
	debug("init rpc client error");
	goto exit;
    }

    ds_node ds;

    if( ( ret = pomme_client_get_ds(&rpcc,
		    id,&ds) ) != 0 )
    {
	debug("Get Data Node info failure");
	goto exit;
    }
    *ip = ds.ip;
    *port = ds.port;

exit:
    return ret;
}

static int get_ms(pomme_client_t *client, u_int32 id,
       	u_int32 *ip, u_int16 *port)
{
    int ret = 0;
    *ip = inet_addr("127.0.0.1");
    *port = POMME_META_RPC_PORT;
    return ret;
}

static int get_inode(pomme_client_t *client,
	char *path, u_int64 *inode,
	int create)
{
    int ret = 0;

    if( strcmp(path,"/") == 0 )
    {
	*inode = 0;
	return RET_SUCCESS;
    }
    path = make_path(path); 

    pomme_data_t key, val;
    key.size = pomme_strlen(path);
    key.data = path;

    val.size = sizeof(u_int64);
    val.data = inode;

    debug("before hash get");
    if(( ret = pomme_hash_get(client->imap,
		    &key, &val)) < 0 )
    {
	char *fpath = get_parrent(path);  
	char *fname = get_name(path);
	rpcc_t *rct = NULL;
	debug("parrent inode of:%s/%s",fpath,fname);

	u_int64 pinode = -1;

	if(( ret = get_inode(client, fpath,
		       	&pinode,create)) < 0 )
	{
	    debug("Get inode for %s failure",fpath);
	    goto clear;
	}
	if( ( rct = inode2rpcc(client
			,pinode)) == NULL )
	{
	    debug("Get rpc handle error");
	    goto clear;
	}

	
	if( (ret = pomme_sync_get_inode(rct, 
		       pinode, fname,create,inode)) < 0 )
	{
	    debug("Remote call failure");
	    goto clear;
	}	    
clear:
	free(rct);
	free(fpath);
	free(fname);
    }
    debug("Inode Get for %s is:%llu",path,*inode);
    return ret; 
}
static PFILE * get_pfile(pomme_client_t *client)
{
    assert( client != NULL );
    PFILE *file = NULL;
    /*  use stack is better */
    queue_body_t *fbody = queue_get_front(client->closed_files);
    if(fbody == NULL )
    {
	debug("Nothing get");
	file = (PFILE *)malloc(sizeof(PFILE));
	if( file == NULL )
	{
	    debug("Malloc Error");
	    return NULL;
	}
	memset(file,0,sizeof(PFILE));
    }else{
	file = queue_entry(fbody, PFILE, next);
    }
    queue_push_back(client->open_files, &file->next);
    return file;
}


static rpcc_t * inode2rpcc(
	pomme_client_t *client,
	u_int64 inode)
{
	int ret = 0;
	u_int32 ms,msip;
	u_int16 msport;
	rpcc_t *rct = NULL;

	if ( ( ret = client->msmap.inode2ms(inode,&ms)) < 0 )
	{
	    debug("Map inode:%llu error",inode);
	    goto clear;
	}

	if( (ret = client->get_ms_info(client,
		       	ms, &msip, &msport) ) < 0 )
	{
	    debug("get meta server info for %u failure",ms);
	    goto clear;
	}

	rct = malloc(sizeof(rpcc_t));
	if( rct == NULL )
	{
	    return rct;
	}

	if( ( ret = pomme_rpcc_init(rct, msip,msport,0))
		!= 0 )
	{
	    debug("init rpc client error");
	    return NULL;
	}
clear:
	return rct;
}
void * send_data(void *data)
{
    int ret = 0;
    pomme_client_t *client = &GLOBAL_CLIENT;
    send_arg_t *arg = (send_arg_t *)data;
    PFILE *file = arg->file;

    u_int32 ip,ms;
    u_int16 port;
    int handle;

    if ( ( ret = client->msmap.inode2ms(file->inode,&ms)) < 0 )
    {
	debug("Map inode:%llu error",file->inode);
	goto clear;
    }

    if( ( ret = client->get_ds_info(client,
		    ms, arg->dsid,&ip, &port)) < 0 )
    {
	debug("Get infomation for %u failure",arg->dsid);
	goto clear;
    }	

   debug("Data node: %u:%u",ip,port); 
   if( (ret =  create_client(ip,port,
		   &handle) ) < 0 ) 
   {
       // TODO , collect infomation to report failures:
       debug("Connect error");
       goto clear;
   }
   if( ( ret = pomme_client_put_data(arg->objectid,
		   handle, arg->data,
		   arg->len,arg->flags) ) < 0 )
   {
       debug("Put data error");
       goto clear;
   }  
   return (void *) 0 ;
clear:
   close(handle);
   free(data);
   return (void *)ret;

}
