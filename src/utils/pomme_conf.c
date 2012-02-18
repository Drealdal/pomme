/*
 * =====================================================================================
 *
 *       Filename:  pomme_conf.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2012 11:43:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_conf.h"
int pomme_conf_set(DB *db_handle,
	DB_TXN *txnid,
       void *key_data, int key_len,
	void *val_data, int val_len       
       )
{
    unsigned int flags = 0;
    int ret = 0;
    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT)); 

    key.data = key_data;
    key.size = key_len;

    val.data = val_data;
    val.size = val_len;

    ret = db_handle->put(db_handle, txnid, &key, &val, flags);
    if( ret < 0 )
    {
	debug("put to database fail: %s",db_strerror(ret));	
	ret = POMME_WRITE_DB_ERROR;
	goto err;
    }
err:
    return ret;
}
int pomme_conf_get(DB *db_handle,
	DB_TXN *txnid,
	void *key_data,int key_len,
       void *val_data, int *val_len)	
{
    unsigned int flags = 0;
    int ret = 0;
    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT)); 

    key.data = key_data;
    key.size = key_len;
    
    val.data = val_data;
    val.ulen = *val_len;
    val.flags |= DB_DBT_USERMEM;

    ret = db_handle->put(db_handle, txnid, &key, &val, flags);
    if( ret < 0 )
    {
	debug("put to database fail: %s",db_strerror(ret));	
	ret = POMME_READ_DB_ERROR;
	goto err;
    }
    /*
     * what will happen if the val_len < size , the error will be ocour
     */
    *val_len = val.size;
err:
    return ret;

}
