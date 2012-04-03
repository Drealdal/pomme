/*
 * =====================================================================================
 *
 *       Filename:  pomme_bdb.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/17/2011 12:06:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_BDB_H
#define POMME_BDB_H
#include "bdb.h"

/**
 * @brief pomme_db_setvalue 
 *
 * @param db_handle: the handle of the database where the data will be stored
 *
 * @return 0 for sucess , < 0 failure 
 */
int pomme_bdb_setvalue(void *db_handle,
	pomme_data_t *key,
	pomme_data_t *value);

/**
 * @brief pomme_bdb_getvalue 
 * @param db_handle
 * @return 0 for success , < 0 falure
 */
int pomme_bdb_getvalue(void *db_handle,
	pomme_data_t *key,
	pomme_data_t *value);

/**
 * @brief pomme_bdb_getnext 
 *
 * @param db_cusor
 *
 * @return 0 for success, < 0 for failure
 */
int pomme_bdb_getnext(void *db_cusor,
	pomme_data_t *key,
	pomme_data_t *value);
#endif
