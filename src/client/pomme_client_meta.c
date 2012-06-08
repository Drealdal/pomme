/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_meta.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月24日 23时03分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_client_meta.h"
#include "pomme_meta.h"
#include "utils.h"
#include "pomme_utils.h"
#include "pomme_rpcc.h"


int pomme_sync_create_file(rpcc_t *rct, 
	char *path,
	int mode,
	pomme_file_t **file)
{
    int ret = 0;
    assert( rct != NULL );
    assert( path != NULL );

    pomme_data_t *arg = malloc(3*sizeof(pomme_data_t));
    assert( arg != NULL );
    char *name = POMME_META_CREATE_FILE_S;
    arg[0].size = pomme_strlen(name);
    arg[0].data = name;

    arg[1].size = pomme_strlen(path);
    arg[1].data = path;

    arg[2].size = sizeof(int);
    arg[2].data = &mode;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));

    if( (ret = rct->sync_call(rct,3,arg,&res,0) ) < 0) 
    {
	debug("Error call");
	goto err;
    }else{
	debug("created");
    }
    *file = (pomme_file_t *)res.data;
err:
    free(arg);
    return ret;
}
int pomme_sync_read_file_meta(
	rpcc_t *rct,
	char *path,
	pomme_file_t **file,
	int *obj_num,
       	ms_object_t **object)
{
    int ret = 0; 
    assert(rct != NULL);
    assert(path != NULL);

    pomme_data_t *arg = malloc(2*sizeof(pomme_data_t));
    assert( arg != NULL );
    char *name = POMME_META_READ_FILE_S;
    arg[0].size = pomme_strlen(name);
    arg[0].data = name;

    arg[1].size = pomme_strlen(path);
    arg[1].data = path;

    pomme_data_t res;
    memset(&res,0,sizeof(pomme_data_t));

    if( ( ret = rct->sync_call(rct, 2, arg, &res, 0 )) != 0 )
    {
	debug("Call read file failure:%d",ret);
	goto e_exit;
    }else{
	debug("Read file meta ok");
    }
    *file = res.data;
    debug("ret:%d",ret);
    debug("%d",res.size);
    debug("%d",sizeof(pomme_file_t));
    debug("%d",sizeof(ms_object_t));
    *obj_num = (res.size - sizeof(pomme_file_t))/sizeof(ms_object_t);
    if( *object > 0 )
    {
	*object = res.data + sizeof(pomme_file_t);
    }else{
	*object = NULL;
    }
e_exit:
    free(arg);
    return ret;
}

int pomme_write_file(rpcc_t *rct, 
	pomme_file_t *file,
       	u_int64 off,
       	u_int64 len,
       	void *data)
{
    int ret = 0;
    assert( NULL != file );

    

    return ret;
}
int pomme_stat_file(rpcc_t *rct, char *path,pomme_file_t **file)
{
    int ret = 0 ;
    assert( rct != NULL );
    assert( path != NULL );
    pomme_data_t * arg = malloc(2*sizeof(pomme_data_t));
    assert( arg != NULL );
    char *name = POMME_META_STAT_FILE_S;
    arg[0].size = strlen(name)+1;
    arg[0].data = name;

    arg[1].size = strlen(path)+1;
    arg[1].data = path;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));

    if( ( ret = rct->sync_call(rct,2,arg,&res,0))
	    < 0 )
    {
	debug("Stat error");
	goto err;	
    }
    if(res.size == sizeof(pomme_file_t) )
    {
	*file = res.data;
    }else{
	ret = *(int *)res.data;
	debug("response info:%d",*(int *)res.data);
	goto err;
    }
    return ret;
err:
    free(arg);
    return ret;

}

/**
 * @brief pomme_get_ds 
 *
 * @param rct
 * @param id
 * @param ds: should be free by the caller
 *
 * @return 
 */
int pomme_client_get_ds(rpcc_t *rct, u_int32 id, ds_node *ds)
{
    int ret = 0;
    assert( rct != NULL );
    assert( ds != NULL );

    pomme_data_t *arg = malloc(2*sizeof(pomme_data_t));
    assert( arg != NULL );
    char *name = POMME_META_GET_DS_S;
    arg[0].size = pomme_strlen(name);
    arg[0].data = name;

    arg[1].size = sizeof(u_int32);
    arg[1].data = &id;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));

    if( ret = rct->sync_call(rct, 2 , arg, &res, 0 ) < 0 )
    {
	debug("Get Ds info error");
	goto err;
    }
    if( res.size == sizeof(ds_node) )
    {
	memcpy(ds, res.data, sizeof(ds_node));
    }else{
	debug("return code:%d",res.size);
	ret = res.size;
    }
    pomme_data_t *pre = &res;
    pomme_data_distroy(&pre);
err:
    free(arg);
    return ret;
}

int pomme_uuid_create(uuid_t id)
{
    uuid_generate_time(id);
    return 0;
}

int pomme_client_lock_file(rpcc_t *rct, char *path,
	int interval)
{
    int ret = 0;
    assert( rct != NULL );
    assert( ds != NULL );

    pomme_data_t *arg = malloc(2*sizeof(pomme_data_t));
    assert( arg != NULL );
    char *name = POMME_LOCK_S;
    arg[0].size = pomme_strlen(name);
    arg[0].data = name;

    arg[1].size = pomme_strlen(path);
    arg[1].data = path;

    arg[2].size = sizeof(data);
    arg[2].data = &interval;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));

    if( ret = rct->sync_call(rct, 2 , arg, &res, 0 ) < 0 )
    {
	debug("Get Ds info error");
	goto err;
    }
    if( res.size == sizeof(ds_node) )
    {
	memcpy(ds, res.data, sizeof(ds_node));
    }else{
	debug("return code:%d",res.size);
	ret = res.size;
    }
    pomme_data_t *pre = &res;
    pomme_data_distroy(&pre);
err:
    free(arg);
    return ret;
}

int pomme_client_extend_lock(rpcc_t *tct, char *path,
       	int previous, int inteval)
{
    return 0;
}
int pomme_client_release_lock(rpcc_t *rct, char *path,
	int previous)
{
    return 0;
}
