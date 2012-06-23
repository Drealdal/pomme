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
#include "pomme_rpc.h"
#include "pomme_blist.h"
#include "pomme_serilize.h"

static int fregister(pomme_rpcs_t *,char *,void *, int, pomme_data_t *);
static int func_print(pomme_rpcs_t *rpcs);
static int start(pomme_rpcs_t *rpcs);
static int call(pomme_rpcs_t *rpcs, char *name, int conn); 

int pomme_rpcs_init(pomme_rpcs_t *rpcs,
	void *extra,
	int max_thread,
	int max_waiting,
	int cur_num,
	short port)
{
    int ret = 0;
    assert( rpcs != NULL);
    memset( rpcs, 0, sizeof(pomme_rpcs_t));
    rpcs->extra = extra;
    init_link(&rpcs->func);
    rpcs->func_register = &fregister;
    rpcs->func_print = &func_print;
    rpcs->start = &start;
    rpcs->call  = &call;

    rpcs->port = port;

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
	free(pos->arg);
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

    pomme_func_t *pf = malloc(sizeof(pomme_func_t));
    if( pf == NULL )
    {
	ret = POMME_MEM_ERROR;
	goto err;
    }
    memset(pf, 0, sizeof(pomme_func_t));

    /*  
    pomme_arg_t *arg = malloc(sizeof(pomme_arg_t));
    if( arg == NULL )
    {
	ret = POMME_MEM_ERROR;
	goto malloc_err;
    }
    memset(arg, 0, sizeof(pomme_arg_t));
    */
    pf->n = n;
    pf->arg = args;

    pf->name = funcn;
    pf->fp = funcp;

    link_add( &pf->next, &rpcs->func);
    return ret;
//malloc_err:
//    free(pf);
err:
    return ret;
}
static int printf_arg(int n,pomme_data_t *arg)
{
    int i;
    if(arg == NULL)
    {
	printf("arg_num:0\n");
	printf("arg: NULL\n");
	return 0;
    }
    printf("arg num:%d\n",n);
    printf("\tlen\n");

    for( i = 0 ; i < n ; i ++)
    {
	printf("\t%d",arg[i].size);
    }
    printf("\n");
    return 0;
}
static int func_print(pomme_rpcs_t *rpcs)
{
    int ret = 0;
    pomme_func_t *pos = NULL;

    list_for_each_entry(pos, (&rpcs->func),next)
    {
	printf("func name:%s\n",pos->name);
	printf_arg(pos->n,pos->arg);
    }
    return ret;
}

static int call(pomme_rpcs_t *rpcs, char *name, int conn)
{
    debug("begin call");
    int i,ret = 0 ;
    int find = 0 ;

    pomme_data_t rat;
    memset(&rat, 0, sizeof(pomme_data_t));

    pomme_func_t *pfunc = NULL;
    list_for_each_entry(pfunc,(&rpcs->func),next)
    {
	if(strcmp(name,pfunc->name) == 0 )
	{
	    find = 1;
	    break;
	}
    }
    if( find == 0 )
    {
	/*
	 * send not found
	 */
	debug("not found");
	rat.size = POMME_UNFIND_FUNC;
	write_data(&rat, conn);
	return ret;
    }
    pomme_data_t *argus = NULL;
    if((ret = pomme_rpc_read(conn,pfunc->n, 
		    pfunc->arg,&argus) ) < 0)
    {
	debug("read argument error");
	/*
	 * send argument error
	 */
	rat.size = ret;
	write_data(&rat, conn);
	return ret;
    }

    pomme_data_t *ra = pfunc->fp(rpcs->extra,pfunc->n,argus);
    if( ra == NULL )
    {
	debug("error occured call the function");
	rat.size = POMME_ERROR_EXE;
	write_data(&rat, conn);

	ret = POMME_ERROR_EXE;
	goto clean_ret;
    }
    ret = write_data(ra, conn);
    pomme_data_distroy(&ra);
    pomme_data_t *pa = NULL;
clean_ret:
    for( i = 0; i < pfunc->n; i++)
    {
	pa = argus+i;
	pomme_data_distroy(&pa);
    }
    free(argus);
    return ret ;

}

