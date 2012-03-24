/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/23/2012 10:00:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpc.h"

int pomme_rpc_write(int fd, int n , pomme_data_t *wrs)
{
    int i,ret = 0;
    /* write the number of agument */
    if( (ret = write(fd, &n, sizeof(int))) < 0)
    {
	debug("write error");
	return POMME_WRITE_MSG_ERROR;
    }
    for( i = 0 ; i < n ; i ++ )
    {
	write_data(&wrs[i],fd);
    }
    return ret;
}

int pomme_rpc_read(int fd, int n , pomme_data_t *epect, pomme_data_t **read)
{
    int i  , ret = 0;
    /* read the number of agument */
    int rn = 0;

    if( ( ret = read(fd, &rn , sizeof(int))) < 0)
    {
	debug("read_error");
	return POMME_READ_MSG_ERROR; 
    }
   *read = malloc(rn*sizeof(pomme_data_t)); 
   for( i = 0; i < rn ; i++)
   {
        read_data(*read+i, fd);
   }
   if(n != rn )
   {
       ret = POMME_UNMATCH_ARGU_UNM;
       goto unmatch;
   }
   for( i = 0; i < n ; i++ )
   {
       if(expect[i].size != (*read)[i].size)
       {
	   ret = POMME_UNMATCH_ARG_TYPE;
	   break;
       } 
   }
unmatch:
   for(i = 0 ; i < rn ; i++)
   {
       pomme_data_distroy(*read+i);
   } 
   free(*read);
ex:
   return ret;
}
