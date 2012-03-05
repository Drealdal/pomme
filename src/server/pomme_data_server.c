/*
 * =====================================================================================
 *
 *       Filename:  pomme_data_server.c
 *
 *    Description:  the implement of the pomme data server
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:15:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_msg.h"
#include "pomme_storage.h"
#include "pomme_protocol.h"
#include "pomme_data_server.h"

#define MAX_PENDING 100
#define MAX_CLIENTS 1000
#define PACKAGE_LENGTH 1024

extern int stop_log;

int pomme_env_init(pomme_env_t *env,
	unsigned int c_flags,
	unsigned int o_flags,
	char *home,
	char *meta_file,
	int mode)
{
    int ret = 0;
    assert( env != NULL );
    assert( home != NULL );

    ret = pthread_mutex_init(&env->mutex, NULL);

    if( ret != 0 )
    {
	debug("init mutex error");
	goto mutex_err;
    }

    ret = db_env_create( &env->db_env,c_flags); 

    if( ret < 0 )
    {
	debug("create db env error:%s",db_strerror(ret));
	goto env_err;
    }

   ret =  env->db_env->open(env->db_env, home, o_flags, mode);
   if( ret != 0 )
   {
       debug("open db env error: %s",db_strerror(ret));
       goto open_err;
   }

   ret = db_create( &env->db_meta, NULL, 0);

   if( ret != 0 )
   {
       debug("Create Db meta fail: %s",db_strerror(ret));
       goto create_db_err;
   }

   DB_ENV * pEnv = env->db_env;
   DB_TXN *tid;

   /*
    * No to thread will do the write operation
    * on the same record
    *
    */
   //TODO consider the flags
   ret = pEnv->txn_begin(pEnv, NULL, &tid,
	   DB_READ_COMMITTED);
   if( ret != 0 )
   {
       debug("txn_begin error");
       goto create_db_err;
   }

   DB *dbp = env->db_meta;

   if(( ret = dbp->open(dbp,tid,env->meta_file,NULL,DB_QUEUE,DB_CREATE,0664))!=0)
   {
       dbp->err(dbp,ret,"open failed%s",env->meta_file);
       goto create_db_err;
   }
   return ret;
	
create_db_err:

open_err:
	//TODO FLAGS
	env->db_env->close(env->db_env, 0);
env_err:
mutex_err:
	return ret;


}

int pomme_env_distroy(pomme_env_t *env)
{
    int ret = 0;

    env->db_env->close(env->db_env, DB_FORCESYNC);
    return ret;
err:
    return ret;
}

int pomme_ds_init( pomme_ds_t *ds, 
	char *home,
	char *meta_file,
	unsigned int env_c_flags,
	unsigned int env_o_flags,
	int env_mode)
{
    int ret = 0;
    assert( ds != NULL); 

    ret = pomme_env_init(&ds->env, 
	    env_c_flags,env_o_flags,
	    home,meta_file,
	    env_mode);
    if( ret < 0 )
    {
	debug("data server env init failure");
	goto err;
    }
	
    ret  = pomme_hash_int_int( 100,&ds->storage_file);
    if( ret < 0 )
    {
	debug("init hash fail");
	goto hash_err;
    }
    
    init_log();
    ds->ds_logger = create_logger(POMME_LOG_MAX,"data_server");
    if( NULL == ds->ds_logger )
    {
	debug("create logger failure");
	goto logger_err;
    }
    POMME_LOG_INFO("Data Server data structure init success",ds->ds_logger);
    return ret;

logger_err:
    pomme_hash_distroy(&ds->storage_file);
hash_err:
    pomme_env_distroy(ds->env);
err:
    return ret;

}


int pomme_ds_distroy( pomme_ds_t *ds )
{
    int ret = 0;
    POMME_LOG_INFO("STOP DATA SERVER",ds->ds_logger);
    stop_log = 1;
    pomme_env_distroy(ds->env);
    pomme_hash_distroy(ds->storage_file);
    while(stop_log == 1)
    {
	sleep(1);
    }

    return ret;
}

int setnonblocking(int sock);

static int handle_put_data(int handle, pomme_protocol_t *pro)
{
	assert( pro!=NULL );
}
static int handle_request(int handle)
{
    char buffer[PACKAGE_LENGTH];
    int flags, ret = 0;
    size_t r_len = 0;

    ret = pomme_recv(handle, buffer, PACKAGE_LENGTH, &r_len, flags);
    if( ret < 0 )
    {
	debug("recev fail");
	goto err;
    }
   
   pomme_protocol_t pro;
   memset( &pro, 0, sizeof(pomme_protocol_t));

   pomme_pack_t *p_buffer = &buffer;

   ret = unpack_msg( &pro, p_buffer);
   if( ret < 0 )
   {
       debug("unpack msg failure");
       goto err;
   }

   switch( pro.op )
   {
       case put_data:
	   break;
       case get_data:
	   break;

       default:
	   debug("unknown protocol msg");
	   ret = POMME_UNKNOWN_MSG;
	   break;
   }
	
err:
    return ret;
}

int server()
{
    int ret = 0;
    struct sockaddr_in addr;
    int socid = socket( AF_INET, SOCK_STREAM, 0);	
    if( socid < 0 )
    {
	debug("create socket identifier fail: %s",strerror(socid));
	goto err_exit;
    }

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);

    ret = bind(socid, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if( ret < 0 )
    {
	debug("bind error: %s",strerror(ret));
	goto err_exit;
    }

    if( listen( socid, MAX_PENDING) < 0 )
    {
	debug("listen error");
	goto err_exit;
    }

    int epid = epoll_create(MAX_CLIENTS+1);
    if( epid < 0 )
    {
	debug("epoll create failure");
	goto err_exit;
    }

    struct epoll_event ev, events[MAX_CLIENTS+1];
    ev.events = EPOLLIN;
    ev.data.fd = socid;

    if( epoll_ctl(epid, EPOLL_CTL_ADD, socid, &ev) < 0 )
    {
	debug("epoll_ctl sockid fail");
	goto err_exit;
    }

    int nfds = 0, i, conn_sock;
    while(1)
    {
	nfds = epoll_wait(epid, events, MAX_CLIENTS+1, -1);
	if( nfds == -1 )
	{
	    debug("epoll wait failure");
	    goto err_exit;
	}

	for( i = 0; i< nfds; ++i )
	{
	    if( events[i].data.fd == socid )
	    {
		conn_sock = accept(socid, (struct sockaddr *)NULL, NULL);
		if( conn_sock == -1 )
		{
		    debug("accept error");
		    goto err_exit;
		}
//		setnonblocking(conn_sock);
//		ev.events = EPOLLIN | EPOLLET;
//		ev.data.fd = conn_sock;
//
//		if( epoll_ctl(epid, EPOLL_CTL_ADD, conn_sock, &ev) < 0 )
//		{
//		    debug("epoll_ctl conn_sock fail");
//		    goto err_exit;
//		}
		
	    }
	}
    }


err_exit:
    return -1;
}

int setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
	debug("fcntl(sock,GETFL)");
	return -1;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
	perror("fcntl(sock,SETFL,opts)");
	return -1;
    }
    return 0;
}

