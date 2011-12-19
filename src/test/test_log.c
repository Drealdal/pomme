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
extern int stop_log;
int main()
{
	init_log();
  	struct logger *	logger = create_logger(POMME_LOG_MAX,"test_logger");
	struct logger * eror =  create_logger(POMME_LOG_MAX,"eror_logger");
	int i = 0;
	for(i = 0 ;i<1000;i++)
	{
		char message[100];
		memset(message,0,sizeof(message));
		sprintf(message,"%d",i);
		POMME_LOG_WARNING(message,logger);
		POMME_LOG_ERROR(message,eror);
	}
	stop_log = 1;
	while(stop_log == 1)
	{
		printf("%d\n",i++);
		sleep(1);
	}
	return 0;
}
