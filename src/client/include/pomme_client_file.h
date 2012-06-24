/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_file.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/17/2012 11:58:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CLINET_FILE_H
#define _POMME_CLINET_FILE_H
#include "pomme_file.h"
#include "pomme_meta.h"

typedef struct client_object
{
    uuid_t id;
    u_int64 off;
    u_int64 len;
    int dirty;// has not been flush to server
    queue_body_t next;
}cobject_t;
typedef struct pomme_client_file
{
    u_int64 inode;
    pomme_file_t meta;
    ms_object_t *object;
    int ocount;// the object count when opening file
    int ncount;// the file create by the client
    pomme_queue_t nobject;// new object
    queue_body_t next;
}PFILE;
#endif
