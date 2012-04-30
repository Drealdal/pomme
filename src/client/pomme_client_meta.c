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
#include "utils.h"
#include "pomme_utils.h"
#include "pomme_rpcc.h"

int pomme_create_file(rpcc_t *rct, char *path,int mode)
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
