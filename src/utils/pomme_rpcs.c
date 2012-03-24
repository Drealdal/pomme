/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcs.c
 *
 *    Description:  the implemention of the rpc server
 *
 *        Version:  1.0
 *        Created:  03/18/2012 10:19:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcs.h"
#include "pomme_blist.h"
#include "pomme_serilize.h"

static int fregister(pomme_rpcs_t *,char *,void *, int, writable *);
static int func_print(pomme_rpcs_t *rpcs);
static int start(pomme_rpcs_t *rpcs);

int pomme_rpcs_init(pomme_rpcs_t *rpcs,
	int max_thread,
	int max_waiting,
	int cur_num)
{
    int ret = 0;

    assert( rpcs != NULL);
    memset( rpcs, 0, sizeof(pomme_rpcs_t));
    init_link(&rpcs->func);
    rpcs->func_register = &fregister;
    rpcs->func_print = &func_print;

    ret = pomme_tp_init(&rpcs->thread_pool,
	    max_thread,max_waiting,cur_num);

    return ret;
}

int pomme_rpcs_distroy(pomme_rpcs_t *rpcs)
{
    int ret = 0;
    assert( rpcs != NULL );
    
    pomme_func_t *pos = NULL;
    pomme_link_t *head = &rpcs->func;

    list_for_each_entry(pos, head, next)
    {
	link_del(&pos->next);
	/* the pomme_arg_t->args[i]->data is NULL in func list*/
	POMME_ARG_F(pos->arg);
	free(pos);
    }
    memset(rpcs, 0, sizeof(pomme_rpcs_t));

    return ret;
}

/**
 * @brief fregister 
 * @param funcn: function name
 * @param funcp: function pointer
 * @param n: argument of number
 * @param args: writable array
 *
 * @return ==0 success, < failure 
 */
static int fregister(pomme_rpcs_t *rpcs,
	char *funcn,
	void *funcp,
	int n,
	writable *args)
{
    int ret = 0;
    assert( funcn != NULL );
    assert( funcp != NULL );
    assert( args != NULL);

    pomme_func_t *pf = malloc(sizeof(pomme_func_t));
    memset(pf, 0, sizeof(pomme_func_t));

    pomme_arg_t *arg = malloc(sizeof(pomme_arg_t));
    memset(arg, 0, sizeof(pomme_arg_t));

    arg->n = n;
    arg->args = args;

    pf->name = funcn;
    pf->fp = funcp;
    pf->arg = arg;

    link_add( &pf->next, &rpcs->func);
    return ret;
}


static int start(pomme_rpcs_t *rpcs)
{
    return 0;
}
