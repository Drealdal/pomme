/*
 * =====================================================================================
 *
 *       Filename:  pomme_data_main.c
 *
 *    Description:  the implement of the pomme data server
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:15:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_data_server.h"
int main()
{
    int ret = 0;
    pomme_ds_t ds;
    memset(&ds, 0 , sizeof(pomme_ds_t));
//    ret = pomme_ds_init(&ds);
    if( ret < 0 )
    {
	debug("data server memory init failure");
	goto err;
    }

    int cur_id, cur_fd;
    get_storage_files("/home/zhumeiqi/work/pomme/data",NULL,&cur_id, &cur_fd);
    

  //  server();
err:
    return ret;
}
