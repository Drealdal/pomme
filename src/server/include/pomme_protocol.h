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
typedef enum pomme_protocol_type{put_data=1,get_data,pomme_close}pomme_protocol_type_t;

typedef struct pomme_protocol
{
    pomme_protocol_type_t op;
    /*
     * the length of data in the package,
     *
     */
    size_t len;
    /*
     * the length of the object data
     */
    size_t total_len;
    uuid_t id;
    size_t offset;
    void *data;
}pomme_protocol_t;



#define pomme_msg_len(pro) (  pro->len+sizeof(pomme_protocol_t) - sizeof(void *) )
#define pomme_msg_get_len(pro) ( sizeof(pomme_protocol_t) -sizeof(void *)+sizeof(pro->len))

#define POMME_MAX_PROTO_DATA ( POMME_PACKAGE_SIZE - sizeof(pomme_protocol_t) +sizeof(void *)) 


int pack_msg( pomme_protocol_t *pro , pomme_pack_t **buf);
int unpack_msg( pomme_protocol_t *pro, pomme_pack_t *buf);
/*
 * printf the content of an pomme_protocol_t structure
 * for debug use
 *
 */

int pomme_print_proto(pomme_protocol_t *pro,int (*data_printer)(void *));




#endif
