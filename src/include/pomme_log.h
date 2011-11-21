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
#include "pomme_list.h"
#define LOG_DIR_DEFAULT "./log/"

#ifndef MAXLOGTRY
#define MAXLOGTRY 3
#endif

typedef enum LOG_LEVEL{
	POMME_LOG_NULL,
	POMME_LOG_ERROR,
	POMME_LOG_WARNING,
	POMME_LOG_INFO,
	POMME_LOG_DEBUG,
	POMME_LOG_MAX
}pomme_log_level_t;

typedef struct log {
	struct timeval log_time; 
        char *message;
	pomme_log_level_t log_type;
}log_t;
typedef struct logger{
	pomme_log_level_t log_level;// level < log_level will be logged
        FILE *file_handle;	
}logger_t;
void POMME_LOG(char *filename,int line,char *message,pomme_log_level_t level);
int start_log(pomme_log_level_t level, char *filename);

#define POMME_LOG_ERROR(message) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_ERROR)
#define POMME_LOG_WARNING(message) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_WARNING)
#define POMME_LOG_INFO(message) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_INFO)
#define POMME_LOG_DEBUG(message) POMME_LOG(__FILE__,__LINE__,message,POMME_LOG_DEBUG)


#endif
