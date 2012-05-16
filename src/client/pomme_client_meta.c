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

int pomme_sync_create_file(rpcc_t *rct, char *path,int mode)
{
    int ret = 0;
    assert( rct != NULL );
    assert( path != NULL );

    pomme_data_t *arg = malloc(3*sizeof(pomme_data_t));
    char *name = POMME_META_CREATE_FILE_S;
    arg[0].size = strlen(name)+1;
    arg[0].data = name;

    arg[1].size = strlen(path)+1;
    arg[1].data = path;

    arg[2].size = sizeof(int);
    arg[2].data = &mode;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));

    if( (ret = rct->sync_call(rct,3,arg,&res,0) ) < 0) 
    {
	debug("Error call");
    }else{
	debug("created");
    }
    return ret;
}
int pomme_sync_read_file_meta(rpcc_t *rct,char *path,pomme_file_t *file, ms_object_t *object)
{
    int ret = 0; 
    assert(rct != NULL);
    assert(path != NULL);

    pomme_data_t *arg = malloc(2*sizeof(pomme_data_t));
    char *name = POMME_META_READ_FILE_S;
    arg[0].size = pomme_strlen(name);
    arg[0].data = name;

    pomme_data_t res;
    memset(&res,0,sizeof(pomme_data_t));

    if( ( ret = rct->sync_call(rct, 2, arg, &res, 0 )) != 0 )
    {
	debug("Call read file failure:%d",ret);
	goto e_exit;
    }else{
	debug("Read file meta ok");
    }
    file = res.data;
    object = res.data + sizeof(pomme_file_t);
e_exit:
    return ret;
}

int pomme_write_file(rpcc_t *rct, char *path, u_int64 off, u_int64 len)
{
    int ret = 0;

    return ret;
}
int pomme_stat_file(rpcc_t *rct, char *path,pomme_file_t *file)
{
    int ret = 0 ;
    assert( rct != NULL );
    assert( path != NULL );
    pomme_data_t * arg = malloc(2*sizeof(pomme_data_t));
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
    if(res.size == sizeof(pomme_file_t)&& (*(int *)res.data) == 0 )
    {
	memcpy(file, res.data+1, sizeof(pomme_file_t));
    }else{
	ret = *(int *)res.data;
	debug("response info:%d",*(int *)res.data);
    }
    return ret;
err:
    return ret;

}
