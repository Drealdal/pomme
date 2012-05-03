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
#include "pomme_rpcs.h"
#include "pomme_log.h"
#include "pomme_hash.h"
#include "pomme_protocol.h"
#include "pomme_type.h"
#include "pomme_meta.h"

typedef struct pomme_ms pomme_ms_t;
struct pomme_ms
{
    logger_t *ms_logger;
    /*
     * the ds < id <--> ip>
     */
    pomme_hash_t *ds;
    /* meta db 
     * Use Btree, alloc multiple data item 
     * for one key
     * */

    DB_ENV *env;

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

    pomme_rpcs_t rpcs;
    int (*start)(pomme_ms_t *ms);
    int (*stop)(pomme_ms_t *ms);
    /* file management */
    pomme_data_t * (*POMME_META_CREATE_FILE)(void *ms,const int n , const pomme_data_t *arg);
    pomme_data_t * (*POMME_META_STAT_FILE)(void *ms, const int n , const pomme_data_t *arg);
    // get all the object of the file
    pomme_data_t * (*POMME_META_READ_FILE)(void *ms, const int n , const pomme_data_t *arg);
    pomme_data_t * (*POMME_META_WRITE_FILE)(void *ms, const int n , const pomme_data_t *arg);
    /* data server group management */
    pomme_data_t * (*POMME_META_HEART_BEAT)(void *ms, const int n, const pomme_data_t *arg);



    pomme_data_t * (*join_dsgroup)(void *ms,int n , pomme_data_t *arg);
    pomme_data_t * (*leave_dsgroup)(void *ms, int n , pomme_data_t *arg);

    /*
     * local 
     */
    int (*get_ds_group)(const char *path);
    /**
     * @brief rpc funtion which will be called by the data
     * server to report infomation 
     */

};
typedef struct heart_beat
{
    u_int32 myId;
    u_int32 myGroup;
    u_int64 objectCount;
    u_int64 requestCount;
}pomme_hb_t;

int pomme_ms_init(pomme_ms_t *ms,
	pomme_log_level_t log_level,
	int env_o_flags,
	int env_o_mode,
	int hash_size,
	int max_thread,
	int max_wating);

#endif
