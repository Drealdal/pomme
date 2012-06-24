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
#include "pomme_client.h"
#include "pomme_utils.h"
#include "pomme_path.h"
#include "pomme_client_meta.h"
#include "pomme_mapping.h"

pomme_client_t GLOBAL_CLIENT;

static int get_ds(pomme_client_t *client, 
        u_int32 msid,	
	u_int32 id,
       	u_int32 *ip, u_int16 *port);

static int get_ms(pomme_client_t *client, u_int32 id,
       	u_int32 *ip, u_int16 *port);

static int get_inode(pomme_client_t *client,
	char *path, u_int64 *inode,
	int create);

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

    init_queue(&client->files,"Client files",client->max_count);
    pomme_mapping_init(&client->msmap);

    client->inited = 1;
    debug("Here");

    client->get_ds_info = get_ds;
    client->get_ms_info = get_ms;

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

    debug("Before get inode");
    if( (ret = get_inode(client,path,&inode,
	create) ) < 0 )
    {
	debug("Get inode info failure");
	return NULL;
    }

    return file;
}


static int get_ds(pomme_client_t *client, 
        u_int32 msid,	
	u_int32 id,
       	u_int32 *ip, u_int16 *port)
{
    int ret;
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
	debug("%s:%s",fpath,fname);

	u_int64 pinode = -1;
	u_int32 ms = 0,msip;
	u_int16 msport;

	if(( ret = get_inode(client, fpath,
		       	&pinode,create)) < 0 )
	{
	    debug("Get inode for %s failure",fpath);
	    goto clear;
	}

	if ( ( ret = client->msmap.
		    inode2ms(pinode,&ms)) < 0 )
	{
	    debug("Map inode:%llu error",pinode);
	    goto clear;
	}

	if( (ret = client->get_ms_info(client,
		       	ms, &msip, &msport) ) < 0 )
	{
	    debug("get meta server info for %u failure",ms);
	    goto clear;
	}

	rpcc_t rct;
	if( ( ret = pomme_rpcc_init(&rct, msip,msport,0))
		!= 0 )
	{
	    debug("init rpc client error");
	    goto clear;
	}
	debug("%s:%s:%s",path,fpath,fname);
	
	if( (ret = pomme_sync_get_inode(&rct, 
		       pinode, fname,create,inode)) < 0 )
	{
	    debug("Remote call failure");
	    goto clear;
	}	    
clear:
	free(fpath);
	free(fname);
    }
    return ret; 
}
