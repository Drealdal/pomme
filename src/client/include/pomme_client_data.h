/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_data.h
 *
 *    Description:  the header for client_data
 *
 *        Version:  1.0
 *        Created:  03/01/2012 06:49:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CLIENT_DATA_H
#define _POMME_CLIENT_DATA_H

#include "utils.h"
#include "pomme_pack.h"
#include "pomme_protocol.h"
#include "pomme_msg.h"


typedef struct pomme_client
{
    /**
     * @brief meta server, the root server of the cluster
     */
   int mserver_handle; 

}pomme_client_t;

/**
 * @brief pomme_put_data : put data to an connection
 *
 * @param id: the id of the object
 * @param handle: the handle of the connection
 * @param buffer: the buffer where the data to be sent is stored
 * @param len: the length of the data
 *
 * @return : < 0 failure, ==0 success
 */
int pomme_client_put_data(
	uuid_t id,
	int handle, 
	void *buffer,
	int len,
	int flags);

/**
 * @brief pomme_get_data : get  data from an 
 *
 * @param id: id of the object
 * @param off: the offset within the object
 * @param len: the len of data to get
 * @param handle: the handle of the connection
 * @param buffer: where the received data is stored
 * @param len: the recved data length
 *
 * @return: < 0 failure, = 0 success 
 */
int pomme_client_get_data(u_int64 id,
	size_t off,
	size_t len,
	int handle,
	void *buffer,
	int *r_len);
int pomme_client_close(int handle);


#endif
