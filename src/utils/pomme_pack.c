/*
 * =====================================================================================
 *
 *       Filename:  pomme_pack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/12/2012 07:30:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_pack.h"

int pomme_pack_create( pomme_pack_t **pack,
	void *data,
	size_t size)
{
    u_int32 flags = 0;
    int ret = POMME_SUCCESS;
    if( *pack == NULL )
    {
	*pack = malloc(sizeof(pomme_pack_t));
	if( NULL == *pack )
	{
	    debug("malloc mem for package failure");
	    ret = POMME_MEM_ERROR;
	    goto err;
	}

	flags = 1;
    }
    pomme_pack_t *pBuf = *pack;
    memset(pBuf, 0 , sizeof( pomme_pack_t) );

    SET_PACK_MAGIC(pBuf);
    debug("MAGIC:%d",pBuf->magic);
    if( flags == 1 )
    {
	SET_PACK_NEED_FREE(pBuf);
    }

    pBuf->data = data;
    pBuf->size = size;
    if( data == NULL )
    {
	pBuf->data = malloc(size);
	if( NULL == pBuf->data )
	{
	    debug("malloc mem for package data fail");
	    ret = POMME_MEM_ERROR;
	    goto data_malloc_err;
	}
	memset(pBuf->data, 0 , size );
	SET_PACK_DATA_FREE(pBuf);

	if( pomme_get_endian() == POMME_LITTLE_ENDIAN )
	{
	    SET_ENDIAN_LITTLE(pBuf);
	}else{
	    SET_ENDIAN_BIG(pBuf);
	}
    }
    pBuf->cur = 1;
    return ret;


data_malloc_err:
    if( flags == 1 ) free(pBuf);
err:
    return ret;

}
int pomme_pack_distroy( pomme_pack_t **pack )
{
    int ret = POMME_SUCCESS ;
    pomme_pack_t *pBuf = *pack;
    if( pBuf == NULL )
    {
	debug("trying to distroy an null pointer");
	goto err;
    }

    if( IS_PACK_DATA_FREE(pBuf))
    {
	free(pBuf->data);
    }

    if( IS_PACK_NEED_FREE(pBuf))
    {
	free(pBuf);
    }
    *pack = NULL;
err:
    return ret;
}

int pack_data(void *data , size_t size, pomme_pack_t *pack)
{
    int ret = 0;
    void *tP = NULL;
    assert( pack!=NULL );

    fprintf(stderr,"I hate!");
    debug("SIZE TO PACK:%d %d total_size:%d",pack->cur,size,pack->size);

    if( ! IS_VALID_PACK(pack) )
    {
	debug("Not valid package");
	ret = POMME_INVALID_PACKAGE;
	goto err;
    }

    if( remaining_size(pack) < size )
    {
	debug("remaining size not enough");
	if( pack->size + POMME_PACKAGE_SIZE > POMME_MAX_PACKAGE_SIZE )
	{
	    ret = POMME_TOO_LARGE_PACKAGE;
	    goto err;
	}
	pack->size += POMME_PACKAGE_SIZE;
	debug("Realloc:%d",pack->size);
	tP = realloc(pack->data, pack->size);
	if( NULL == tP )
	{
	    debug("remalloc mem fail");
	    ret = POMME_MEM_ERROR;
	    goto err;
	}
    }

    memcpy( &pack->data[pack->cur] , data , size );
    if(size == 4)
    {
	debug("data put:%d",*(pack->data+pack->cur));
    }
    pack->cur += size;
    debug("CurSize:%d",pack->cur);
err:
    return ret;
}
int unpack_data(void **data, size_t length, pomme_pack_t *pack)
{
    int ret = 0;
    assert( pack != NULL );
    debug("MAGIC:%d total_len:%d",pack->magic,pack->size);
    if( !( IS_VALID_PACK(pack)) )
    {
	debug("Not valid package");
	ret = POMME_INVALID_PACKAGE;
	goto err;
    }
    if( remaining_size(pack) < length )
    {
	debug("data not enough to pack, remainning %d , need %d", remaining_size(pack), length);
	ret = POMME_NOT_ENOUGH_UNPACK;
	goto err;
    }
    if( *data == NULL )
    {
	*data = (char *)pack->data + pack->cur;
    }else{
	memcpy( *data, &pack->data[pack->cur], length );
    }
    if(length == 4)
    {
	debug("data get:%d",*(int *)(pack->data + pack->cur) );
    }
    pack->cur += length;
    debug("CurSize:%d",pack->cur);
err:
    return ret;
}
