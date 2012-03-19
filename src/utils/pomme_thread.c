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
#include "utils.h"

static int start(pomme_tpool_t *ptp); 
static int add_worker(pomme_tpool_t *, pomme_worker_t *wr);

static int shutdown(pomme_tpool_t *);
static int stop(pomme_tpool_t *);

static int thread_routine(pomme_worker_t *worker);

int pomme_tp_init(pomme_tpool_t *ptp,int num,
	int w_num)
{
    int ret = 0;

    assert( ptp != NULL);
    memset( ptp, 0, sizeof(pomme_tpool_t));

    ptp->max_thread_num = num;
    ptp->max_waiting = w_num;
    ptp->tids = malloc(sizeof(pthread_t)*num);

    if(ptp->tids == NULL)
    {
	debug("malloc err");
	ret = POMME_MEM_ERROR;
	goto err;
    }

   if ( ( ret = queue_init( &ptp->workers,
	    "pomme thread pool",w_num)) < 0 )
   {
       debug("queue init failure");
       goto queue_err;
   }

   ptp->add_worker = add_worker;
   ptp->start = start;

   ptp->stop = stop;
   ptp->shutdown = shutdown;
   ptp->thread_routine = thread_routine;

   ptp->inited = 1;

   return ret;
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
    for( i = 0; i < ptp->max_thread_num; i++ )
    {
        if( ( ret = pthread_create(tids+i, NULL,
		       	ptp->routine, NULL )) < 0 )
	{
	    debug("create thread %d fail %s",ret, strerror(ret));
	    goto ret;
	}
    }	
err:
    return ret;
}

static int add_worker(pomme_tpool_t *ptp, pomme_worker_t *wr)
{

    int ret = 0;
    assert( wr != NULL );


    return ret;
}


int pomme_tp_distroy(pomme_tpool_t *ptp)
{
    int ret = 0;

    assert( ptp != NULL );

    return ret;

}
