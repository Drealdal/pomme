/*
 * =====================================================================================
 *
 *       Filename:  pomme_type.h
 *
 *    Description: The type defination of pommes 
 *
 *        Version:  1.0
 *        Created:  12/12/2011 06:26:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_TYPE_H
#define POMME_TYPE_H
#include <sys/time.h>

typedef unsigned char u_int8;
typedef char int8;

typedef unsigned short u_int16;
typedef short int16;

typedef int int32;
typedef unsigned int u_int32;

typedef long long int64;
typedef unsigned long long u_int64;


typedef struct pomme_data{
	int size;
	u_int32 flags;//the operation for the flag, BDB likely interface 
	void *data;
}pomme_data_t;

typedef struct heart_beat
{
    /*  basic info */
    u_int32 myid;
    u_int32 mygroup;
    u_int32 ip;
    u_int16 port;
    /*  statistic info */
    u_int64 objectCount;
    u_int64 requestCount;
}pomme_hb_t;

typedef struct pomme_lock
{
    time_t wt;
    int interval;
}pomme_lock_t;
#endif
