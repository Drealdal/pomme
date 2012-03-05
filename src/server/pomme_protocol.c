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
    t_len = pomme_msg_len(pro);
    //pro->len+sizeof(pomme_protocol_type_t)+2*sizeof(size_t);
    ret =  pomme_pack_create(buf, NULL, t_len);
    
    if( ret < 0 )
    {
	debug("buf create fail");
	goto err;
    }
    
    pomme_pack(&pro->op, pomme_protocol_type_t, *buf);
    pomme_pack(&pro->total_len, size_t , *buf);

    pomme_pack_array(&pro->data, char, pro->len , *buf);
    ret = t_len;
err:
    return ret;
}

int unpack_msg(pomme_protocol_t *pro, pomme_pack_t *buf)
{
    int ret = 0;

    assert( pro != NULL );
    assert( buf != NULL );

    pomme_protocol_type_t *p_op = &pro->op;
    pomme_unpack( &p_op, pomme_protocol_type_t, buf);
    pomme_unpack( &pro->total_len, size_t, buf);
    pomme_unpack_array( &pro->data, char, &pro->len, buf);

err:
    return ret;
}
static char * get_proto_type(pomme_protocol_t * t)
{
	int ret = 0;
	switch(t->op)
	{
	case put_data:
		printf("put_data");
		break;
	case get_data:
		printf("get_data");
		break;
	default:
		ret = POMME_UNKNOWN_MSG;
		break;
	}
	return ret;
}

int pomme_print_proto(pomme_protocol_t *pro,int (*data_printer)(void *))
{
	int ret = 0;
	assert(pro!=NULL);
	printf("OP_TYPE: %s \n",get_proto_type(pro));

	// TODO add  logic to determine how to printf size_t
	// use %u instead now

	printf("total data length: %u\n",pro->total_len);
	printf("pack data length: %u\n",pro->len);
	if(data_printer != NULL)
	{
		printf("The data is:\n");
		ret = data_printer(pro->data);
		if( ret < 0 )
		{
			debug("Wrong data format cased printer pring error");
		}
		printf("\n");
	}
	return ret ;
}
