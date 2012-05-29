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
	PFILE *file);


int pomme_sync_read_file_meta(rpcc_t *rct,char *path,pomme_file_t *file, ms_object_t *object);
#endif
