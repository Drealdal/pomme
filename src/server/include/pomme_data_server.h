/*
 * =====================================================================================
 *
 *       Filename:  pomme_data_server.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:57:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_DATA_SERVER_H
#define _POMME_DATA_SERVER_H
#define version 0.01
#include "utils.h"

typedef struct pomme_env
{
    DB_ENV *db_env;
    DB *conf;
    /*
     * the db file to storage 
     * the meta infomation of
     * the objects
     */
    DB *db_meta;
    char *meta_file;

    int flags;// reserved
    pthread_mutex_t mutex;
}pomme_env_t;

/*
 * the structure of the mem
 */
typedef struct pomme_ds
{
	logger_t *ds_logger;
	/*
	 *  (id -> fd )
	 *  the id is the id of the storage file
	 *  fd is the handle of the opened file
	 */
	pomme_hash_t *storage_file;
	pomme_env_t *env;
	/*
	 * the home directory of the  data server
	 * and the db_env
	 */
	char *home;
	unsigned int env_c_flags;
	unsigned int env_o_flags;
	int env_mode;
}pomme_ds_t;


/*
 *@param: init an pomme_env_t structure
 *@param: c_flags, the create flags for db_env
 *@param: o_flags, the open flags for db_env
 *@param: home , the home for the db_env
 *@return: < 0 error, == 0 success
 */
int pomme_env_init(pomme_env_t *env,
	unsigned int c_flags,
	unsigned int o_flags,
	char *home,
	int mode);
/*
 * @param: distory an pomme_env_t structure
 * @return: <0 error, == 0 success
 */
int pomme_env_distory(pomme_env_t *env);
/*
 * @param:init an pomme_ds_t structure
 * return: < 0 error, == 0 success
 */
int pomme_ds_init(pomme_ds_t * ds);
/*
 * @param: distroy an pomme_ds_t structure
 * return: < 0 error, == 0 success
 */
int pomme_ds_distroy(pomme_ds_t *ds);
/*
 * add object
 * @param:ds the structure of the in mem dataserver
 * @@para:handle the handle of the connection
 * @param:pro the recved package, maybe just first part of the data
 */
int pomme_add_object(pomme_ds_t *ds,
		int handle,
		pomme_protocol_t *pro);
/*
 * get object
 * @param: ds the in memory structure of the dataserver
 * @param: handle the handle of the connection
 * @param: the pro will content the info to know the object position
 */
int pomme_get_object(pomme_ds_t *ds,
		int handle,
		pomme_protocol_t *pro);


/*****
 * the first version of the data server
 */
int heartbeat();
int server_init();

#endif

