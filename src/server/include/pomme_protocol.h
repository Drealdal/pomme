/*
 * =====================================================================================
 *
 *       Filename:  pomme_protocol.h
 *
 *    Description:  the header file of pomme_protocol.h
 *
 *        Version:  1.0
 *        Created:  02/17/2012 09:14:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_PROTOCOL_H
#define _POMME_PROTOCOL_H
#include "utils.h"
#include "pomme_pack.h"
typedef enum pomme_protocol_type{put_data=1,get_data}pomme_protocol_type_t;
typedef struct pomme_protocol
{
    pomme_protocol_type_t op;
    /*
     * the len is the real length of the data
     * if the len is bigger than the len of data,
     * that means there will be more data behind
     * the first message length is smaller than 
     * 1024Byte , than means if the data is longer
     * than (1024-sizeof(op)-sizeof(len))there will
     * be more package
     */
    size_t len;
    void *data;
}pomme_protocol_t;
#define POMME_MAX_PROTO_DATA ( POMME_PACKAGE_SIZE - sizeof( pomme_protocol_type_t) \
       -sizeof(size_t))	

int pack_msg( pomme_protocol_t *pro , pomme_pack_t **buf);
int unpack_msg( pomme_protocol_t *pro, pomme_pack_t *buf);




#endif
