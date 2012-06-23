/*
 * =====================================================================================
 *
 *       Filename:  pomme_utils.h
 *
 *    Description:  The declaration of serveral use of pomme system 
 *
 *        Version:  1.0
 *        Created:  11/15/2011 03:56:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */

#ifndef POMME_UTILS_H
#define POMME_UTILS_H
#include <stdio.h>
#include <stdlib.h>
extern int debug_mask;
#define ALL_DEBUG 0xffffffff
#define NULL_DEBUG 0x00000000

#define FFL __FILE__,__func__,__LINE__

#ifdef DEBUG
#define debug(mask,msg,argc...) do{\
    if((mask&debug_mask)!=0)\
    {\
    	fprintf(stdout,"%s %s %d: "msg"\n",FFL,##argc);\
    }\
}while(0)

#define debug_setmask(mask) do{ debug_mask = mask;}while(0)

#else
#define debug(mask,msg,argc...) 0
#define debug_setmask(mask) 0
#endif



#define warning(msg,argc...) fprintf(stdout,"WARNING:%s %s %d: "msg"\n",FFL,##argc)
#define error(msg,argc...) fprintf(stderr,"ERROR:%s %s %d: "msg"\n",FFL,##argc)
#define err_exit(msg,argc...) do{\
fprintf(stderr,"Fatal ERROR:%s %s %d:"msg"\n",FFL,##argc);\
exit(-1);\
}while(0);

#endif
