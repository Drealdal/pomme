
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
	u_int64 length;// the dataLength of the object

	time_t time;// the create time of the object on this server
	/*
	 * reserved£¬ not used recently , the version of the data is
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
	u_int16 version;
	/*
	 * the _offset of the data within the file this object beyong to
	 */
	u_int64 _offset;

}pomme_object_t;

typedef struct pomme_file
{
	/*
	 * full path of the file in the file system
	 * path is used as the key of the file
	 */
	u_int8 *path;
	/*
	 * the create time of the file
	 */
	time_t c_time;
	/*
	 * the modify time
	 */
	time_t m_time;
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
	u_int32 access;
}pomme_file_t;



#endif

