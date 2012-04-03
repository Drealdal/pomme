/*
 * =====================================================================================
 *
 *       Filename:  pomme_dir.h
 *
 *    Description:  An directory
 *
 *        Version:  1.0
 *        Created:  03/27/2012 06:20:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_DIR_H
#define _POMME_DIR_H

#include "utils.h"

typedef struct pomme_dir
{
    time_t ctime; // last status change time
    u_int32 acl;
}pomme_dir_t;
#endif
