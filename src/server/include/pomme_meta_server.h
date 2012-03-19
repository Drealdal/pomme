/*
 * =====================================================================================
 *
 *       Filename:  pomme_meta_server.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/03/2012 05:47:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_META_SERVER_H
#define _POMME_META_SERVER_H

//#define pomme_version 0.01
#include "utils.h"
#include "pomme_log.h"
#include "pomme_hash.h"
#include "pomme_protocol.h"
#include "pomme_type.h"
#include "pomme_meta.h"

typedef struct pomme_ms
{
    logger_t *ds_logger;
    /*
     * the ds < id <--> ip>
     */
    pomme_hash_t *ds;
    /* meta db */
    DB *meta_db

}pomme_ms_t;

#endif
