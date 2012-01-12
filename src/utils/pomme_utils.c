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

