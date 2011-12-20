/*
 * =====================================================================================
 *
 *       Filename:  test_buffer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/18/2011 06:43:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_buffer.h"
#include "pomme_queue.h"
#include "utils.h"
typedef struct data
{
	int data_begin;
	int length;
	struct queue_body next;
}data_t;
typedef struct data_queue
{
	pomme_buffer_t *buffer;
	struct queue_head *head;
}data_queue_t;
#define BUFFER_SIZE (1024*1024*1024)
#define	chunck_size (32*1024)
static void * privoder(void *);
static void * consumer(void *);
data_queue_t datas;
pthread_t p_thread, c_thread;
int main()
{
	int ret = 0;
	datas.buffer = NULL;
	datas.head = NULL;
	if( 0 > pomme_buffer_init(&datas.buffer,BUFFER_SIZE,chunck_size) )
	{
		printf("Buffer init Error\n");
		return -1;
	}
	init_queue(&datas.head,"Buffer data queue",10000000);
	ret = pthread_create(&p_thread, NULL, &privoder,&datas);
	if(ret < 0 )
	{
		fprintf(stderr,"Create privoder thread error\n");
		exit(-11);
	}
	ret = pthread_create(&c_thread, NULL, &consumer,&datas);
	if(ret < 0 )
	{
		fprintf(stderr,"Create consumer thread error\n");
		exit(-11);
	}
	sleep(100000);
	
	return ret;
}
static void *privoder(void * argc)
{
	data_queue_t *datas = (data_queue_t *)argc;
	srand(time(NULL));
	while(1)
	{
		int a = rand()%chunck_size;
		data_t *tm = malloc(sizeof(struct data));
		do{
			tm->data_begin = pomme_buffer_next(datas->buffer,datas->buffer->chunk_size);
		}while( tm->data_begin < 0 );
		tm->length = a;
		pomme_buffer_take(datas->buffer,tm->length);
		queue_push_back(datas->head,&tm->next);
	}	
	return NULL;
}
static void *consumer(void *argc)
{
	data_queue_t *datas = (data_queue_t *)argc;
	while(1)
	{
		queue_body_t *pos = queue_get_front(datas->head);
		if( pos ==NULL )
		{
			/*
			 * here we do wait
			 */
			sleep(1);
			continue;
		}
		data_t *entry = queue_entry(pos,data_t,next);
		int l = entry->length;
		pomme_buffer_release(datas->buffer,entry->data_begin,l);
		free(entry);
	}
	return NULL;
}
