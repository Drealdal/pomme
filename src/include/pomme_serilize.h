/*
 * =====================================================================================
 *
 *       Filename:  pomme_serilize.h
 *
 *    Description:  serilize
 *
 *        Version:  1.0
 *        Created:  03/20/2012 08:11:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_SERILIZE_H
#define _POMME_SERILIZE_H


typedef struct writable
{
    int len;
    void *data;// the data 
}writable;

int read_writable(writable * wt, int handle);
int write_writable(writable * wt, int handle);

#endif
