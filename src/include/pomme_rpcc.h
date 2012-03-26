/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcc.h
 *
 *    Description:  rpc client
 *
 *        Version:  1.0
 *        Created:  03/23/2012 07:43:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_RPCC_H
#define _POMME_RPCC_H
#include "pomme_type.h"

typedef struct pomme_rpc_handle rpcc_t;

struct pomme_rpc_handle
{
    char *ip; 
    u_int16 port;
    u_int32 default_timeout;// in ms

    /**
     * @brief caller: call an remote function
     * @param rh: the handle of an rpc client
     * @param n: the number of argument, the first argument should
     * 		 be the name of the functon to call
     * @param argu: argument list
     * @param res:response data
     * @param time_out: max time too wait for the response
     *
     * @return ==0 for success, < 0 failure 
     */
    int (*sync_call)(rpcc_t *rh, int n,
	    pomme_data_t *argu, 
	    pomme_data_t **res,
	    int time_out = -1);

    int (*asyn_call)(rpcc_t *rh, int n,
	    pomme_data_t *argu,
	    pomme_data_t **res,
	    int time_out = -1);
};

int pomme_rpcc_init(rpcc_t *rh,
	char *ip, 
	u_int16 port, 
	u_int32 time_out);
int pomme_rpcc_distroy(rpcc_t *rh);
#endif
