/*
 * =====================================================================================
 *
 *       Filename:  testLog.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2011 12:38:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_log.h"
#include "utils.h"
int main()
{
	POMME_LOG_WARNING("test_log");
	char time[100];
	pomme_time(time);
	printf("%s\n",time);
	
	return 0;
}
