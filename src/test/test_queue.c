/*
 * =====================================================================================
 *
 *       Filename:  test_queue.c
 *
 *    Description:  test the implements of queue 
 *
 *        Version:  1.0
 *        Created:  11/20/2011 12:12:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_queue.h"
#include "utils.h"

typedef struct test_queue_body
{
	int num;
	struct queue_body queue;

}test_queue_body_t;

int main(int argc, char *argv[])
{
	struct queue_head *my_queue = NULL;
	init_queue(&my_queue,"Test Queue",10);
	int i;
	for( i =  0 ; i< 20;i++)
	{
		printf("%d\n",i);
		test_queue_body_t *tm = malloc(sizeof(test_queue_body_t));
		tm->num = i;
		if( tm == NULL )
		{
			printf("Malloc Error\n");
			return -1;
		}
		queue_push_back(my_queue,&tm->queue);
	}
	struct queue_body *tmp = NULL;
	for( i = 0 ; i < 20 ; i++)
	{
		tmp = queue_get_front(my_queue);	
		if( NULL != tmp )
		{
			test_queue_body_t *t = queue_entry(tmp,struct test_queue_body,queue);
			printf("%d\n",t->num);
			free(t);
			
		}else{
			continue;
		}

	}

	return 0;
}
