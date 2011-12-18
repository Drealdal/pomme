/*
 * =====================================================================================
 *
 *       Filename:  pomme_buffer.h
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  12/17/2011 11:36:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_BUFFER_H
#define POMME_BUFFER_H
#include "utils.h"

#define POMME_BUFFER_NEED_FREE 1 // The pomme_buffer is inited by the system

#define lock_buffer(buffer) pthread_mutex_lock(&(buffer->mutex))
#define unlock_buffer(buffer) pthread_mutex_unlock(&(buffer->mutex))
#define pos(buffer,offset) ((char *)buffer->buffer + offset)

                                                /* defination of error return */
#define NULL_POINTER_ERROR -1
#define NO_AVAILABLE_BUFFER -2

#define offset_of(buffer,pointer) ((char *)pointer - (char *)buffer->buffer)
typedef struct pomme_buffer
{
	int32 begin;
	int32 end;

	int32 chunk_size;// the buffer is not availale if the avalable buffer is smaller than this
	int32 size;
	u_int32 flags;

	pthread_mutex_t mutex;
	void *buffer;
}pomme_buffer_t;

/*-----------------------------------------------------------------------------
 *  seek to next available position
 *-----------------------------------------------------------------------------*/
int32 pomme_buffer_next(pomme_buffer_t *buffer);

/*-----------------------------------------------------------------------------
 *  claim data to be usable
 *-----------------------------------------------------------------------------*/
int pomme_buffer_release(pomme_buffer_t *buffer,int32 offset);

/*-----------------------------------------------------------------------------
 *  init an pomme_buffer
 *-----------------------------------------------------------------------------*/
int pomme_buffer_init(pomme_buffer_t **pomme_buffer,int32 size);
#endif
