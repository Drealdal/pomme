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

typedef struct pomme_local_file
{
	u_int64 id;
	int8 *path;
}pomme_local_file_t;

/**
 * @brief create_storage 
 *
 * @param db_handle: the handle of the database where the storage infomation 
 * @param txnid: the transaction to protext the operation
 * @param id: the id of the created local file will be return here
 *
 * @return 
 */
int create_storage(DB *db_handle,
	DB_TXN *txnid,
	char *storage_path,
	size_t *id);

#endif
