/*
 * =====================================================================================
 *
 *       Filename:  pomme_meta_server.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/03/2012 05:47:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_META_SERVER_H
#define _POMME_META_SERVER_H

//#define pomme_version 0.01
#include "utils.h"
#include "pomme_log.h"
#include "pomme_hash.h"
#include "pomme_protocol.h"
#include "pomme_type.h"
#include "pomme_meta.h"

typedef struct pomme_ms
{
    logger_t *ds_logger;
    /*
     * the ds < id <--> ip>
     */
    pomme_hash_t *ds;
    /* meta db 
     * Use Btree, alloc multiple data item 
     * for one key
     * */
    DB *meta_db;
    /*
     * The infomation of the data node;
     *
     * data_node is organised as group, every
     * object is write to a group,
     * a node is beyond to a group, this means 
     * the data on every node within a group is
     * the same. borrow from facebook (find needle
     * in haystack)
     *
     * considering use diffirent storage file on
     * data node to enable one nodes in serval group
     */
    DB *data_nodes;

}pomme_ms_t;

#endif
