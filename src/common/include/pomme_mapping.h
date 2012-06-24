/*
 * =====================================================================================
 *
 *       Filename:  pomme_mapping.h
 *
 *    Description:  mapping inode to servers
 *
 *        Version:  1.0
 *        Created:  06/23/2012 05:46:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */

#ifndef _POMME_MAPPING_H
#define _POMME_MAPPING_H
typedef struct mapping{
    int ( *inode2ms)(u_int64 inode); 
}map_t;

#endif
