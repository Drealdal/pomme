/*
 * =====================================================================================
 *
 *       Filename:  test_bdb_cusor.c
 *
 *    Description:  Test the use of BDB Cusor
 *
 *        Version:  1.0
 *        Created:  04/03/2012 12:49:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "db.h"
#define DB_NAME = "test_cusor"
#define DB_FILE = "./cursor.db"
int main()
{
    int ret = 0;
    unsigned int o_flags = 0;
    DB *db = NULL;

    if( ( ret = db_create(&dbp, NULL,0 ) ) != 0 )
    {
	debug("error:%s",db_strerror(ret));
	return -1;
    }

    o_flags |= DB_CREATE;
    o_flags |= DB_THREAD;

   if( (ret = db->open(db, NULL, DB_FILE, 
		   DB_NAME,DB_BTREE,o_flags,0)) != 0)
   {
       debug("open error:%s",db_strerror(ret));
       return -1;
   }


    return 0;
}
