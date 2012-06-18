/*
 * =====================================================================================
 *
 *       Filename:  test_rpcs.c
 *
 *    Description:  test the implemet of rpc
 *
 *        Version:  1.0
 *        Created:  03/26/2012 06:05:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"
#include "pomme_rpcs.h"
/*
 * return the arg pass in
 */
pomme_data_t * remote_print(void *extra,int n , pomme_data_t *arg)
{
    debug("arrg_num:%d",n);
    if( n != 1 )
    {
	return NULL;
    }
    debug("size:%p %d",arg,arg[0].size);
    return arg;
}

int main()
{

    pomme_rpcs_t rpcs ;
    pomme_rpcs_init(&rpcs,NULL,10,100,2,POMME_RPC_PORT);
    pomme_data_t *arg = malloc(sizeof(pomme_data_t));

    memset(arg,0,sizeof(pomme_data_t));
    arg[0].size = -1;
    rpcs.func_register(&rpcs,"remote_print",&remote_print,1,arg);
    rpcs.start(&rpcs);

    return 1;

}
