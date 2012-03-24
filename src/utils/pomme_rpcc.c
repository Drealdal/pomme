/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcc.c
 *
 *    Description:  The implement of rpc client
 *
 *        Version:  1.0
 *        Created:  03/23/2012 10:51:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcc.h"

static int sync_call(rpcc_t *rh, int n,
	pomme_data_t *argu, 
	pomme_data_t **res,
	int time_out = -1);

static int asyn_call(rpcc_t *rh, int n,
	pomme_data_t *argu,
	pomme_data_t **res,
	int time_out = -1);

int pomme_rpcc_init(rpcc_t *rh,
	u_int32 ip,
	u_int16 port,
	u_int32 time_out)
{
    int ret = 0;
    assert( rh != NULL );
    
    rh->ip = ip;
    rh->port = port;
    rh->default_timeout = time_out;

    rh->sync_call = &sync_call;
    rh->asyn_call = &asyn_call;

    return ret;
}
