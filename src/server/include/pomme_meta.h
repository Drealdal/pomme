
/*
 * =====================================================================================
 *
 *       Filename:  pomme_meta.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/12/2011 06:15:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_META_H
#define POMME_META_H
#include "utils.h"
#include "pomme_type.h"

#define MAX_STORE_FILE_NAME_LENGTH 256

typedef struct pomme_object
{

	/*
	 * This structure contains the basic information of an
	 * object, an object is part of an file.
	 */
	size_t	sfid;// the id of the storage file
	u_int64 offset;// the offset of the object in the file
	u_int64 len;// the dataLength of the object

	time_t time;// the create time of the object on this server
	/*
	 * reserved not used recently , the version of the data is
	 * equal to the rank of the object in the val of the key
	 * --------------------------------------------------------
	 * FileName | object1 object2 object3 .... objectN ....
	 * --------------------------------------------------------
	 * N is treated as the version number of the object.
	 * We should add constraint of the number of an file thus
	 * constraint the version number of the File.We could do combine
	 * operation on objects, this will resulted in bigger object
	 * in another Storage file. This will only be used when we sort to
	 * very high performance.
	 *
	 */
	u_int32 ver;
	off_t start;
}pomme_object_t;

typedef struct pomme_file
{
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
typedef struct ms_object ms_object_t;
struct ms_object
{
    uuid_t id;
    u_int64 off;
    u_int64 len;
};

#endif

