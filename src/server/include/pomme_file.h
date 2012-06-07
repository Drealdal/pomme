/*
 * =====================================================================================
 *
 *       Filename:  pomme_file.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2012 01:17:15 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_FILE_H
#define _POMME_FILE_H
#include "utils.h"
#include "pomme_type.h"
typedef struct pomme_file
{
    char type;
    /* where the objects stored,will not be changed utils some server is down
     * if any server in a dsgroup fails, we just modify infomation of the 
     * dsgroup */
    u_int32 dsgroup;
    /* the create time of the file*/
    time_t c_time;
    /** the modify time*/
    time_t m_time;
    /*  last access time */
    time_t a_time;
    /*
     * the delete time,
     * if the d_time < cur_time , the file is treated
     * deleted
     */
    time_t d_time;
    /*
     * The length of the file
     */
    u_int64 len;
    /*
     *access
     */
    int acl;
    /* storage server */
}pomme_file_t;

#endif
