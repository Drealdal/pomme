/*
 * =====================================================================================
 *
 *       Filename:  pomme_utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2011 03:55:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"
char *pomme_time(char *buf)
{
	time_t t = time(NULL);
	char *re = NULL;
	if( buf != NULL )
	{
		re = ctime_r(&t,buf);	
	}else{
		re = ctime(&t);
	}
	return re;
}	

