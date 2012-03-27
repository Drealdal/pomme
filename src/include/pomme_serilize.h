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
#include "utils.h"

int read_data(pomme_data_t * wt, int handle);
int write_data(pomme_data_t * wt, int handle);

#endif
