/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/23/2012 07:42:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_RPC_H
#define _POMME_RPC_H
#include "pomme_serilize.h"

int pomme_rpc_write(int fd,int n, pomme_data_t *wrs);
int pomme_rpc_read(int fd,int n,pomme_data_t *expect,pomme_data_t *wrs);


#endif
