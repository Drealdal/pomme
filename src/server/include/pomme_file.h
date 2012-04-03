/*
 * =====================================================================================
 *
 *       Filename:  pomme_file.h
 *
 *    Description:  The implement of file 
 *
 *        Version:  1.0
 *        Created:  03/27/2012 06:45:09 PM
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

/*
 * the path of the file is used as the key of the
 * file metas,an pomme_file_t object is the first
 * value item of the key
 */
typedef struct pomme_file
{
    time_t ctime;//last file status change
    time_t atime;//last access time
    time_t mtime;//last modify time
    u_int32 acl;// u g o
}pomme_file_t;

#endif
