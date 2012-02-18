/*
 * =====================================================================================
 *
 *       Filename:  pomme_const.h
 *
 *    Description:  the defination of consts
 *
 *        Version:  1.0
 *        Created:  01/11/2012 09:48:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CONST_H
#define _POMME_CONST_H

/*
 * error code definitions
 */
#define POMME_SUCCESS 0
                                                /* local file errors */
#define POMME_LOCAL_DIR_ERROR (-101) 
#define POMME_LOCAL_FILE_ERROR (-102)
#define POMME_WRITE_FILE_ERROR (-103)
                                                /* general error code */
#define POMME_MEM_ERROR (-201)
#define POMME_INVALID_PACKAGE (-202)
#define POMME_TOO_LARGE_PACKAGE (-203)
#define POMME_NOT_ENOUGH_UNPACK (-204)

/*
 * flags definitions
 */

#define POMME_DATA_NEED_FREE 1  // The pomme_data_t->data is alloc by the system, need free
#define POMME_SELF_NEED_FREE 2 // THE pomm_data_t structure is alloc by the system , need free
/*
 * const data definitions
 */
#define POMME_PACKAGE_MAGIC 0x1e

#define POMME_LITTLE_ENDIAN 1
#define POMME_BIG_ENDIAN 2

#define POMME_PACKAGE_SIZE 1024
#define POMME_MAX_PACKAGE_SIZE (1024*1024*1024)

#endif