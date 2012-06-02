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
#include "pomme_serilize.h"
#include "utils.h"

static int sync_call(rpcc_t *rh, int n,
	pomme_data_t *argu, 
	pomme_data_t *res,
	int time_out )
{
    int ret = 0;
    int conn=0;

    assert( res != NULL);

    if( (ret = create_client(rh->ip,
	    rh->port,&conn )) < 0 )
    {
	debug("connect to server error");
	return POMME_CONNECT_ERROR;
    }


    /* write funcnaem */
    if( ( ret = write_data(argu,conn) ) < 0 )
    {
	debug("write error");
	return POMME_WRITE_ARGU_ERROR;
    }
    debug("write fname");


    if( ( ret = pomme_rpc_write(conn, 
		    n-1,argu+1) ) < 0 )
    {
	debug("write error");
	return POMME_WRITE_ARGU_ERROR;
    }

    if( ( ret = read_data(res, conn)) < 0 )
    {
	debug("read response error");
	return POMME_READ_RES_ERROR;
    }	

    return ret;

}

static int asyn_call(rpcc_t *rh, int n,
	pomme_data_t *argu,
	pomme_data_t *res,
	int time_out )
{
    int ret = 0;
    return ret;
}

int pomme_rpcc_init(rpcc_t *rh,
	u_int32 ip,
	u_int16 port,
	u_int32 time_out)
{
    int ret = 0;
    assert( rh != NULL );
    memset( rh , 0 , sizeof(rpcc_t));
/*  
    int nl = strlen(ip);
    rh->ip = malloc(nl+1);
    if(rh->ip == NULL)
    {
	exit(-1);
    }
    strcpy(rh->ip, ip);
   */ 
    rh->ip = ip;
    rh->port = port;
    rh->default_timeout = time_out;

    rh->sync_call = &sync_call;
    rh->asyn_call = &asyn_call;

    return ret;
}

int pomme_rpcc_distroy(rpcc_t *rh)
{
  //  free(rh->ip);
    return 0;
}
