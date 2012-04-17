/*
 * =====================================================================================
 *
 *       Filename:  pomme_hash.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2011 05:30:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_HASH_H
#define POMME_HASH_H
/*
 * rIif the structure table this flag,We can use this mem with copy
 *
 */
#define POMME_HASH_NEED_FREE 1   //The hash_t is alloc by the sys
#include "pomme_type.h"
#include "pomme_blist.h"
#include "pomme_error.h"

typedef struct pomme_hash_node
{
	u_int32 key_len;
	u_int32 value_len;
	void *key;
	void *value;
	pomme_link_t link;
}pomme_hash_node_t;

typedef struct pomme_hash
{
	u_int32 size;// the init size of hash table
	u_int32 flags;//
	pomme_link_t *table;//where the data goes
	int (* hash_func)(void *data,u_int32);
	int (*cmp_func)(void *key1,void *key2);
}pomme_hash_t;

/*
 * if hash == NULL ,the init function will Malloc for it then init the data
 * init will malloc mem for table
 * h_func: the hash_function used by this hashtable
 * c_func: the cmpare function used by this hash table
 */
int pomme_hash_init(int size, int(*h_func)(void *,u_int32),int(*c_func)(void *,void *),pomme_hash_t **hash);

/*-----------------------------------------------------------------------------
 *  distory a hash structure will free all the data in the hash table,includeing
 *  data in pomme_data_t structure
 *-----------------------------------------------------------------------------*/
int pomme_hash_distroy(pomme_hash_t **hash);
/*
 * add a key/data pair into the hashTable
 */
int pomme_hash_put(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data);
int pomme_hash_put_2(pomme_hash_t *hash, void *key, u_int32 key_len, void *value, u_int32 value_len);
/*
 * get the data of key from hash table
 * the application will repsonse for the mem for data
 */
int pomme_hash_get(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data);
/*
 * del data from hash table ,return success if not exist
 */
int pomme_hash_del(pomme_hash_t *hash,pomme_data_t *key);

/*-----------------------------------------------------------------------------
 *  hash a data of length into int
 *-----------------------------------------------------------------------------*/
int str_hash(void *str,u_int32 str_len);

/*-----------------------------------------------------------------------------
 *  cmp to void * data , they are cmpared BYTE WISE
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *  init an pomme_data_t with an memory of length size,if this data is used when
 *  calling the put operation,the memory will be managed by the system,otherwise
 *  the application should do it.
 *  @param: size ,the size of memory needed
 *  @return , if NULL error happens
 *-----------------------------------------------------------------------------*/
int pomme_data_init(pomme_data_t *data,u_int32 size);

/*-----------------------------------------------------------------------------
 *  distory an pomme_data_t structure,manage the Memory allocated by sys
 *-----------------------------------------------------------------------------*/
int pomme_data_distroy(pomme_data_t *data);

/**
 * @brief pomme_hash_int_int : init an hash of <int, int> pair
 *
 * @param size
 * @param hash
 *
 * @return 
 */
int pomme_hash_int_int(int size, pomme_hash_t **hash);

int pomme_hash_int_longlong(int szie, pomme_hash_t **hash);

#endif
