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
int pomme_data_init(pomme_data_t **pdata, int size)
{
    int ret = POMME_SUCCESS;

    if( *pdata == NULL )
    {
	*pdata = malloc(sizeof(pomme_data_t));
	if( *pdata == NULL )
	{
	    return POMME_MEM_ERROR;
	}
	memset(*pdata, 0, sizeof(pomme_data_t));
	(*pdata)->flags |= POMME_SELF_NEED_FREE;
    }
    pomme_data_t *data = *pdata;
    
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
    if( size > 0 )
    {
	data->data = malloc(size);
	if(data->data == NULL)
	{
	    debug("malloc data for data init failure");
	    ret = POMME_MEM_ERROR;
	    goto err;
	}
	memset(data->data,0,size);
	data->flags |= POMME_DATA_NEED_FREE; 
    }
err:
    return ret;
}
int pomme_data_extend(pomme_data_t *data, int size)
{
    assert(data != NULL);
    if(size == 0) return 0;
    data->size += size;
    void * f = data->data;
    if( (data->flags & POMME_DATA_NEED_FREE) == 0 )
    {
	data->flags |= POMME_DATA_NEED_FREE;
	if((data->data = malloc(data->size) ) == NULL )
	{
	    debug("Malloc error");
	    return POMME_MEM_ERROR;
	}
	memcpy(data->data, f, data->size - size );
    }else{
	data->data = realloc(data->data,data->size);
	if( f == data->data )
	{
	    debug("realloc error");
	    return POMME_MEM_ERROR;
	}
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 *  distroy a pomme_data_t
 *-----------------------------------------------------------------------------*/
int pomme_data_distroy(pomme_data_t **pdata)
{
    pomme_data_t *data = *pdata;
    if(data == NULL)
    {
	debug("distroy null pointer");
	return 0;
    }
    if( (data->flags & POMME_DATA_NEED_FREE) != 0 && data->data != NULL)
    {
	free(data->data);
    }
    if( (data->flags & POMME_SELF_NEED_FREE) != 0 )
    {
	free(data);
	*pdata = NULL;
    }
    else{
	memset(data, 0 , sizeof(pomme_data_t));
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

int create_client(u_int32 ip,
	short port,
	int *sock_fd)
{
    int ret;
    struct sockaddr_in pin;   
    int  times = 1;
    pin.sin_family = AF_INET;   

    pin.sin_addr.s_addr = ip;
    pin.sin_port = htons(port);   
      
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);   
    debug("ip:%u port:%d",ip, port);
    if( *sock_fd < 0 )
    {
	debug("create socket failure:%s",strerror(errno));
	return -1;
    }

    while( (ret = connect(*sock_fd, 
		    (void *)&pin, sizeof(pin)) ) < 0 )
    {
	debug("connect %d %s,Sleeping Zzzz...",times,strerror(ret));

	sleep(1<<times);
	times++;
	if( times > 4)
	{
	    return -1;
	}
    }
    return 0;
}


char *pomme_time(time_t t,char *buf)
{
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
int pomme_strlen(const char *path)
{
    assert(path != NULL );
    return strlen(path)+1;
}
int pomme_set_timer(int sec, int usec)
{
    struct itimerval val;
    val.it_value.tv_sec = sec;
    val.it_value.tv_usec = usec;

    val.it_interval = val.it_value;
    setitimer(ITIMER_REAL, &val, NULL);
    return 0;
}
void pomme_set_sigaction(int signo, void (*handle)(int))
{
    struct sigaction tact;
    tact.sa_handler = handle;
    tact.sa_flags = 0;

    sigemptyset(&tact.sa_mask);
    sigaction(signo,&tact, NULL);
    return ;
}
void print_uper(FILE *fd,char *str)
{
    if(str == NULL) return ;
    char *ps = str;
    while(ps != NULL)
    {
	if( *ps >'A' && *ps < 'Z' )
	{
	    fprintf(fd,"%c",*ps);
	    continue;
	}
	fprintf(fd,"%c",*ps + 'A' -'a');
    }
    return; 
}
char * to_uper(const char *str)
{
    if(str == NULL) 
    {
	return NULL;
    }

    char *re = malloc(strlen(str)+1);
    char *t = re;
    if( re == NULL )
    {
	error("Malloc Error");
    }
    while(*str != '\0')
    {
	if(*str >='a' && *str <= 'z'){
	    *re++ = *str++ + 'A' - 'a';
	}else
	{
	    *re++ = *str++;
	}

    }
    *re = '\0';
    return t;
}
/*  make short name of the str */
char *pomme_short_name(const char *str,int len)
{
    if( str == NULL )
    {
	return NULL;
    }
    char *t = malloc(len);
    memset(t, 0 , len);
    memcpy( t, str, len-1 );
    return t;
}

/*  pomme_file_copy */
int pomme_fcopy(const char *src, const char *dst)
{
    int ret,i, has_hole = 0;
    struct stat state;
    ret = stat(src, &state);
    if( (state.st_blksize * state.st_blocks ) -
	    state.st_size > state.st_blocks )
    {
	has_hole = 1;
	debug("hole file");
    }
    unsigned char buffer[state.st_blksize];
    FILE *in,*out;

    in = fopen(src,"r");
    if( in == NULL )
    {
	err_exit("Open input file error");
    }

    out = fopen(dst,"w+");
    if( out == NULL )
    {
	err_exit("Open output file error");
    }

    while( ( ret = fread(buffer, state.st_blksize,1,in)) > 0 )
    {
	i = 0;
	if(has_hole)
	{

	    while(i<ret&&buffer[i++]==0);
	    fseek(out, i, SEEK_SET);
	}
	fwrite(buffer+i, ret - i,1,out);
    }
    return ret;
}
