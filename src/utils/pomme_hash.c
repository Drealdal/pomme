/*
 * =====================================================================================
 *
 *       Filename:  pomme_hash.c
 *
 *    Description:  The implement of hash table 
 *
 *        Version:  1.0
 *        Created:  12/13/2011 06:21:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"
#include "pomme_hash.h"
#include <assert.h>
static inline int init_hash_node(pomme_hash_node_t **node); 
static inline int distroy_hash_node(pomme_hash_node_t **node);

/*-----------------------------------------------------------------------------
 *  init pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_init(pomme_data_t *data, u_int32 size)
{
	if( data == NULL )
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to do an init operation on an Null pointer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return -1;
	}
	if(data->data != NULL && (data->flags & POMME_DATA_NEED_FREE) != 0 )
	{
		if(data->size >= size)
		{
			memset(data->data,0,data->size);
			data->size = size;
			return 0;
		}else{
			free(data->data);
		}
	}
	data->size = size;
	data->data = malloc(size);
	if(data->data == NULL)
	{
#ifdef DEBUG
		fprintf(stderr,"Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
	}
	memset(data->data,0,size);
	data->flags |= POMME_DATA_NEED_FREE; 
	return 0;
}

/*-----------------------------------------------------------------------------
 *  distroy a pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_distroy(pomme_data_t *data)
{
	if(data == NULL)
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to Distroy an Null Pointer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return 0;
	}
	if( (data->flags & POMME_DATA_NEED_FREE) != 0 && data->data != NULL)
	{
		free(data->data);
	}
	return 0;
}

int pomme_hash_init(int size,int(*h_func)(void *,u_int32),int(*c_func)(void *,void *) ,pomme_hash_t **hash)
{
	u_int32 flags = 0;
	int i = 0;
	if( *hash == NULL )
	{
		*hash = malloc(sizeof(pomme_hash_t));	
		if( *hash == NULL )
		{
#ifdef DEBUG
			printf("Malloc error %s %s %d\n",__FILE__,__func__,__LINE__);
#endif
			goto error_exit;
		}
		flags |= POMME_HASH_NEED_FREE;
	}else{
		// distory the hash table
	}
	pomme_hash_t *p_hash = *hash;
	p_hash->size = size;
	p_hash->hash_func = h_func;
	p_hash->cmp_func = c_func;
	p_hash->table = malloc(size*sizeof(pomme_link_t));
	if(p_hash->table == NULL)
	{
#ifdef DEBUG
		printf("Malloc error %s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		goto error_exit;
	}
	for( i = 0; i< size;i++)
	{
		init_link(&(p_hash->table[i]));

	}
	return 0;
error_exit:
	if( (flags&POMME_HASH_NEED_FREE) != 0)
	{
		free(p_hash);
	}
	return -1;
}

/*-----------------------------------------------------------------------------
 *  distroy a hash structure
 *-----------------------------------------------------------------------------*/

