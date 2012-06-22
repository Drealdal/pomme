/*
 * =====================================================================================
 *
 *       Filename:  pomme_ms_iml.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/02/2012 10:59:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_MS_IML_H
#define _POMME_MS_IML_H
#include "utils.h"
#include "pomme_log.h"
#include "pomme_meta_server.h"


/**
 * @brief pomme_create_file: create a file
 *
 * @param ms
 * @param path: the path of the file
 * @param mode
 *
 * @return the inode number of the file 
 */
pomme_data_t * pomme_create_file(pomme_ms_t *ms,const char *path,const int mode);

pomme_data_t * pomme_read_file(pomme_ms_t *ms, u_int64 inode );

/**
 * @brief pomme_write_file 
 * @param id: can be generate by metaserver or client?
 */
pomme_data_t * pomme_write_file(pomme_ms_t *ms, 
	u_int64 inode,
	uuid_t id,
	u_int64 offset,
       	u_int64 len);

pomme_data_t * pomme_stat_file(pomme_ms_t *ms, u_int64 inode);

pomme_data_t * pomme_heart_beat(pomme_ms_t *ms,pomme_hb_t *hb);

/**
 * @brief pomme_lock: lock a file, only the 
 * 		      client with the lock can write the file 
 * @return: if the lock is successed, the time of expire will be returned
 */
pomme_data_t * pomme_lock(pomme_ms_t *ms, u_int64 inode,
	int interval);
/**
 * @brief pomme_extend_lock: the holder of the file is trying to extend lock time 
 *
 * @param previous: the last end time of the lock, only who hold the lock can know this infomation
 * @param interval: time to hold,
 *
 * @return: return time(NULL)+interval 
 */
pomme_data_t * pomme_extend_lock(pomme_ms_t *ms, u_int64 inode,
       	time_t previous, time_t interval);
/**
 * @brief pomme_release_lock 
 * @param previous: the expire time of the lock to indicate the ownership
 * @return:  
 */
pomme_data_t * pomme_release_lock(pomme_ms_t *ms, u_int64 inode, time_t previous);

int pomme_map_ds_group(const char *path);

int ms_start(pomme_ms_t *ms);
int ms_stop(pomme_ms_t *ms);

pomme_data_t *pomme_get_ds(pomme_ms_t *ms, u_int32 id);
pomme_data_t *pomme_all_ds(pomme_ms_t *ms);

/**
 * @brief pomme_mkdir: insert an key-value pair in the database
 * @param ms: 
 * @param path the path of the object
 * @return 
 */
pomme_data_t *pomme_mkdir(pomme_ms_t *ms, const char *path);
pomme_data_t *pomme_dirinit(pomme_ms_t *ms, const char *path);


/**
 * @return new object id
 * 10bit for metaserver, rest 54 will be used to identify the object
 * on a single node
 */
int  ms_create_object(pomme_ms_t *ms, uuid_t id);

int  ms_create_ds(pomme_ms_t *ms, u_int32 *id, u_int32 *group); 

#endif
