/*
 * =====================================================================================
 *
 *       Filename:  _protocol.c
 *
 *    Description:  the implement of the _protocol
 *
 *        Version:  1.0
 *        Created:  02/17/2012 09:14:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_protocol.h"
int pack_msg(pomme_protocol_t *pro, pomme_pack_t **buf)
{
    int ret = 0;
    size_t t_len = 0;

    assert( pro!= NULL );
    assert( buf!= NULL );
    
    pomme_pack_t *p_buf = *buf;
    t_len = pro->len+sizeof(pomme_protocol_type_t)+sizeof(size_t);
    ret =  pomme_pack_create(buf, NULL, t_len);
    
    if( ret < 0 )
    {
	debug("buf create fail");
	goto err;
    }
    
    pomme_pack(&pro->op, struct pomme_protocol, *buf);


    pomme_pack_array(&pro->data, char, pro->len , *buf);

err:
    return ret;
}

int unpack_msg(pomme_protocol_t *pro, pomme_pack_t *buf)
{
    int ret = 0;

    assert( pro != NULL );
    assert( buf != NULL );

    pomme_protocol_type_t *p_op = &pro->op;
    pomme_unpack( &p_op, pomme_protocol_t, buf);

    pomme_unpack_array( &pro->data, char, &pro->len, buf);

err:
    return ret;
}

