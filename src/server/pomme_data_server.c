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

#define MAX_PENDING 100
#define MAX_CLIENTS 1000
#define PACKAGE_LENGTH 1024

int setnonblocking(int sock);
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

static int server()
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
