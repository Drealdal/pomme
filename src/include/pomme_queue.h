/*
 * =====================================================================================
 *
 *       Filename:  pomme_queque.h
 *
 *    Description:  the implement of queue 
 *
 *        Version:  1.0
 *        Created:  11/15/2011 05:26:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_QUEUE_H
#define POMME_QUEUE_H
#include "utils.h"

#define QUEUE_HEAD_NULL ((void *)0x00100100)
#define QUEUE_TAIL_NULL ((void *)0x00100200)

#define is_head_null(queue) queue->head == QUEUE_HEAD_NULL
#define is_tail_null(queue) queue->tail == QUEUE_TAIL_NULL
typedef struct queue_head
{
	struct queue_body *head;
	struct queue_body *tail;
	/*
	 * matrix
	 */
	int maxLength;// the max allowed queue length, this is usefull when we wantto limit the length of waitting client
	int curLength;// the current queue length
	char *name;
	pthread_mutex_t mutex;
	/*
	 * utils
	 */
	int need_free;// whether the struct is malloc by self
}pomme_queue_t;
struct queue_body{
	struct queue_body *next;
}queue_body_t;
#define is_full_queue(queue) (queue->curLength >= queue->maxLength)
#define is_empty_queue(queue) (queue->curLength == 0)
#define lock_queue(queue) pthread_mutex_lock(&queue->mutex);
#define unlock_queue(queue) pthread_mutex_unlock(&queue->mutex);
/*
 * @queue , address of queue pointer , if the queue is null ,The system will malloc for it.
 * @name  , will be copied to make sure that the name will not be freed util the queue is free
 */
int init_queue(struct queue_head **queue,char *name,int maxLength)
{
	struct queue_head * q_addr = *queue;
	if(q_addr == NULL)
	{
		q_addr = malloc(sizeof(struct queue_head));
		if(q_addr == NULL)
		{
			printf("%s %d:Queue %s init Malloc Mem Error\n",__FILE__,__LINE__,name);
			return -1;
		}
		memset(q_addr,0,sizeof(struct queue_head));
		q_addr->need_free = 1;
	}else{
		memset(q_addr,0,sizeof(struct queue_head));
	}
	q_addr->head = NULL;
	q_addr->tail = NULL;
	q_addr->maxLength = maxLength;
	int nLen = strlen(name);
	q_addr->name = malloc(nLen);
	if( q_addr->name == NULL )
	{
		printf("%s %d: Queue %s init Malloc Mem(for name) Error \n",__FILE__,__LINE__,name);
		return -1;
	}
	memcpy(q_addr->name,name,nLen);
	if( pthread_mutex_init(&q_addr->mutex,NULL) != 0 )
	{
		printf("%s %d: Queue %s init Mutex Error \n",__FILE__,__LINE__,name);
		return -1;
	}
	q_addr->curLength = 0;
	*queue = q_addr;
	return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  destory_queue
 *  Description:  destory an empty queue, if the queue is not empty will 
 *  		  incur error
 * =====================================================================================
 */
int destory_queue(struct queue_head *queue)
{
	if( is_empty_queue(queue) )
	{
		return -1;
	}
	free(queue->name);
	if( queue->need_free == 1 )
	{
		free(queue);
		return 0;
	}
	queue->tail = QUEUE_TAIL_NULL;
	queue->head = QUEUE_HEAD_NULL;
	queue->maxLength = 0;
	return 0;

}

static int __add_queue(struct queue_head *queue,struct queue_head *toadd)
{
	lock_queue(queue);
	if(is_full_queue(queue))
	{
#ifdef DEBUG
		printf("Queue:%s is Full\n",name);
#endif
		unlock_queue(queue);
		return -1;
	}
	if(is_tail_null(queue))
	{
		queue->tail = toadd;
		queue->head = toadd;
		queue->curLength = 1;
		unlock_queue(queue);
		return 0;
	}
	queue->tail->next = toadd;
	queue->tail = toadd;
	unlock_queue(queue);
	return 0;
}
static struct queue_body * __del_queue(struct queue *queue)
{
	struct queue_body * re = NULL;
	lock_queue(queue);
	if( is_empty_queue(queue) )
	{
		goto out_pos;
	}
	re = queue->head;
	queue->head = queue->head->next;
	if(queue->head = QUEUE_TAIL_NULL)
	{
		queue->tail = QUEUE_TAIL_NULL;
		queue->head = QUEUE_HEAD_NULL;
	}
	queue_curLength--;
out_pos:
	unlock_queue(queue);
	return re;
}

#endif
