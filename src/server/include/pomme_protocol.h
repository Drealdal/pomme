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
#include "pomme_pack.h"
typedef enum pomme_protocol_type{put_data=1,get_data}pomme_protocol_type_t;
typedef struct pomme_protocol
{
    pomme_protocol_type_t op;
    size_t len;
    void *data;
}pomme_protocol_t;


#endif
