/*
 * =====================================================================================
 *
 *       Filename:  pomme_utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2011 03:55:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"



/*-----------------------------------------------------------------------------
 *  init pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_init(pomme_data_t *data, u_int32 size)
{
    int ret = POMME_SUCCESS;
    if( data == NULL )
    {
	debug("init an null pointer");
	ret = POMME_MEM_ERROR;
	goto err;
    }
    if(data->data != NULL && (data->flags & POMME_DATA_NEED_FREE) != 0 )
    {
	if(data->size >= size)
	{
	    memset(data->data,0,data->size);
	    goto err;
	}else{
	    free(data->data);
	}
    }
    data->size = size;
    data->data = malloc(size);
    if(data->data == NULL)
    {
	debug("malloc data for data init failure");
	ret = POMME_MEM_ERROR;
	goto err;
    }
    memset(data->data,0,size);
    data->flags |= POMME_DATA_NEED_FREE; 
err:
    return ret;
}

/*-----------------------------------------------------------------------------
 *  distroy a pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_distroy(pomme_data_t *data)
{
    if(data == NULL)
    {
	debug("distroy null pointer");
	return 0;
    }
    if( (data->flags & POMME_DATA_NEED_FREE) != 0 && data->data != NULL)
    {
	free(data->data);
    }
    return 0;
}


char *pomme_time(char *buf)
{
    time_t t = time(NULL);
    char *re = NULL;
    if( buf != NULL )
    {
	re = ctime_r(&t,buf);	
    }else{
	re = ctime(&t);
    }
    return re;
}	
struct tm* pomme_time_all()
{
    time_t t = time(NULL);
    return localtime(&t);
}

int pomme_get_endian()
{
    int data = 1;
    char *pdata = (void *)&data;
    if( *pdata == 0x01 )
    {
	return POMME_LITTLE_ENDIAN;
    }
    return POMME_BIG_ENDIAN;

}
