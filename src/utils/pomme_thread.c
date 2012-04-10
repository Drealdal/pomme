/*
 * =====================================================================================
 *
 *       Filename:  pomme_thread.c
 *
 *    Description:  the implement of pomme_thread_pool
 *
 *        Version:  1.0
 *        Created:  03/19/2012 12:35:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_thread.h"
#include "pomme_queue.h"
#include "utils.h"

static int start(pomme_tpool_t *ptp); 
static int stop(pomme_tpool_t *);
static int add_worker(pomme_tpool_t *, 
	pomme_worker_t *wr);
static void * thread_routine(void *worker);

static int add_thread(pomme_tpool_t *ptp);
static int remove_thread(pomme_tpool_t *ptp);
static int extend_pool(pomme_tpool_t *ptp, int n);

int pomme_tp_init(pomme_tpool_t *ptp,int num,
	int w_num,int cur_num)
{
    debug("ptp:%p",ptp);
    int ret = 0;

    assert( ptp != NULL);
    memset( ptp, 0, sizeof(pomme_tpool_t));

    ptp->max_thread_num = num;
    ptp->max_waiting = w_num;
    ptp->cur_thread_num = cur_num;
    ptp->tids = malloc(sizeof(pthread_t)*num);

    if(ptp->tids == NULL)
    {
	debug("malloc err");
	ret = POMME_MEM_ERROR;
	goto err;
    }

   if ( ( ret = init_queue( &ptp->workers,
	    "pomme thread pool",w_num)) < 0 )
   {
       debug("queue init failure");
       goto queue_err;
   }
   debug("worker:%p",ptp->workers);

   if( ( ret = init_queue( &ptp->finished,
		   "pomme thread finish", num)) < 0 )
   {
       debug("queue init failure");
       goto finish_err;
   }
		   

   ptp->add_worker = &add_worker;
   ptp->start = &start;

   ptp->stop = &stop;
   ptp->add_thread = &add_thread;

   ptp->remove_thread = &remove_thread;
   ptp->extend_pool = &extend_pool;

   ptp->thread_routine = &thread_routine;
   if( (ret =  sem_init(&ptp->sem,0,0) ) < 0)
   {
       debug("sem init fail:%s",strerror(ret));
       goto sem_err;
   }
   ptp->inited = 1;

   return ret;
sem_err:
   destroy_queue(ptp->finished);
finish_err:
   destroy_queue(ptp->workers);
queue_err:
  free(ptp->tids); 
err:
    return ret;
}


static int start(pomme_tpool_t *ptp)
{
    if( ptp->inited != 1 )
    {
	debug("Not init thread pool");
	return POMME_THREAD_POOL_NOTINIT; 
    }
    int ret = 0;
    int i;
    for( i = 0; i < ptp->cur_thread_num; i++ )
    {
	routine_arg *arg = malloc(sizeof(routine_arg));
	arg->rank = i;
	arg->ptp = ptp;
        if( ( ret = pthread_create(ptp->tids+i, NULL,
		       	ptp->thread_routine, arg )) < 0 )
	{
	    debug("create thread %d fail %s",ret, strerror(ret));
	    goto err;
	}
    }	
    for(; i < ptp->max_thread_num;i++)
    {
	thread_ids_t *ids = malloc(sizeof(thread_ids_t));	
	memset(ids, 0, sizeof(thread_ids_t));
	ids->rank = i;
	queue_push_back(ptp->finished,&ids->next); 
    }
err:
    return ret;
}
int pomme_tp_distroy(pomme_tpool_t *ptp)
{
    int ret = 0;
    assert( ptp != NULL );
    if( ptp->stoped != 1)
    {
	ptp->stop(ptp);
    }

    destroy_queue(ptp->workers);
    destroy_queue(ptp->finished);

    free(ptp->tids);

    ret = sem_destroy(&ptp->sem);

    memset(ptp, 0, sizeof(ptp));

    return ret;
}

static int add_worker(pomme_tpool_t *ptp, pomme_worker_t *wr)
{

    int ret = 0;

    assert( wr != NULL );
    memset(&wr->next, 0, sizeof(queue_body_t));

    if( (ret = queue_push_back( 
		    ptp->workers, &wr->next) ) < 0 )
    {
	ret = POMME_THREAD_POOL_FULL;
	goto err;
    }
    sem_post(&ptp->sem);
err:
    return ret;
}

static void * thread_routine(void *arg)
{
    routine_arg *info = arg; 
    info->tid = pthread_self();

    pomme_tpool_t *ptp = info->ptp;
    debug("thread %d @ %d",(int)info->tid,info->rank);
    debug("this code,%p",ptp);
    while(1)
    {
	sem_wait(&ptp->sem);	

	queue_body_t *wr = queue_get_front(ptp->workers);
	debug("%p",wr);
	pomme_worker_t *w = queue_entry(wr,pomme_worker_t,next);
	debug("%p",w);
	/* The sem make this assert true */
	assert( w!=NULL );
	if(w->process == NULL )
	{
	    /* exit from the thread pool */
	    ptp->tids[info->rank] = 0;
	    thread_ids_t *ids = malloc(sizeof(thread_ids_t));

	    assert( ids != NULL );

	    memset(ids, 0, sizeof(thread_ids_t));
	    ids->rank = info->rank;
	    queue_push_back(ptp->finished,&ids->next); 

	    free(w);

	    break;
	}
	w->process(w->arg);	

	free(w->arg);
	free(w);
    }
    free(info);
    //TODO  use lock?
    return NULL; 
}
static int remove_thread(pomme_tpool_t *ptp)
{
    int ret = 0;
    pomme_worker_t *wr = malloc(sizeof(pomme_worker_t));
    assert( wr != NULL );

    memset( wr, 0, sizeof(pomme_worker_t));
    ptp->add_worker(ptp, wr);
    /* only the one thread will do these things */
    ptp->cur_thread_num--;

    return ret;
}

static int add_thread(pomme_tpool_t *ptp)
{
    int ret = 0;
    if( ptp->cur_thread_num >= ptp->max_thread_num )
    {
	debug("exceed max");
	return POMME_TOO_MANY_THREAD; 
    }
    queue_body_t *id = queue_get_front( ptp->finished );
    thread_ids_t *ids = queue_entry(id, thread_ids_t, next);
    assert( ids != NULL );
    routine_arg arg;
    arg.rank = ids->rank;
    if( ( ret = pthread_create(ptp->tids+ids->rank, NULL,
		    ptp->thread_routine, &arg )) < 0 )
    {
	debug("create thread %d fail %s",ret, strerror(ret));
    }else{
    	ptp->cur_thread_num++;
    }
    free(ids);
    return ret;
}
static int extend_pool(pomme_tpool_t *ptp, int n)
{
    int ret = 0,i;
    int num = n > 0 ? n : 0-n;
    for( i = 0 ; i < num ; i++ )
    {
	if(n>0)
	{
	    ret = ptp->add_thread(ptp);
	}else{
	    ret = ptp->remove_thread(ptp);
	}
	if( ret < 0 )
	{
	    debug("%s %d thread fail",
		    n>0?"add":"remove",i);
	    break;
	}
    }
    return ret;
}

static int stop(pomme_tpool_t *ptp)
{
   int ret = 0,i; 
   if(( ret = extend_pool(ptp, 
		   0-ptp->cur_thread_num)) < 0 )
   {
       debug("fail");
   }
   for( i = 0; i < ptp->max_thread_num ; i++ )
   {
       if( ptp->tids[i] != 0 )
       {
	   pthread_join(ptp->tids[i],NULL);
       }
   } 
   ptp->stoped = 1;
 return 0; 
}
