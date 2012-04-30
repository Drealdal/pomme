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
int pomme_create_file(rpcc_t *rct, char *path,int mode);


#endif
