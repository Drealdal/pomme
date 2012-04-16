/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpc_server.h
 *
 *    Description:  the implement of pomme_rpc
 *
 *        Version:  1.0
 *        Created:  03/18/2012 08:15:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_RPC_SERVER_H
#define _POMME_RPC_SERVER_H
#include "pomme_blist.h"
#include "pomme_rpcs.h"
#include "pomme_serilize.h"
#include "pomme_thread.h"
#include "utils.h"
/* thre argument of an function */
typedef struct pomme_arg
{
    int n;// argument num
    pomme_data_t *args;
}pomme_arg_t;


#define POMME_ARG(arg,n) \
    pomme_data_t *arg = NULL;\
do{\
    arg=malloc(n*sizeof(pomme_data_t));\
    memset(arg, 0, sizeof(pomme_arg_t)*n);\
}while(0)

#define POMME_ARG_I(arg,i,type) \
    do{\
	arg[i].size = sizeof(type);\
    }while(0);

typedef struct pomme_func
{
    char *name;
    pomme_data_t * (*fp) (pomme_data_t *arg,int n);
    int n;
    pomme_data_t *arg; 
    pomme_link_t next;
}pomme_func_t;

typedef struct pomme_rpcs pomme_rpcs_t;

struct pomme_rpcs
{
    int version; //the version of the rpc server
    int is_start;

    /* the port to use */
    short port;
    int max_pending;
    int max_clients;
    pomme_link_t func; 
    pomme_tpool_t thread_pool;
    /*regist an function*/ 
    int  (*func_register) (pomme_rpcs_t *rpcs,char *fn,void *fp,int n,pomme_data_t *arg);
    /** print all the function */
    int (*func_print)(pomme_rpcs_t *rpcs);
    int (*call)(pomme_rpcs_t *rpcs, char *name,int conn);
    /**  start the server */
    int (*start)(pomme_rpcs_t *rpcs);
};

typedef struct call_param
{
    int epid;
    /** connection handle */
    int conn;
    /* function name */
    pomme_data_t *fname;
    pomme_rpcs_t *rpcs;
}call_param_t;

/**
 * @brief pomme_rpcs_init 
 * @param rpcs
 * @return < 0 failure, = 0 for success 
 */
int pomme_rpcs_init( pomme_rpcs_t *rpcs,
	int max_thread,
	int max_waiting,
	int cur_num,
	short port);
/**
 * @brief pomme_rpcs_distroy 
 * @param rpcs
 * @return <0 failure, ==0 for success 
 */
int pomme_rpcs_distroy( pomme_rpcs_t *rpcs);

#endif
