/*
 * =====================================================================================
 *
 *       Filename:  pomme_conf.h
 *
 *    Description:  the interface used to get configuration infomations
 *
 *        Version:  1.0
 *        Created:  02/17/2012 10:40:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CONF_H
#define _POMME_CONF_H

#include "utils.h"
#include "db.h"

/**
 * @brief pomme_conf_set : set a configuration
 *
 * @param db_handle: where to stored the configuration
 * @param txnid: the transaction to protect the operation
 * @param key: the data of the key stored
 * @param key_len: the length of key
 * @param val: the data of the value
 * @param val_len: the length of the value data
 *
 * @return == 0 for success , < 0 for failure 
 */
int pomme_conf_set(DB *db_handle,
	DB_TXN *txnid,
       void *key, int key_len,
	void *val, int val_len       
       );

/**
 * @brief pomme_conf_get 
 *
 * @param db_handle: where to stored the configuration
 * @param txnid: the transaction to protect the operation
 * @param key: the data of the key stored
 * @param key_len: the length of key
 * @param val: the data of the value
 * @param val_len: the max length of the expected value data
 *
 * @return ==0 for success , < for failure 
 */
int pomme_conf_get(DB *db_handle,
	DB_TXN *txnid,
	void *key,int key_len,
       void *val, int *val_len);	
#endif
