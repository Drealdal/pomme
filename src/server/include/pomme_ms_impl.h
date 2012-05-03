/*
 * =====================================================================================
 *
 *       Filename:  pomme_ms_iml.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/02/2012 10:59:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_MS_IML_H
#define _POMME_MS_IML_H
#include "utils.h"
#include "pomme_log.h"
#include "pomme_meta_server.h"


pomme_data_t * pomme_create_file(pomme_ms_t *ms,const char *path,const int mode);
pomme_data_t * pomme_read_file(pomme_ms_t *ms, const char *path, u_int64 offset,u_int64 len);
pomme_data_t * pomme_write_file(pomme_ms_t *ms, const char *path, u_int64 offset, u_int64 len);
pomme_data_t * pomme_stat_file(pomme_ms_t *ms, const char *path);

pomme_data_t * pomme_heart_beat(pomme_ms_t *ms,pomme_hb_t *hb);

int pomme_map_ds_group(const char *path);
int ms_start(pomme_ms_t *ms);
int ms_stop(pomme_ms_t *ms);

#endif
