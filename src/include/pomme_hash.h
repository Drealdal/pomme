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
#define POMME_DATA_NEED_FREE 1  // The pomme_data_t->data is alloc by the system, need free

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
	pomme_link_t **table;//where the data goes
	int (* hash_func)(void *data);
	int (*cmp_func)(void *key1,void *key2);
}pomme_hash_t;

typedef struct pomme_data{
	u_int32 size;
	u_int32 flags;//the operation for the flag, BDB likely interface 
	void *data;
}pomme_data_t;
/*
 * if hash == NULL ,the init function will Malloc for it then init the data
 * init will malloc mem for table
 * h_func: the hash_function used by this hashtable
 * c_func: the cmpare function used by this hash table
 */
int pomme_hash_init(int size, int(*h_func)(void *),int(*c_func)(void *,void *),pomme_hash_t **hash);

/*-----------------------------------------------------------------------------
 *  distory a hash structure will free all the data in the hash table,includeing
 *  data in pomme_data_t structure
 *-----------------------------------------------------------------------------*/
int pomme_hash_distroy(pomme_hash_t **hash);
/*
 * add a key/data pair into the hashTable
 */
int pomme_hash_put(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data);
//int pomme_hash_put_value(pomme_hash_t *hash, void *key, u_int32 key_len, void *value, u_int32 value_len);
/*
 * get the data of key from hash table
 * the application will repsonse for the mem for data
 */
int pomme_hash_get(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data);
#endif
