/*
 * =====================================================================================
 *
 *       Filename:  pomme_utils.h
 *
 *    Description:  The declaration of serveral use of pomme system 
 *
 *        Version:  1.0
 *        Created:  11/15/2011 03:56:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */

#ifndef POMME_UTILS_H
#define POMME_UTILS_H
#include "utils.h"

#define FFL __FILE__,__func__,__LINE__

#ifdef DEBUG
#define debug(msg,argc...) fprintf(stderr,"%s %s %d: "msg"\n",FFL,##argc)
#else
#define debug(msg,argc...) 0
#endif


int pomme_data_init(pomme_data_t *data, u_int32 size);
int pomme_data_distroy(pomme_data_t *data);
char *pomme_time(char *buf);
struct tm *pomme_time_all();
/**
 * @brief pomme_get_endian : test the endian of the server
 */
int pomme_get_endian();


#endif
