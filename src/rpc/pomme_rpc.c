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
#include "pomme_const.h"
#include "utils.h"

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

int pomme_rpc_read(int fd, int n ,
       	pomme_data_t *expect, pomme_data_t **re)
{
    int i  , ret = 0;
    /* read the number of agument */
    int rn = 0;

    if( ( ret = read(fd, &rn , sizeof(int))) < 0)
    {
	debug("read_error");
	return POMME_READ_MSG_ERROR; 
    }

    if(rn != 0 )
    {
	*re = malloc(rn*sizeof(pomme_data_t)); 
    }else{
	*re = NULL;
    }
    memset(*re, 0, rn*sizeof(pomme_data_t));

   for( i = 0; i < rn ; i++)
   {
        read_data(*re+i, fd);
   }

   if(n != rn )
   {
       ret = POMME_UNMATCH_ARGU_NUM;
       goto unmatch;
   }

   for( i = 0; i < n ; i++ )
   {
       if(expect[i].size != -1 && expect[i].size != (*re)[i].size)
       {
	   ret = POMME_UNMATCH_ARGU_TYPE;
	   goto unmatch;
       } 
   }
   goto ex;
unmatch:
   debug("unmatch arg");
   for(i = 0 ; i < rn ; i++)
   {
       pomme_data_t *pi = *re + i;
       pomme_data_distroy(&pi);
   } 
   free(*re);
ex:
   debug("read over");
   return ret;
}
