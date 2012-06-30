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


int pomme_client_put_data(
	uuid_t id,
	int handle, 
	void *buffer,
	int len,
	int flags)
{
	/*
	 * what should consider is whether we should
	 * use async way to send, of use sync interface
	 */
    int ret = 0;
    int first_msg_send;
    pomme_protocol_t pro; 
    memset(&pro, 0, sizeof(pomme_protocol_t));
    pro.op = put_data ;
    pro.total_len = len ;

    uuid_copy(pro.id, id);


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

//    ret = pomme_send(handle, buf->data,
//    		pomme_msg_len((&pro)), flags);

    ret = pomme_send(handle, buf->data,
    		buf->cur, flags);
    if( ret < 0 )
    {
    	debug("send package failure");
    	goto err;
    }

    if( len - first_msg_send > 0  )
    {
    	ret = pomme_send(handle, buf->data+ POMME_PACKAGE_SIZE,
    			len - first_msg_send, flags);
    	if( ret < 0 )
    	{
    		debug("send rest data of the package failure");
    	}
    }

err:
    pomme_pack_distroy(&buf);
    return ret;
}

int pomme_client_get_data(uuid_t id,
	size_t off,
	size_t len,
	int handle,
	void *buffer,
	int *r_len)
{
    assert( buffer !=NULL );
    assert( r_len  != NULL );
    int ret = 0,flags=0;

    pomme_protocol_t pro,rpro;
    memset( &pro, 0, sizeof(pro));
    memset( &rpro, 0, sizeof(rpro));

    pro.op = get_data;
    pro.total_len = len;
    pro.offset = off;
    uuid_copy(pro.id, id);

    pomme_pack_t *buf = NULL;
    if( ( ret = pack_msg(&pro, &buf) ) < 0 )
    {
	debug("pack msg failure");
	goto err;
    }

    if( ( ret = pomme_send(handle, buf->data, 
		    pomme_msg_get_len((&pro)),flags )) < 0)
    {
	debug("send data fail");
	goto err;
    }
    /*
     * recv data package
     */
    unsigned char t_buffer[POMME_PACKAGE_SIZE];
    size_t t_len = 0;

    if( ( ret = pomme_recv(handle, t_buffer, 
		    POMME_PACKAGE_SIZE,
	    &t_len, flags) ) < 0 )
    {
	debug("recv first fail");
	goto err;
    }

    pomme_pack_t *p_buffer = NULL;
    if( (ret = pomme_pack_create(&p_buffer,t_buffer,
		    t_len)) < 0 )
    {
	debug("create buffer error");
	goto err;
    }	

    if( (ret = unpack_msg( &rpro, p_buffer) ) < 0 )
    {
	debug("unpack msg fail");
	goto data_err;
    }

    pomme_print_proto(&rpro,NULL);
    if( rpro.op != put_data )
    {
	debug("wrong operation");
	goto data_err;
    }
    if( rpro.total_len > len )
    {
	debug("too much data recved");
	goto data_err;
    }

    size_t tr_len = rpro.len;
    memcpy(buffer,t_buffer, tr_len);

    while( tr_len < rpro.total_len )
    {
	size_t tmp = 0;
	ret = pomme_recv(handle, buffer+tr_len, 
		rpro.total_len - tr_len, 
		&tmp, flags);
	if( ret < 0 )
	{
	    debug("recv err,data total get:%d",tr_len);
	    break;
	}
	tr_len += tmp;
    }
    *r_len = tr_len;
    debug("data len %d",tr_len);
data_err:
    pomme_pack_distroy(&p_buffer);

err:
    pomme_pack_distroy(&buf); 
    return ret;
}

int pomme_client_close(int handle)
{
    int ret;
    int flags = 0;
    pomme_protocol_t pro;
    memset( &pro, 0, sizeof(pro));

    pro.op = data_close;
    pomme_pack_t *buf = NULL;
    if( ( ret = pack_msg(&pro, &buf) ) < 0 )
    {
	debug("pack msg failure");
	goto err;
    }

    if( ( ret = pomme_send(handle, buf->data, 
		    pomme_msg_get_len((&pro)),flags )) < 0)
    {
	debug("send data fail");
	goto buf_err;
    }
    close(handle);
buf_err:
    pomme_pack_distroy(&buf);
err:
    return ret;

}
