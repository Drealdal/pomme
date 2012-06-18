/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_meta.h
 *
 *    Description:  the api to get meta from meta server
 *
 *        Version:  1.0
 *        Created:  2012年04月24日 23时03分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CLIENT_META_H
#define _POMME_CLIENT_META_H
#include "pomme_rpcc.h"
#include "pomme_meta.h"
#include "pomme_file.h"
/**
 * @brief pomme_sync_create_file 
 *
 * @param rct: client handle to the meta server
 * @param path: path of the file
 * @param mode: create mode of the file
 * @param file: the return handle of the file
 *
 * @return == 0 for success, < 0 for failure 
 */
int pomme_sync_create_file(rpcc_t *rct, 
	char *path,
	int mode,
	pomme_file_t **file);

int pomme_sync_read_file_meta(
	rpcc_t *rct,
	char *path,
	pomme_file_t **file,
	int *obj_num,
       	ms_object_t **object);

/**
 * @brief pomme_write_file 
 *
 * @param rct: rpc handle to metaserver
 * @param file: the file to write
 * @param off: the offset to write
 * @param len: the length to write
 * @param data: the data to write
 *
 * @return == 0 for success, < 0 failure 
 */
int pomme_write_file(rpcc_t *rct, 
	pomme_file_t *file,
       	u_int64 off,
       	u_int64 len,
       	void *data);
int pomme_stat_file(rpcc_t *rct, char *path,pomme_file_t **file);
int pomme_uuid_create(uuid_t id);
#endif
