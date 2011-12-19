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
#include "pomme_queue.h"

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
	q_addr->head = QUEUE_HEAD_NULL;
	q_addr->tail = QUEUE_TAIL_NULL;
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
#ifdef DEBUG
		fprintf(stderr,"%s %d: Queue %s init Mutex Error \n",__FILE__,__LINE__,name);
#endif
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
	int ret = 0;
	lock_queue(queue);
	if( !is_empty_queue(queue) )
	{
#ifndef DEBUG
		printf("Trying to distory an non empty queue %s\n",queue->name);
#endif
		ret = -1;
		goto d_end;
	}
	free(queue->name);
	if( queue->need_free == 1 )
	{
		unlock_queue(queue);
		free(queue);
		return 0;
	}
	queue->tail = QUEUE_TAIL_NULL;
	queue->head = QUEUE_HEAD_NULL;
	queue->maxLength = 0;
d_end:
	unlock_queue(queue);
	return ret;

}

static int __add_queue(struct queue_head *queue,struct queue_body *toadd)
{
	int ret = 0;
	lock_queue(queue);
	if(is_full_queue(queue))
	{
#ifdef DEBUG
		printf("Queue:%s is Full\n",queue->name);
#endif
		ret = -1;
		goto unl;
	}
	toadd->next = QUEUE_TAIL_NULL;
	if( is_tail_null(queue) )
	{
		queue->tail = toadd;
		queue->head = toadd;
		queue->curLength = 1;
		ret = 0;
		goto unl;
	}
	queue->tail->next = toadd;
	queue->tail = toadd;
	queue->curLength++;
unl:
	unlock_queue(queue);

	return ret;
}
static struct queue_body * __del_queue(struct queue_head *queue)
{
	struct queue_body * re = NULL;
	lock_queue(queue);
	if( is_empty_queue(queue) )
	{
#ifdef DEBUG
		printf("Queue:%s is empty\n",queue->name);
#endif
		goto out_pos;
	}
	re = queue->head;

	if( queue->head == NULL )
	{
		printf("%d\n",queue->curLength);
		printf("Shit!\n");
	}
	queue->head = queue->head->next;
	if(queue->head == QUEUE_TAIL_NULL)
	{
		queue->tail = QUEUE_TAIL_NULL;
		queue->head = QUEUE_HEAD_NULL;
	}
	queue->curLength--;
out_pos:
	unlock_queue(queue);
	return re;
}
int queue_push_back(struct queue_head *queue,struct queue_body *toadd)
{
	return	__add_queue(queue,toadd);
}
struct  queue_body * queue_get_front(struct queue_head *queue)
{
	return __del_queue(queue);
}
int queue_cpy_del(struct queue_head *from, struct queue_head *to)
{
	lock_queue(from);
	lock_queue(to);
	to->head = from->head;
	to->tail = from->tail;
	from->head = QUEUE_HEAD_NULL;
	from->tail = QUEUE_TAIL_NULL;
	to->curLength = from->curLength;
	from->curLength = 0;
	unlock_queue(to);
	unlock_queue(from);
	return 0;
}
