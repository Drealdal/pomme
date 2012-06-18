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
//#define pomme_version 0.01
#include "utils.h"
#include "pomme_log.h"
#include "pomme_hash.h"
#include "pomme_protocol.h"
#include "pomme_type.h"
#include "pomme_meta.h"

typedef struct pomme_env
{
    DB_ENV *db_env;
    /*
     * unused
     */
    DB *conf;

    /* *
     *storage the id and the storage file path
     * */

    DB *sfile;
    char *sfile_file;
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
	pomme_env_t env;
	/*
	 * the home directory of the  data server
	 * and the db_env
	 */
	char *home;
	/* *
	 * where the local data file will be stored
	 * */
	char *data_home;
	unsigned int env_c_flags;
	unsigned int env_o_flags;
	int env_mode;
	/*
	 * the id of the current used storage_file
	 */
	int cur_storage_id;
	int cur_storage_fd;
	/*  master */
	u_int32 mip;
	u_int32 mport;
	/*  meta  */
	u_int32 metaip;
	u_int16 metaport;
	/*  statistics */
	u_int32 ip;
	u_int16 port;
	/*  myid */
	int myid;

}pomme_ds_t; /*
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
	char *meta_file,
	char *sfile_file,
	int mode);
/*
 * @param: distory an pomme_env_t structure
 * @return: <0 error, == 0 success
 */
int pomme_env_distroy(pomme_env_t *env);
/*
 * @param:init an pomme_ds_t structure
 * @param: home, the home dir of pomme_env
 * @param: meta_file, the file name of the meta database
 * @param: sfile_file, the file name of the storage database
 * @env_c_flags: the create flags of the env structure
 * @env_o_flags: the open flags of the env structure
 * @env_mode: the open mode of the ds
 * return: < 0 error, == 0 success
 */
int pomme_ds_init(pomme_ds_t * ds,
	char *home,
	char *meta_file,
	char *sfile_file,
	unsigned int env_c_flags,
	unsigned int env_o_flags,
	int env_mode);
/*
 * @param: distroy an pomme_ds_t structure
 * return: < 0 error, == 0 success
 */
int pomme_ds_distroy(pomme_ds_t *ds);

/**
 * @brief pomme_storage_init: init the inmem structrue for storage infomation
 *
 * @param ds: the structure of data server
 *
 * @return < 0 for failure, ==0 success
 */
int pomme_storage_init(pomme_ds_t *ds);

/*
 * add object
 * @param:ds the structure of the in mem dataserver
 * @@para:handle the handle of the connection
 * @param:pro the recved package, maybe just first part of the data
 */
int pomme_put_object(pomme_ds_t *ds,
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


/**
 * @brief get_storage_files 
 *
 * @param path: the path of dir to scan
 * @param storage: the hash table of stroage files
 * @param cur_id: current used storage file 's id
 * @param cur_fd: current used storage file 's fd
 *
 * @return 
 */
int get_storage_files(char *path, pomme_hash_t *storage,
	int *cur_id,
	int *cur_fd);

/**
 * @brief pomme_data_heart_beat: heart beat
 *
 * @return 
 */
void pomme_ds_heart_beat(int signo);
/*****
 * the first version of the data server
 */
int server(pomme_ds_t *);

int get_storage_files(char *path, pomme_hash_t *storage,
	int *cur_id,
	int *cur_fd);

#endif

