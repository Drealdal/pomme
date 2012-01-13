/*
 * =====================================================================================
 *
 *       Filename:  pomme_pack.h
 *
 *    Description:  pach data for transport and storage in the database
 *
 *        Version:  1.0
 *        Created:  01/12/2012 07:29:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_PACK_H
#define _POMME_PACK_H

#include "pomme_utils.h"
#include "pomme_type.h"
#include "pomme_const.h"

typedef struct pomme_package{
    
    u_int8 magic;                               /* magic id, to check valid */

    void *data;                                 /* where the data is stored */
    size_t size;                                /* the max size of the buffer */
    size_t cur;                                 /* the cur pos of the non used data */
    /*
     *  mem flags
     */
    u_int32 flags;
}pomme_package_t;

#define IS_PACK_NEED_FREE(pack) (pack->flags& POMME_SELF_NEED_FREE) != 0 
#define IS_PACK_DATA_FREE(pack) ( pack->flages & POMME_DATA_NEED_FREE) q= 0

#define SET_PACK_NEED_FREE(pack) pack->flags |= POMME_SELF_NEED_FREE
#define SET_PACK_DATA_FREE(pack) pack->flags |= POMME_DATA_NEED_FREE

#define IS_VALID_PACK(pack) pack->magic == POMME_PACKAGE_MAGIC
#define SET_PACK_MAGIC(pack) pack->magic = POMME_PACKAGE_MAGIC

#define SET_ENDIAN_LITTLE(pack) pack->data[0] |= POMME_LITTLE_ENDIAN
#define SET_ENDIAN_BIG(pack) pack->data[0] &= ~(POMME_LITTLE_ENDIAN)

#define IS_ENDIAN_BIG(pack) (pack->data[0] & POMME_LITTLE_ENDIAN ) == 0
#define IS_ENDIAN_LITTLE(pack) (pack->data[0] & POMME_LITTLE_ENDIAN ) != 0 


/**
 * @brief pomme_pack_create 
 *
 * @param pack: the result pointer will be stored here
 * @param data: where the data stored, if NULL ,
 * @param size: the size of the data, if data == NULL ,the size will be the initial length of size
 *
 * @return 
 */
int pomme_pack_create( pomme_pack_t **pack,
	void *data,
	size_t size);
/**
 * @brief pomme_pack_distroy : distroy an pack struct
 *
 * @param pack: the package to distroy
 *
 * @return 0 for success , < 0 for failure 
 */
int pomme_pack_distroy( pomme_pack_t **pack);

#define pomme_pack(data,type,pack) do{\
    size_t size = sizeof(type);\
    pack_data(data,size,pack);\
}while(0);

#define pomme_pack_array(data,type,length,pack) do{\
    size_t len = length;\
    size_t size = sizeof(type) * length;\
    pomme_pack(&len, size_t, pack);\
    pomme_pack(data, size, pack);\
}while(0);

#define pomme_unpack(data, type, pack) do{\
    size_t size = sizeof(type);\
    unpack_data(data, size, pack);\
}while(0);

#define pomme_unpack_array(data, type, length,pack) do{\
    size_t *size = NULL;\
    unpack_data(&size, sizeof(size_t), pack);\
    size_t r_size = *size;\
    *(length) = r_size / sizeof(type);\
    unpack_data(data,r_size,pack);\
}while(0);

int pack_data(void *data , size_t size, pomme_package_t *pack);
/**
 * @brief unpack_data 
 *
 * @param data: where the data will be stored
 * @param length
 * @param pack
 *
 * @return 
 */
int unpack_data(void **data, size_t length, pomme_package_t *pack);


#endif
