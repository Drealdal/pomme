/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_main.c
 *
 *    Description:  the main 
 *
 *        Version:  1.0
 *        Created:  03/07/2012 12:17:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_client_data.h"
#include "pomme_client_meta.h"
#include "pomme_meta.h"
#include "utils.h"
#include "pomme_rpcc.h"
extern int errno;
int main()
{
    int ret , sock_fd;
    char * host_name = "127.0.0.1";   
    rpcc_t rpcc;
    pomme_file_t file;

    if( ( ret = pomme_rpcc_init(&rpcc, host_name,POMME_META_RPC_PORT,0))
	    != 0 )
    {
	debug("init rpc client error");
	exit(-1);
    }
    
    if( ( ret = pomme_sync_create_file(&rpcc,"/zhumeiqi/test/",544,&file)) != 0 )
    {
	debug("create error");
	exit(-1);
    }

    return 0;


    struct hostent * server_host_name;   
    server_host_name = gethostbyname(host_name);   
      
    int port = POMME_DATA_PORT;   
    struct sockaddr_in pin;   

    pin.sin_family = AF_INET;   
//    pin.sin_addr.s_addr = htonl(INADDR_ANY);   
//    pin.sin_addr.s_addr = ((struct in_addr *)(server_host_name->h_addr))->s_addr; 

    pin.sin_addr.s_addr = inet_addr("127.0.0.1");
    pin.sin_port = htons(port);   
      
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);   
    if( sock_fd == -1 )
    {
	debug("create socket failure:%s",strerror(errno));
	return -1;
    }
    while( (ret = connect(sock_fd, (void *)&pin, sizeof(pin)) ) < 0 )
    {
	debug("connect error");
    }

    int buffer[1024];
    int r_buffer[1024];

    memset(&buffer,'c',sizeof(buffer));
    memset(&r_buffer,0,sizeof(buffer));
    int g_len = 0;

    ret = pomme_client_put_data(101,sock_fd,
	    &buffer, sizeof(buffer),0);
    if( ret < 0 )
    {
	debug("put data error");
    }

    ret = pomme_client_get_data(101,0,1024,sock_fd,
	    &r_buffer,&g_len);
    if(ret < 0 )
    {
	debug("Get data error");
	goto err;
    }
    char *p_b = (char *)&r_buffer;
    putchar(*p_b);
err:
    return ret;
}
