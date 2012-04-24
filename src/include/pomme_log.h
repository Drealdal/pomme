/*
 * =====================================================================================
 *
 *       Filename:  pomme_log.h
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  11/08/2011 10:47:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_LOG_H
#define POMME_LOG_H
#include "utils.h"
#include "pomme_queue.h"
#define LOG_DIR_DEFAULT "./"

#ifndef MAXLOGTRY
#define MAXLOGTRY 3
#endif

#define MAX_LOG_LENGTH 1024

typedef enum LOG_LEVEL{
	POMME_LOG_NULL,
	POMME_LOG_ERROR,
	POMME_LOG_WARNING,
	POMME_LOG_INFO,
	POMME_LOG_DEBUG,
	POMME_LOG_MAX
}pomme_log_level_t;

typedef struct log {
	char log_time[24];
        char message[1024];
	queue_body_t next;
	struct logger *logger;
}log_t;

typedef struct logger{
	char *name;
        FILE *file_handle;	
	pomme_log_level_t log_level;// level < log_level will be logged
	struct queue_body next;//The logger should put into a queue ,to manage 

}logger_t;
void POMME_LOG(char *filename,int line,char *message,pomme_log_level_t level,struct logger *logger);
/*
 * register a logger
 */
struct logger * create_logger(pomme_log_level_t level, char *name);
int init_log();
int stop_logger();

#define POMME_LOG_ERROR(message,logger) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_ERROR,logger)
#define POMME_LOG_WARNING(message,logger) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_WARNING,logger)
#define POMME_LOG_INFO(message,logger) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_INFO,logger)
#define POMME_LOG_DEBUG(message,logger) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_DEBUG,logger)


#endif
