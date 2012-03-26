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

static int fregister(pomme_rpcs_t *,char *,void *, int, pomme_data_t *);
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
	pomme_data_t *args)
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
static int printf_arg(pomme_arg_t *arg)
{
    int i;
    if(arg == NULL)
    {
	printf("arg_num:0\n");
	printf("arg: NULL\n");
	return 0;
    }
    printf("arg num:%d\n",arg->n);
    printf("\tlen\n");

    for( i = 0 ; i < arg->n ; i ++)
    {
	printf("\targ->args[i].size");
    }
    printf("\n");
}
static int func_print(pomme_rpcs_t *rpcs)
{
    int ret = 0;
    pomme_func_t *pos = NULL;

    list_for_each_entry(pos, (&rpcs->func),next)
    {
	printf("func name:%s\n",pos->name);
	printf_arg(pos->arg);
    }
    return ret;
}

static int start(pomme_rpcs_t *rpcs)
{
    int ret = 0;
    int i, nfds = 0,  conn_sock;
    int sock_fd,epid;
    struct epoll_event ev;

    /* max_clients +1  */
    struct epoll_event events[rpcs->max_clients+1];

    rpcs->func_print(rpcs);
    rpcs->thread_pool.start(&rpcs->thread_pool);
    if( ( ret = create_server(rpcs->port, 
		   rpcs->max_pending,rpcs->max_clients, 
		   events,&sock_fd, &epid) ) < 0 )
    {
	debug("create server error");
	exit(-1);
    }

    for(;;)
    {
	nfds = epoll_wait(epid, events, rpcs->max_clients+1, -1);
	if( nfds == -1)
	{
	    debug("wait failure");
	    exit(-1);
	}
	for( i = 0 ; i < nfds ; ++i )
	{
	    if( events[i].data.fd == sock_fd )
	    {
		conn_sock = accept(sock_fd, (struct sockaddr *)NULL, NULL);
		if( conn_sock == -1 )
		{
		    debug("accept error");
		    continue;
		}
		setnonblocking(conn_sock);

		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = conn_sock;
		if( epoll_ctl(epid, EPOLL_CTL_ADD, conn_sock, &ev) < 0 )
		{
		    debug("epoll_ctl conn_sock fail");
		    continue;
		}
	    }else{

	    }
	}

    }

    return 0;
}
