/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_data.c
 *
 *    Description:  The client logic for data server
 *
 *        Version:  1.0
 *        Created:  03/01/2012 06:03:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_client_data.h"


int pomme_put_data(int handle, 
	void *buffer,
	int len)
{
    int ret = 0;

    pomme_protocol_t pro; 
    memset(&pro, 0, sizeof(pomme_protocol_t));
    pro.op = put_data;
    pro.len = len;



    return ret;
}
