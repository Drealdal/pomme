/*
 * =====================================================================================
 *
 *       Filename:  pomme_log.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2011 10:59:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_log.h"
#include "pomme_queue.h"
#include "pomme_list.h"
#include "utils.h"

static log_t * get_log_from_logger(struct logger *loger);
static log_t * get_log(pomme_log_level_t level);

int global_log_level = POMME_LOG_WARNING;
struct logger global_logger;

void POMME_LOG(char *filename,int line,char *message,pomme_log_level_t level)
{
	if(level > global_log_level)
	{
		return;
	}
	switch(level)
	{
		case POMME_LOG_ERROR:
			break;
		case POMME_LOG_WARNING:
			break;
		case POMME_LOG_INFO:
			break;
		case POMME_LOG_DEBUG:
			break;
		default:
			return;
	}
	
}
static int add_log(pomme_log_level_t level, char *message)
{
	log_t *tadd = NULL;
	int tried = 0;
	if(tadd == NULL)
	{
		tadd=get_log(level);
		tried++;
		if(tried > MAXLOGTRY)
		{
			return -1;
		}	
	}
	gettimeofday(&tadd->log_time,NULL);
	tadd->log_type = level;
	tadd->message = message;
}


static log_t * get_log(pomme_log_level_t level)
{	
	get_log_from_logger(&global_logger);
	switch (level) {
		case POMME_LOG_ERROR:	
			break;
		default:	
			break;
	}
	return NULL;
}
/*-----------------------------------------------------------------------------
 * get a free log_t from logger struct
 * @loger 
 * @return , return a free slots or null if no available free slots  
 *-----------------------------------------------------------------------------*/
static log_t * get_log_from_logger(struct logger *loger)
{
	//TODO 
	return (log_t *)malloc(sizeof(log_t));
}
int start_log(pomme_log_level_t level,char *filename)
{
	char *dirname = getenv("LOG_DIR");
	if( dirname == NULL )
	{
		dirname = LOG_DIR_DEFAULT;
	}
	if( filename== NULL )
	{
#ifdef DEBUG
		printf("%s %d: filename is null\n",__FILE__,__LINE__);
#endif 		
		filename = "pomme_log";
	}
	struct tm * time_now = pomme_time_all();
	char file[100];
	snprintf(file,100,"%s/%s-%d-%d-%d-%d-%d-%d",dirname,filename,time_now->tm_year+1900,\
			time_now->tm_mon,time_now->tm_mday,time_now->tm_hour,\
			time_now->tm_min,time_now->tm_sec);
	memset(&global_logger,0,sizeof(global_logger));
	global_logger.log_level = level;
	FILE * handle = fopen(file,"a+");
	if( NULL == handle )
	{
#ifdef DEBUG
		printf("%s:%s open log file failure\n",__FILE__,__LINE__);
#endif
#ifndef IGNORE_LOG_FILE_ERROR
		exit(-1);
#else
		global_logger.log_level = POMME_LOG_NULL; 
#endif
	}
	global_logger.file_handle = handle;
	

}
