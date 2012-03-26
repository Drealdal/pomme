/*
 * =====================================================================================
 *
 *       Filename:  pomme_utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2011 03:55:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"



/*-----------------------------------------------------------------------------
 *  init pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_init(pomme_data_t *data, u_int32 size)
{
    int ret = POMME_SUCCESS;
    if( data == NULL )
    {
	debug("init an null pointer");
	ret = POMME_MEM_ERROR;
	goto err;
    }
    if(data->data != NULL && (data->flags & POMME_DATA_NEED_FREE) != 0 )
    {
	if(data->size >= size)
	{
	    memset(data->data,0,data->size);
	    goto err;
	}else{
	    free(data->data);
	}
    }
    data->size = size;
    data->data = malloc(size);
    if(data->data == NULL)
    {
	debug("malloc data for data init failure");
	ret = POMME_MEM_ERROR;
	goto err;
    }
    memset(data->data,0,size);
    data->flags |= POMME_DATA_NEED_FREE; 
err:
    return ret;
}

/*-----------------------------------------------------------------------------
 *  distroy a pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_distroy(pomme_data_t *data)
{
    if(data == NULL)
    {
	debug("distroy null pointer");
	return 0;
    }
    if( (data->flags & POMME_DATA_NEED_FREE) != 0 && data->data != NULL)
    {
	free(data->data);
    }
    return 0;
}


int create_server(short port,
	int max_pending,
	int max_clients,
 	struct epoll_event *events,	
	int *  sockid,
	int *  epid)
{
    int ret = 0;
    struct sockaddr_in addr;
    *sockid = socket(AF_INET,SOCK_STREAM,0);
    if( *sockid < 0 )
    {
	debug("create socket fail:%s",strerror(*sockid));
	return *sockid;
    }
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if( (ret = bind(*sockid, (struct sockaddr *) &addr,
	    sizeof(struct sockaddr_in)) ) < 0 )
    {
	debug("bind errori:%s",strerror(ret));
	return ret;
    }

    if( ( ret = listen(*sockid, max_pending) < 0 ))
    {
	debug("listen error:%s",strerror(ret));	
	return ret;
    }

    if( (*epid = epoll_create(max_clients+1)) < 0 )
    {
	debug("epoll create error:%s",strerror(*epid));
	return *epid;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = *sockid;

    if( (ret = epoll_ctl( *epid,
		    EPOLL_CTL_ADD, *sockid, &ev)) < 0)
    {
	debug("epoll ctl fail:%s",strerror(ret));
	exit(ret);
    }
    return 0;
}

int create_client(char *ip,
	short port,
	int *sock_fd)
{
    int ret;
    struct sockaddr_in pin;   
    int  times = 1;
    pin.sin_family = AF_INET;   

    pin.sin_addr.s_addr = inet_addr(ip);
    pin.sin_port = htons(port);   
      
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);   
    if( *sock_fd < 0 )
    {
	debug("create socket failure:%s",strerror(errno));
	return -1;
    }

    while( (ret = connect(*sock_fd, 
		    (void *)&pin, sizeof(pin)) ) < 0 )
    {
	debug("connect %d ,Sleeping Zzzz...",times);

	sleep(1<<times);
	times++;
	if( times > 4)
	{
	    return -1;
	}
    }
    return 0;

}

char *pomme_time(char *buf)
{
    time_t t = time(NULL);
    char *re = NULL;
    if( buf != NULL )
    {
	re = ctime_r(&t,buf);	
    }else{
	re = ctime(&t);
    }
    return re;
}	
struct tm* pomme_time_all()
{
    time_t t = time(NULL);
    return localtime(&t);
}

int pomme_get_endian()
{
    int data = 1;
    char *pdata = (void *)&data;
    if( *pdata == 0x01 )
    {
	return POMME_LITTLE_ENDIAN;
    }
    return POMME_BIG_ENDIAN;

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