int pomme_hash_distroy(pomme_hash_t **hash)
{
	if( *hash == NULL)
	{
#ifdef DEBUG
		printf("Trying Distroy an Null hash table\n");
#endif

		return 0;
	}
	pomme_hash_t *p_hash = *hash;
	u_int32 flags = p_hash->flags;
	u_int32 size = p_hash->size;
	pomme_link_t *table = p_hash->table;

	uint i = 0;
	for( i = 0; i < size; i++)
	{
		pomme_link_t * p_table = &table[i];
		pomme_hash_node_t *pos = NULL;
		list_for_each_entry(pos,p_table,link)
		{
			distroy_hash_node(&pos);
		}
	}
	free(p_hash->table);
	if( (flags&POMME_HASH_NEED_FREE) != 0)
	{
		free(p_hash);
		*hash = NULL;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
 *  init a hash node 
 *-----------------------------------------------------------------------------*/

static inline int init_hash_node(pomme_hash_node_t **node)
{
	if(*node==NULL)
	{
		*node = malloc(sizeof(pomme_hash_node_t));
#ifdef DEBUG
		printf("Malloc Error @%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return -1;
	}
	memset(*node,0,sizeof(pomme_hash_node_t));
	return 0;
}

/*-----------------------------------------------------------------------------
 *  destroy a node
 *-----------------------------------------------------------------------------*/
static inline int distroy_hash_node(pomme_hash_node_t **node)
{
	if( *node == NULL )
	{
		return 0;
	}
	free((*node)->value);
	free((*node)->key);
	free(*node);
	return 0;
}

/*-----------------------------------------------------------------------------
 *  add an item into the hash table
 *-----------------------------------------------------------------------------*/
int pomme_hash_put(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data)
{
	u_int32 ipos = (*hash->hash_func)(key->data,key->size)%(hash->size);
	pomme_link_t *p_link = &hash->table[ipos];
	pomme_hash_node_t *pos = NULL;
	list_for_each_entry(pos,p_link,link)
	{
		if(&pos->link == p_link)
		{
			break;
		}
		if( (pos->key_len == key->size) &&
				( 0 == hash->cmp_func(key->data,pos->key)))
		{
			//find the same key in the hash table
			if( (data->flags&POMME_DATA_NEED_FREE) != 0 )
			{                       
				/* the memory of the data is malloc by the sys,so we use is 
				 * directly, and free the origin data
				 **/
				pos->value = data->data;
				data->data = pos->value;
				int t = pos->value_len;
				pos->value_len = data->size;
				data->size = t;

			}else if(data->size <= pos->value_len)
			{
				/*The origin mem is bigger than the mem is needed,
				 * so we copy the data into the original mem and update
				 * the new length.
				 */
				pos->value_len = data->size;
				memcpy(pos->value,data->data,data->size);
			}else{
				/* The need memory size is bigger than current
				 * memory size ,so we realloc the mem,we need handle the 
				 * */
				free(pos->value);
				pos->value = malloc(data->size);
				if(pos->value == NULL)
				{
#ifdef DEBUG
					printf("Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
					return -1;
				}
				memcpy(pos->value,data->data,data->size);
				pos->value_len = data->size;
			}
			return 0;
		}
		if(pos->link.next == LIST_POSITION_2)
		{
			break;
		}
	}
	pomme_hash_node_t *node = malloc(sizeof(pomme_hash_node_t));
	if(node == NULL)
	{
#ifdef DEBUG
		printf("Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		goto malloc_node_error;
	}
	memset(node,0,sizeof(pomme_hash_node_t));
	init_link(&(node->link));
	//INIT_LINK((&(node->link)),struct pomme_hash_node,link);
	node->key_len = key->size;
	node->key = malloc(key->size);
	if( node->key == NULL)
	{
#ifdef DEBUG
		printf("Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		goto malloc_key_error;
	}
	memcpy(node->key,key->data,key->size);
	node->value_len = data->size;
	node->value = malloc(data->size);
	if( node->value == NULL )
	{
#ifdef DEBUG
		printf("Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		goto malloc_data_error;
	}
	memcpy(node->value,data->data,data->size);
	link_add(&node->link,p_link);
	return 0;


malloc_data_error:
	free(node->key);
malloc_key_error:
	free(node->value);
	free(node);
malloc_node_error:
	return -1;
	/*
	 * not find the same key for the key
	 */
}

/*-----------------------------------------------------------------------------
 *  delete an item from the hash table
 *-----------------------------------------------------------------------------*/
int pomme_hash_get(pomme_hash_t *hash, pomme_data_t *key, pomme_data_t *data)
{
	/*
	 * The value will be copy into data->data,and if data->data is null and
	 * the POMME_DATA_NEED_FREE is not set,we will copy the data into the public
	 * buffer given in hash_t that means ,it is not safe if serveral thread are
	 * access the hash table.Or after serveral get method call,all the data->data
	 * will point to the same buffer 
	 */
	if( hash == NULL )
	{
#ifdef DEBUG
		printf("Get from an null hash table@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return -1;
	}
	if( key == NULL || data == NULL )
	{
#ifdef DEBUG
		printf("Arg is wrong: %s\n",key?"key Null":"value Null");
#endif
		return -1;
	}
	u_int32 p = (*hash->hash_func)(key->data,key->size)%hash->size;
	pomme_link_t *p_link = &hash->table[p];
	if(p_link == NULL)
	{
		/* not found ,the size is set to 0, return value is success */
		data->size = 0;
		return 0;
	}
	pomme_hash_node_t *pos = NULL;
	list_for_each_entry(pos,p_link,link)
	{
		if( (pos->key_len == key->size) &&
				( 0 == hash->cmp_func(key->data,pos->key)))
		{
			printf("find\n");
			if( data->size < pos->value_len )
			{
				/*
				 * the length of the value is bigger than expected,
				 * copy the data into the mem but return value is
				 * set to -1 to indicate any error
				 */
				memcpy(data->data,pos->value,data->size);
				return -1;
			}else{
				memcpy(data->data,pos->value,pos->value_len);
				return 0;
			}
		}
		if(pos->link.next == LIST_POSITION_2)
		{
			break;
		}

	}
	/* not found ,the size is set to 0, return value is success */
	data->size = 0;
	return 0;
}

/*-----------------------------------------------------------------------------
 *  put int to hash table using the values ,not use pomme_data_t
 *  @param: key  the data of the key
 *  @param: key_len the length of the key
 *  @param: value the data of the value
 *  @param: value_len the length of the value data
 *-----------------------------------------------------------------------------*/
int pomme_hash_put_2(pomme_hash_t *hash, void *key, u_int32 key_len, void *value, u_int32 value_len)
{
	pomme_data_t _key,_data;
	memset(&_key,0,sizeof(pomme_data_t));
	memset(&_data,0,sizeof(pomme_data_t));
	_key.size = key_len;
	_key.data = key;
	_data.size = value_len;
	_data.data = value;
	return pomme_hash_put(hash,&_key,&_data);
}

/*-----------------------------------------------------------------------------
 *  str hash,we tread the data of void * to be str
 *  @param : str the pionter to the data
 *  @param : str_len , the length of the data  int BYTE
 *-----------------------------------------------------------------------------*/
int str_hash(void *str,u_int32 str_len)
{
	if(str==NULL)return 0;
	char *data = str;
	u_int32 a = 378551;
	u_int32 b = 63689;	
	u_int32 hash = 0, i = 0;
	for( i = 0 ; i < str_len; i++)
	{
		hash ^= hash*a + data[i];	
		a*=b;
	}	
	u_int32 ret = (hash & 0x7fffffff);
	return ret;
}
