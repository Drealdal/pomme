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

typedef unsigned char u_int8;
typedef char int8;

typedef unsigned short u_int16;
typedef short int16;

typedef int int32;
typedef unsigned int u_int32;

typedef long long int64;
typedef unsigned long long u_int64;

#define POMME_DATA_NEED_FREE 1  // The pomme_data_t->data is alloc by the system, need free
typedef struct pomme_data{
	u_int32 size;
	u_int32 flags;//the operation for the flag, BDB likely interface 
	void *data;
}pomme_data_t;

#endif
