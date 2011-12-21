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

#ifdef DEBUG
#define debug(msg,argc...) fprintf(stderr,msg,##argc)
#else
#define debug(msg,argc...) 0
#endif


char *pomme_time(char *buf);
struct tm *pomme_time_all();
#endif
