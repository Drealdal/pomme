/*
 * =====================================================================================
 *
 *       Filename:  pomme_const.h
 *
 *    Description:  the defination of consts
 *
 *        Version:  1.0
 *        Created:  01/11/2012 09:48:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CONST_H
#define _POMME_CONST_H

/*
 * error code definitions
 */
#define POMME_SUCCESS 0
#define POMME_RPC_EXIT 1
                                                /* local file errors */
#define POMME_LOCAL_DIR_ERROR (-101) 
#define POMME_LOCAL_FILE_ERROR (-102)
#define POMME_WRITE_FILE_ERROR (-103)
#define POMME_FILE_NOT_VALID (-104)
#define POMME_FILE_MULTI_CUR (-105)
                                                /* general error code */
#define POMME_MEM_ERROR (-201)
#define POMME_INVALID_PACKAGE (-202)
#define POMME_TOO_LARGE_PACKAGE (-203)
#define POMME_NOT_ENOUGH_UNPACK (-204)
#define POMME_WRONG_ARG (-204)
#define POMME_HASH_NOT_FOUND (-205)
                                                /* transport errors */
#define POMME_SEND_MSG_ERROR (-301)	
#define POMME_RECV_MSG_ERROR (-302)
#define POMME_UNKNOWN_MSG (-303)
#define POMME_WRITE_MSG_ERROR (-304)
#define POMME_READ_MSG_ERROR (-305)
                                         /* db errors */
#define POMME_WRITE_DB_ERROR (-401)
#define POMME_READ_DB_ERROR (-402)
#define POMME_DB_ERROR (-403)
                                                /** thread pool */
#define POMME_THREAD_POOL_NOTINIT (-501)
#define POMME_THREAD_POOL_FULL (-502)
#define POMME_TOO_MANY_THREAD (-503)
                                                /** rpc */
#define POMME_UNMATCH_ARGU_NUM (-601)
#define POMME_UNMATCH_ARGU_TYPE (-602)
#define POMME_ERROR_EXE (-603)
#define POMME_UNFIND_FUNC (-604)
#define POMME_CONNECT_ERROR (-605)
#define POMME_WRITE_ARGU_ERROR (-606)
#define POMME_RPC_NOT_IMPL (-607)
/* read response data error */
#define POMME_READ_RES_ERROR (-608)
                                                /** Error to client */
#define POMME_META_FILE_EXIST (-700) 
#define POMME_META_INTERNAL_ERROR (-701)
#define POMME_META_FILE_NOT_FOUND (-702)
#define POMME_META_OFFSET_TOO_LARGE (-703)
#define POMME_META_DATA_NODE_NOTFOUND (-704) 
                                                /** lock */
#define POMME_LOCK_EXPIRE (-801)


/*
 * trying to recv msg from a shutdown peer
 */
#define POMME_SHUTDOWN_PEER (-303)   

/*
 * flags definitions
 */

#define POMME_DATA_NEED_FREE 1  // The pomme_data_t->data is alloc by the system, need free
#define POMME_SELF_NEED_FREE 2 // THE pomm_data_t structure is alloc by the system , need free
/*
 * const data definitions
 */
#define POMME_PACKAGE_MAGIC 0x1e
#define POMME_STORAGE_MAGIC 0X1e

#define POMME_DATA_LOG_LEVEL POMME_LOG_ALL
#define POMME_META_LOG_LEVEL POMME_LOG_ALL

#define POMME_LITTLE_ENDIAN 1
#define POMME_BIG_ENDIAN 2

#define POMME_PACKAGE_SIZE 1024
#define POMME_MAX_PACKAGE_SIZE (1024*1024*1024)

#define POMME_PATH_MAX 1024
#define POMME_MAX_REPLICA 5

#define POMME_CLIENT_BUFFER_DS 100
#define POMME_CLIENT_BUFFER_MS 100

#define POMME_DATA_HEART_BEAT_INTERVAL 10
#define POMME_META_FILE_LOCK_TIME 30
/*
 * configure change to configure file???
 */
#define POMME_DATA_PORT 0x1e00
#define POMME_RPC_PORT 0x1E01
#define POMME_META_RPC_PORT 0x1e02

#define POMME_META_FILE "/home/zhumeiqi/work/pomme/data/pomme_meta.db"
#define POMME_META_NAME "pomme_meta"

#define POMME_DATA_NODES_FILE "/home/zhumeiqi/work/pomme/data/pomme_data_nodes.db"
#define POMME_DATA_NODES_NAME "data_nodes"

#define POMME_LOCK_MANAGER_FILE  "/home/zhumeiqi/work/pomme/data/pomme_lock_manager.db"
#define POMME_LOCK_MANAGER_NAME  "lock_manager"

#define POMME_META_ENV_HOME "/home/zhumeiqi/work/pomme/datam/"


#define POMME_DATA_HOME "/tmp/"
#define POMME_MFILE_NAME "/home/zhumeiqi/work/pomme/data/mfile.db"
#define POMME_SFILE_NAME "/home/zhumeiqi/work/pomme/data/sfile.db"
/*  rpc configure xml lables */
#define RPC_SERVER "server"
#define RPC_NAME "name"
#define RPC_PARAM "param"
#define RPC_PARAMS "params"
#define RPC_PARAM_TYPE "type"
#define RPC_PARAM_NAME "name"

#define RPC_FUNCTIONS "functions"
#define RPC_FUNCTION "function"
#define RPC_FUNCTION_NAME "name"
#define RPC_FUNCTION_RETURN "return"
#define RPC_FUNCTION_RETURN_TYPE "type"


/*  function names */
#define POMME_META_CREATE_FILE create_file
#define POMME_META_CREATE_FILE_S "create_file"

#define POMME_META_READ_FILE read_file
#define POMME_META_READ_FILE_S "read_file"

#define POMME_META_STAT_FILE stat_file
#define POMME_META_STAT_FILE_S "stat_file"

#define POMME_META_WRITE_FILE write_file
#define POMME_META_WRITE_FILE_S "write_file"

#define POMME_META_HEART_BEAT heart_beat
#define POMME_META_HEART_BEAT_S "heart_beat"

#define POMME_META_GET_DS get_ds
#define POMME_META_GET_DS_S "get_ds"

#define POMME_META_ALL_DS all_ds
#define POMME_META_ALL_DS_S "all_ds"
                                                /** lock */
#define POMME_LOCK lock
#define POMME_LOCK_S "lock" 

#define POMME_EXTEND_LOCK extend_lock
#define POMME_EXTEND_LOCK_S "extend_lock"

#define POMME_RELEASE_LOCK release_lock
#define POMME_RELEASE_LOCK_S "release_lock"

#endif

