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
#define debug(msg,argc...) fprintf(stdout,"%s %s %d: "msg"\n",FFL,##argc)
#else
#define debug(msg,argc...) 0
#endif

#define warning(msg,argc...) fprintf(stdout,"WARNING:%s %s %d: "msg"\n",FFL,##argc)
#define error(msg,argc...) fprintf(stderr,"ERROR:%s %s %d: "msg"\n",FFL,##argc)


int pomme_data_init(pomme_data_t **data, int size);
int pomme_data_distroy(pomme_data_t **data);
int pomme_strlen(const char *path);
char *pomme_time(time_t t, char *buf);
struct tm *pomme_time_all();


/**
 * @brief pomme_init_timmer set a timer
 *
 * @param msec: 1s = 100000ms
 *
 * @return null 
 */
int pomme_init_timmer(int msec);

/**
 * @brief create_server : create an server at localhost
 *
 * @param port: the port to listen
 * @param max_pending: the max pending client of the socket
 * @param max_clients: the max events of epoll
 * @param events: where the events used by the epoll , size == max_client+1
 * @param sockid: the sockid of the listen handle
 * @param epid: the epid of the epoll
 *
 * @return: < 0 for failure , ==0 sucess 
 */
int create_server(short port,
	int max_pending,
	int max_clients,
 	struct epoll_event *events,	
	int  *sockid,
	int  * epid);

/**
 * @brief create_client: connect to an server , and get an client handle 
 * @param ip: the ip of the server
 * @param port: the port of the server
 * @param sock_fd: the client handle created
 * @return: < 0 failure, ==0 for success 
 */
int create_client(u_int32 ip,
	short port,
	int *sock_fd);

int setnonblocking(int sock);
/**
 * @brief pomme_get_endian : test the endian of the server
 */
int pomme_get_endian();
int print_uper(FILE *fd,char *str);


#endif
