/*
 * =====================================================================================
 *
 *       Filename:  test_rpcc.c
 *
 *    Description:  The client for test rpcs
 *
 *        Version:  1.0
 *        Created:  03/27/2012 01:17:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcc.h"
#include "pomme_const.h"
#include "utils.h"

int main()
{
    int ret = 0;
    char * host_name = "127.0.0.1";   
    char *name = "remote_print";

    rpcc_t rpcc;
    u_int32 ip = inet_addr(host_name);

    if( (ret = pomme_rpcc_init(&rpcc, ip,
	    POMME_RPC_PORT,0)) < 0 )
    {
	debug("init rpc client error");
	exit(-1);
    }
    pomme_data_t *arg = malloc(2*sizeof(pomme_data_t));
    arg[0].size=strlen(name)+1;
    arg[0].data = name;

    arg[1].size = strlen(name)+1;
    arg[1].data = name;

    pomme_data_t res;
    memset(&res, 0, sizeof(pomme_data_t));
    debug("before call");
   if( (ret = rpcc.sync_call(&rpcc,2, arg,&res,0) ) < 0 )
   {
       debug("remote call error");
   }else{
    printf("Get from server:%s",(char *)res.data);
   }
    free(arg);
    pomme_data_t *pres = &res;
    pomme_data_distroy(&pres);


    return ret;
}
