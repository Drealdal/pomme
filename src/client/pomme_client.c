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
static int get_ds(pomme_client_t *client, 
        u_int32 msid,	
	u_int32 id,
       	u_int32 *ip, u_int16 *port);

static int get_ms(pomme_client_t *client, u_int32 id,
       	u_int32 *ip, u_int16 *port);

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


int pomme_client_init(pomme_client_t *client, 
	u_int32 mip, 
	u_int16 mport)
{
    int ret = 0;
    assert( NULL != client );
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

    return ret;
i_err:
    pomme_hash_distroy(&client->imap);
m_err:
    pomme_hash_distroy(&client->ds_nodes);
err:
    return ret;
}

PFILE *pomme_open(const char *spath, const char *mode)
{
    int ret = 0;
    char *path = NULL;
    PFILE *file = NULL;

    assert( spath != NULL );
    file = malloc(sizeof(PFILE));

    if( file == NULL )
    {
	debug("malloc error");
	return file;
    }

    path = make_path((char *)spath);

    return ret;
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
	char *path, u_int64 *inode)
{
    int ret = 0;

    path = make_path(path); 
    if( strcmp(path,"") == 0 )
    {
	*inode = 0;
	return RET_SUCCESS;
    }

    pomme_data_t key, val;
    key.size = pomme_strlen(path);
    key.data = path;

    val.size = sizeof(u_int64);
    val.data = inode;

    if(( ret = pomme_hash_get(client->imap,
		    &key, &val)) < 0 )
    {
	char *fpath = get_parrent(path);  
	u_int64 pinode = -1;

	if(( ret = get_inode(client, fpath,
		       	&pinode)) < 0 )
	{
	    debug("Get inode for %s failure",fpath);
	    return RET_FAILURE;
	}

    }

    return ret; 
}
