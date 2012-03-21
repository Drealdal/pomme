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
#include "utils.h"
/* thre argument of an function */
typedef struct pomme_arg
{
    int n;// argument num
    writable *args;
}pomme_arg_t;


#define POMME_ARG(arg,n) \
    pomme_arg_t *arg = NULL;\
do{\
    arg=malloc(sizeof(pomme_arg_t));\
    arg->n = n;\
    arg->args = malloc(n*sizeof(pomme_arg_t));\
    memset(arg->args, 0, sizeof(pomme_arg_t)*n);\
}while(0)

#define POMME_ARG_I(arg,i,type) \
    do{\
	arg->args[i]->len = sizeof(type);\
    }while(0);
#define POMME_ARG_F_I(arg,i) free(arg->args[i]->data);
#define POMME_ARG_F(arg) do{\
int i;for(i=0;i<arg->n;i++){\
    POMME_ARG_F_I(arg,i);}\
    free(arg);\
    arg=NULL;\    
}while(0);

typedef struct pomme_func
{
    char *name;
    void *fp;
    void *arg; 
    pomme_link_t next;
}pomme_func_t;

typedef struct pomme_rpcs pomme_rpcs_t;

typedef struct pomme_rpcs
{
    int version; //the version of the rpc server
    int is_start;
    pomme_link_t func; 
    pomme_tpool_t thread_pool;
    /*regist an function*/ 
    int  (*func_register) (pomme_rpcs_t *rpcs,char *fn,void *fp,int n,writable *arg);
    /** print all the function */
    void (*func_print)(pomme_rpcs_t *rpcs);
    /**  start the server */
    void (*start)(pomme_rpcs_t *rpcs);
};

/**
 * @brief pomme_rpcs_init 
 * @param rpcs
 * @return < 0 failure, = 0 for success 
 */
int pomme_rpcs_init( pomme_rpcs_t *rpcs,
	int max_thread,
	int max_waiting,
	int cur_num);
/**
 * @brief pomme_rpcs_distroy 
 * @param rpcs
 * @return <0 failure, ==0 for success 
 */
int pomme_rpcs_distroy( pomme_rpcs_t *rpcs);

#endif
