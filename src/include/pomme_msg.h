/*
 * =====================================================================================
 *
 *       Filename:  pomme_msg.h
 *
 *    Description:  the defination of send and receive msg
 *
 *        Version:  1.0
 *        Created:  02/17/2012 09:37:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_MSG_H
#define _POMME_MSG_H
#include "utils.h"



/**
 * @brief pomme_send send data of length len
 *
 * @param handle: the file handle of the collection
 * @param buffer: where the data is 
 * @param len: the len of the data
 * @param flags: the send flags of the connection
 *
 * @return : == 0 success, <0 failure
 */
int pomme_send(int handle, 
	void *buffer,
       	size_t len,
	int flags);

/**
 * @brief pomme_recv 
 *
 * @param handle: the file handle of the collection
 * @param buffer: where the data is 
 * @param len: the len of the data buffer
 * @param r_len: the received data len
 * @param flags: the recv flags of the connection
 *
 * @return ==0 success, < 0 failure 
 */
int pomme_recv(int handle,
       	void *buffer,
       	size_t len,
	size_t *r_len,
	int flags);

#endif
