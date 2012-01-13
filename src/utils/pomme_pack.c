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
	*pack = malloc(sizeof(pomme_package_t));
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

data_malloc_err:
    if( flags == 1 ) free(pBuf);
err:
    return ret;

}

int pack_data(void *data , size_t size, pomme_package_t *pack)
{
    int ret = 0;
    return ret;
}
int unpack_data(void **data, size_t *length, pomme_package_t *pack)
{
    int ret = 0;
    return ret;
}
