/*
 * =====================================================================================
 *
 *       Filename:  pomme_buffer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/18/2011 02:30:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_buffer.h"

/*-----------------------------------------------------------------------------
 *  init a buffer, the size is set to size
 *-----------------------------------------------------------------------------*/
int pomme_buffer_init(pomme_buffer_t **buffer,int32 size)
{
	int ret = 0;
	u_int32 flags = 0;
	if( *buffer == NULL )
	{
		*buffer = malloc(sizeof(pomme_buffer_t));
		if( NULL == *buffer )
		{
#ifdef DEBUG
			fprintf(stderr,"Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
			ret = -1;
			goto __exit;	
		}
		flags |= POMME_BUFFER_NEED_FREE;
	}
	pomme_buffer_t *p_buffer = *buffer;
	p_buffer->size = size;
	p_buffer->begin = 0;
	p_buffer->end = 0;
	if( pthread_mutex_init(&p_buffer->mutex,NULL) != 0 )
	{
#ifdef DEBUG
		fprintf(stderr,"Mutex init Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return -1;
	}
	p_buffer->buffer = malloc(size);
	if( p_buffer->buffer == NULL )
	{
#ifdef DEBUG
			fprintf(stderr,"Malloc Error@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
			ret = -1;
			goto buffer_error;

	}
	p_buffer->flags = flags;
	return 0;
buffer_error:
	if( (flags&POMME_BUFFER_NEED_FREE) != 0 )
	{
		free(p_buffer);
		*buffer = NULL;
	}
__exit:
	return ret;
}

/*-----------------------------------------------------------------------------
 *  distroy an buffer
 *-----------------------------------------------------------------------------*/
int pomme_buffer_distroy(pomme_buffer_t **buffer)
{
	if( *buffer == NULL )
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to free an Null buffer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return 0;
	}
	pomme_buffer_t * p_buffer = *buffer;
	lock_buffer(p_buffer);
	free(p_buffer->buffer);
	unlock_buffer(p_buffer);
	if( (p_buffer->flags & POMME_BUFFER_NEED_FREE) != 0 )
	{
		free(p_buffer);
		*buffer = NULL;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
 *  get the offset of next position
 *-----------------------------------------------------------------------------*/
int32 pomme_buffer_next(pomme_buffer_t *buffer)
{
	int ret = 0;
	if( buffer == NULL )
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to do operation on null pointer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return NULL_POINTER_ERROR;
	}
	lock_buffer(buffer);
	int32 begin = buffer->begin;
	int32 end = buffer->end;
	int32 size = buffer->size;

	if( begin <= end )
	{
		int32 tm = size - end;
		if( tm >= buffer->chunk_size )
		{
			ret = end;
			goto out;
		
		}
		if( begin >= buffer->chunk_size )
		{
			buffer->end = 0;
			ret = 0;
#ifdef 	DEBUG
//			fprintf(stderr,"Not enough at the end , rewind to the beginning\n");
#endif
			goto out;
		}
               /* This means there are no available buffer */
#ifdef DEBUG
		fprintf(stderr,"Not enough mem\n");
#endif
		ret = NO_AVAILABLE_BUFFER;
	}else{
		int32 tm = begin - end;
		if( tm >= buffer->chunk_size )
		{
			ret = end;
		}else{
			ret = NO_AVAILABLE_BUFFER;
#ifdef DEBUG
		fprintf(stderr,"Not enough mem\n");
#endif
		}
	}
out:
	unlock_buffer(buffer);
	return ret;
}

/*-----------------------------------------------------------------------------
 *  release buffer to be availbale
 *-----------------------------------------------------------------------------*/
int pomme_buffer_release(pomme_buffer_t *buffer,int32 begin,int32 offset)
{	if( buffer == NULL )
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to do operation on null pointer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return NULL_POINTER_ERROR;
	}
	lock_buffer(buffer);
	buffer->begin = begin + offset;
	unlock_buffer(buffer);
	return 0;
}

/*-----------------------------------------------------------------------------
 *  set part of the buffer as occupied
 *-----------------------------------------------------------------------------*/
int pomme_buffer_take(pomme_buffer_t *buffer,int offset)
{
	
	if( buffer == NULL )
	{
#ifdef DEBUG
		fprintf(stderr,"Trying to do operation on null pointer@%s %s %d\n",__FILE__,__func__,__LINE__);
#endif
		return NULL_POINTER_ERROR;
	}
	lock_buffer(buffer);
	buffer->end += offset;	
	unlock_buffer(buffer);
	return 0;
	
}
