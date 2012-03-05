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
	int len,
	int flags)
{
	/*
	 * what should consider is whether we should
	 * use async way to send, of use sync interface
	 */
    int ret = 0;
    int len_to_send, first_msg_send;
    pomme_protocol_t pro; 
    memset(&pro, 0, sizeof(pomme_protocol_t));
    pro.op = put_data;
    pro.total_len = len;


    first_msg_send = len > POMME_MAX_PROTO_DATA ? POMME_MAX_PROTO_DATA:len;
    pro.len = first_msg_send;
    // how to handle the data should be very serious considered
    // reduce the cost of memcopy ,that mean we should reorganize the
    // struct of pomme_protocol_t

    pomme_pack_t *buf = NULL;

    ret = pack_msg( &pro , &buf);
    if( ret < 0 )
    {
    	goto err;
    }

    ret = pomme_send(handle, buf,
    		pomme_msg_len((&pro)), flags);
    if( ret < 0 )
    {
    	debug("send package failure");
    	goto err;
    }

    if( len - first_msg_send > 0  )
    {
    	ret = pomme_send(handle, buf + POMME_PACKAGE_SIZE,
    			len - first_msg_send, flags);
    	if( ret < 0 )
    	{
    		debug("send rest data of the package failure");
    	}
    }

err:
    return ret;
}
