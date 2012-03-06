/*
 * =====================================================================================
 *
 *       Filename:  pomme_storage.h
 *
 *    Description:  Dealing with local file storage
 *
 *        Version:  1.0
 *        Created:  01/11/2012 06:16:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_STORAGE_H
#define _POMME_STORAGE_H
#include "utils.h"
#include "db.h"
#include <dirent.h>

typedef enum storage_status {FULL=0,CUR}storage_status_t;
typedef struct pomme_local_file
{
	u_int64 id;
	int8 *path;
}pomme_local_file_t;

typedef struct pomme_ds_head
{
    int magic;
    int id;
    storage_status_t status;
    
}pomme_ds_head_t;

/**
 * @brief create_storage 
 *
 * @param db_handle: the handle of the database where the storage infomation 
 * @param txnid: the transaction to protect the operation
 * @param id: the id of the created local file will be return here
 *
 * @return 
 */
int create_storage(DB *db_handle,
	DB_TXN *txnid,
	char *storage_path,
	size_t *id);
/**
 * @brief check_storage_stat: check the state of the storage file
 * @param db_handle: the handle of the database where the storage file
 * @param txnid: the transaction to protect the operation
 * @param id:  the id of the file
 *
 * @return < 0 means error , > 0 the state of the storage file
 */
int check_storage_stat(DB *db_handle,
		DB_TXN *txnid,
		size_t id);
/*
 * @brief get_storage_file: get a storage file to use
 *
 */
int get_strorage_file(DB *db_handle,
		DB_TXN *txnid,
		size_t *id);

/**
 * @brief put_data_2_storage put some data to the storage file
 *
 * @param file_handle: the handle of the file
 * @param data: pointer to the data to be stored
 * @param len: the lenth of the data
 * @param start: the start point of the offset
 *
 * @return == 0 for success , < 0 for error 
 */
int put_data_2_storage(int file_handle,
	void *data,
       	size_t len,
	off_t *start); 

int set_file_head( int fd, int id);
int is_file_valid(int fd , pomme_ds_head_t *head);

#endif
