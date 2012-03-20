/*
 * =====================================================================================
 *
 *       Filename:  pomme_threads.h
 *
 *    Description:  the description of the implements of a thread pool
 *
 *        Version:  1.0
 *        Created:  02/19/2012 05:48:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_THREADS_H
#define _POMME_THREADS_H
#include "pomme_queue.h"
#include "utils.h"

typedef struct pomme_worker
{
    void (*process)(void *arg);
    void *arg;
    queue_body_t next;
}pomme_worker_t;

typedef pomme_thread_pool pomme_tpool_t;
struct pomme_thread_pool
{

    int inited; 
    int stop;

    int max_thread_num;
    int cur_thread_num;
    
    sem_t sem;
    /* lock for global varible */
    phread_mutex_t g_lock;
    /*The max waiting length of the queue     */

    int max_waiting;

    pomme_queue_t *workers;
    pomme_queue_t *finished;

    pthread_t *tids;
    int (*add_worker)(pomme_tpool_t *ptp,pomme_worker_t *wr);
    int 

    int (*start)(pomme_tpool_t *ptp);
    int (*stop)(pomme_tpool_t *ptp);

    /* if n > 0 the number of current thread will +n ,else - */
    int ( *extend_pool)(pomme_tpool_t *ptp,int n);

    int (* remove_thread)(pomme_tpool_t *ptp);
    int (* add_thread)(pomme_tpool_t *ptp);

    int (*thread_routine)(void *);
};

typedef struct routine_arg
{
    int rank;
    pthread_t tid;
    pomme_tpool_t *ptp;
}routine_arg;

typedef struct thread_ids
{
    int rank;
    pomme_body_t next;
}thread_ids_t;
/**
 * @brief pomme_tp_init : init an pthread pool
 * @param ptp
 * @return <0 failure, == 0 success 
 */
int pomme_tp_init(pomme_tpool_t *ptp,
	int num);
/**
 * @brief pomme_tp_distroy:distroy an thread pool
 * @param ptp
 * @return < 0 failure, == 0 success 
 */
int pomme_tp_distroy(pomme_tpool_t *ptp);

#endif
