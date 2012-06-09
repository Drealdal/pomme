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
#include "utils.h"
static int stop_log = 0;
static log_t * get_log_from_logger(struct logger *loger);
static log_t * get_log(pomme_log_level_t level,struct logger *logger);
static int add_log(char *message,struct logger *logger);
static void distory_logger(struct logger *logger);
static void* write_log(void *);

int global_log_level = POMME_LOG_WARNING;
pomme_queue_t loggers;
pomme_queue_t log_buffer;
pomme_queue_t free_slots;
pthread_t log_thread;

void POMME_LOG(char *filename,int line,char *message,pomme_log_level_t level,struct logger *logger)
{
	if(level > global_log_level)
	{
		return;
	}
	if( logger == NULL )
	{
		fprintf(stderr,"Null logger");
		return;
	}
	char local_mem[1024];
	char *prefix = NULL;
	switch(level)
	{
		case POMME_LOG_ERROR:
			prefix = "POMME_LOG_ERROR:";
			break;
		case POMME_LOG_WARNING:
			prefix = "POMME_LOG_WARNING:";
			break;
		case POMME_LOG_INFO:
			prefix = "POMME_LOG_INFO:";
			break;
		case POMME_LOG_DEBUG:
			prefix = "POMME_LOG_DEBUG:";
			break;
		default:
			prefix = "POMME_LOG_NULL:";
			break;
	}
	snprintf(local_mem,1024,"%s %s %d %s",prefix,filename,line,message);

	add_log(local_mem,logger);
	return;
}
static int add_log(char *message,struct logger *logger)
{
	if( logger == NULL)
	{
		fprintf(stderr,"POMME_LOG_ERROR:Malloc memeroy for message %s fail\n",message);
		return;
	}
	log_t *tadd = NULL;
	int tried = 0;
	while(tadd == NULL)
	{
		struct queue_body * body = queue_get_front(&free_slots);	
		if(body !=NULL)
		{
			tadd = queue_entry(body,struct log,next);
			break;
		}
		tried++;
		if(tried >= MAXLOGTRY)
		{
			break;
		}	
	}
	if( tadd == NULL )
	{
		tadd = malloc(sizeof(log_t));
	}
	if( tadd == NULL )
	{
		fprintf(stderr,"POMME_LOG_ERROR:Malloc memeroy for message %s fail\n",message);
		return;
	}
	struct tm *time_now = (struct tm*)pomme_time_all(); 
	sprintf(tadd->log_time,"%d-%d-%d %d:%d:%d ",time_now->tm_year+1900,\
					time_now->tm_mon,\
					time_now->tm_mday,\
					time_now->tm_hour,\
					time_now->tm_min,\
					time_now->tm_sec);
	strcpy(tadd->message , message);

	tadd->logger = logger;

	queue_push_back(&log_buffer,&tadd->next);
	return;

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
struct logger *create_logger(pomme_log_level_t level,char *filename)
{
	struct logger *global_logger = malloc(sizeof(struct logger));
	if( global_logger == NULL)
	{
		return NULL;
	}	
	char *dirname = getenv("POMME_LOG_DIR");
	if( dirname == NULL )
	{
		dirname = LOG_DIR_DEFAULT;
	}
	if( filename== NULL )
	{
		debug("%s %d: filename is null\n",__FILE__,__LINE__);
		filename = "pomme_log";
	}
	struct tm * time_now = (struct tm*)pomme_time_all();
	char file[100];
	snprintf(file,100,"%s/%s-%d-%d-%d",dirname,filename,time_now->tm_year+1900,\
			time_now->tm_mon,time_now->tm_mday);

	memset(global_logger,0,sizeof(struct logger));

	global_logger->log_level = level;

	FILE * handle = fopen(file,"a+");
	if( NULL == handle )
	{
		debug("%s:%s open log file failure\n",__FILE__,__LINE__);
#ifndef IGNORE_LOG_FILE_ERROR
		exit(-1);
#else
		global_logger->log_level = POMME_LOG_NULL; 
#endif
	}
	global_logger->file_handle = handle;
	global_logger->name = malloc(strlen(filename)+1);
	global_logger->next.next = QUEUE_TAIL_NULL;
	if(global_logger->name == NULL)
	{
		fprintf(stderr,"Create logger fail\n");
		free(global_logger);
		return NULL;
	}
	memcpy(global_logger->name,filename,strlen(filename));
	queue_push_back(&loggers,&global_logger->next);
	return global_logger;
}
static void distory_logger(struct logger *logger)
{
	debug("Distory logger %s\n",logger->name);
	if(logger == NULL) return;
	fclose(logger->file_handle);
	free(logger->name);
	free(logger);
}

int init_log()
{
	struct queue_head * buffer = &log_buffer;
	struct queue_head * logger = &loggers;
	struct queue_head * slots = &free_slots;
	init_queue(&buffer,"Log Buffer",1000000);
	init_queue(&slots,"Free Log slots",1000000);
	init_queue(&logger,"Loggers",100);
	int err = pthread_create(&log_thread, NULL,&write_log,NULL);
}
int stop_logger()
{
    debug("Trying to stop log.");
    stop_log = 1;
    while(stop_log == 1 )
    {
	sleep(1);
	debug("Waiting log Zzzzz....");
    }
    return 0;
}
static void* write_log(void * argc)
{
	struct queue_head local_logs;  
	struct queue_head *buffer = &log_buffer;
	struct queue_head * logger = &loggers;
	struct queue_head *local =  &local_logs;
	struct queue_head * slots = &free_slots;
	init_queue(&local,"Log thread local",1000);
	int ret;
	struct queue_body * body = NULL;
	while( stop_log == 0 )
	{

		local_logs.head = QUEUE_HEAD_NULL;
		local_logs.tail = QUEUE_TAIL_NULL;
		while(is_empty_queue(buffer))
		{
			sleep(1);
			if(stop_log)break;
		}
		queue_cpy_del(buffer,local);
		while(!is_empty_queue(local))
		{
			body = queue_get_front(local);
			struct log * entry = queue_entry(body,struct log,next);

			fprintf(entry->logger->file_handle,"%s %s\n",entry->log_time,entry->message);

			body->next  = QUEUE_TAIL_NULL;
			queue_push_back(slots,body);
		}

	}
	while(!is_empty_queue(logger))
	{
		body = queue_get_front(logger);
		struct logger * entry = queue_entry(body,struct logger,next);
		distory_logger(entry);
	}
	stop_log=0;
}