static void thread_call(void *argu)
{
    debug("thread call");
    /* the mem of worker and argu will be freed by the caller thread */
    call_param_t *param = (call_param_t *)argu;
    pomme_rpcs_t *rpcs = param->rpcs;
    rpcs->call(rpcs,param->fname->data, param->conn);
/* clear and free */
    close(param->conn);
    pomme_data_distroy(&param->fname);
}
static int handle_request(pomme_rpcs_t *rpcs,
	int epid,
	int conn)
{
    int ret = 0;
    pomme_data_t *func_name = malloc(sizeof(pomme_data_t));
    if( func_name ==  NULL )
    {
	debug("malloc error");
	return POMME_MEM_ERROR;
    }
    memset(func_name, 0, sizeof(pomme_data_t));

    ret = read_data(func_name, conn);
    if(ret < 0)
    {
	debug("read error");
	ret = -1;
	goto func_name_err;
    }
    debug("Calling: %s",(char *)func_name->data);

    /* will be release in the rpcs->call */
    call_param_t *argu = malloc(sizeof(call_param_t));
    

    if( argu == NULL )
    {
	ret = POMME_MEM_ERROR;
	goto func_name_err;
    }
    memset(argu, 0, sizeof(call_param_t));

    argu->rpcs = rpcs;
    argu->epid = epid;

    argu->fname = func_name;
    argu->conn = conn; 

    pomme_worker_t *worker = malloc(sizeof(pomme_worker_t));
    if( worker == NULL )
    {
	ret = POMME_MEM_ERROR;
	goto malloc_worker_err;
    }
    memset(worker, 0, sizeof(worker));

    worker->process = &thread_call;
    /*
     * argu will be free by worker thread 
     */
    worker->arg = argu;

    if( (ret = rpcs->thread_pool.add_worker(&rpcs->thread_pool, worker)))
    {
	debug("add worker fail");
	goto add_worker_err; 
    }
    debug("add to queue");
    return ret;
    
add_worker_err:
    free(worker);
malloc_worker_err:
    free(argu);
func_name_err:
    pomme_data_distroy(&func_name);
    return ret;

}

static int start(pomme_rpcs_t *rpcs)
{
    int ret = 0;
    int i, nfds = 0,  conn_sock;
    int sock_fd,epid;
    struct epoll_event ev;

    debug("Trying to start rpc server....");
    /* max_clients +1  */
    struct epoll_event events[rpcs->max_clients+1];

    rpcs->func_print(rpcs);
    rpcs->thread_pool.start(&rpcs->thread_pool);
    debug("Marking");
    if( ( ret = create_server(rpcs->port, 
		   rpcs->max_pending,rpcs->max_clients, 
		   events,&sock_fd, &epid) ) < 0 )
    {
	debug("create server error");
	exit(-1);
    }
    debug("rpcs started");

    for(;;)
    {
	nfds = epoll_wait(epid, events, rpcs->max_clients+1, -1);
	if( nfds == -1)
	{
	    debug("wait failure");
	    continue;
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
//		setnonblocking(conn_sock);

		ev.events = EPOLLIN | EPOLLET| EPOLLONESHOT;
		ev.data.fd = conn_sock;
		if( epoll_ctl(epid, EPOLL_CTL_ADD, conn_sock, &ev) < 0 )
		{
		    debug("epoll_ctl conn_sock fail");
		    continue;
		}
	    }else{
		
		if( (ret = handle_request(rpcs,epid,events[i].data.fd) )
		       	== POMME_RPC_EXIT )
		{
		    break;
		}
	    }
	}

    }

    return 0;
}
int stop(pomme_rpcs_t *rpcs)
{
    rpcs->thread_pool.stop(&rpcs->thread_pool);
    return 1;
}
