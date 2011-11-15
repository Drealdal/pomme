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

#define QUEUE_HEAD_NULL ((void *)0x00100100)
#define QUEUE_TAIL_NULL ((void *)0x00100200)
typedef struct queue_head
{
	struct queue_head *head;
	struct queue_head *tail;
}pomme_queue_t;

#endif
