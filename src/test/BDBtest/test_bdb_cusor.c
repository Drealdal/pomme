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
#include "utils.h"
#define DB_NAME  "test_cusor"
#define DB_FILE  "./cursor.db"

int dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2);
int main()
{
    int ret = 0;
    int i,j,data;
    unsigned int o_flags = 0,cusor_flags=0;
    DB *db = NULL;

    if( ( ret = db_create(&db, NULL,0 ) ) != 0 )
    {
	debug("error:%s",db_strerror(ret));
	return -1;
    }

    o_flags |= DB_CREATE;
    o_flags |= DB_THREAD;
//    o_flags |= DB_INIT_CDB;

   unsigned db_flags = 0;
   db_flags |= DB_DUPSORT;
   if( ( ret = db->set_flags(db, db_flags)) != 0 )
   {
       debug("Set databse flags failure:%s",db_strerror(ret));
       return -1;
   }
   /*
    *int
   DB->set_dup_compare(DB *db,
  int (*dup_compare_fcn)(DB *db, const DBT *dbt1, const DBT *dbt2)); 
    */
   if( ( ret = db->set_dup_compare(db, &dup_cmp)) != 0 )
   {
       debug("set dum_cmp failure:%s",db_strerror(ret));
       return -1;
   }

   if( (ret = db->open(db, NULL, DB_FILE, 
		   DB_NAME,DB_BTREE,o_flags,0))!= 0)
   {
       debug("open error:%s",db_strerror(ret));
       return -1;
   }
   /* optimize for bulk operation */
   cusor_flags |= DB_CURSOR_BULK;
   /* used to write */
  // cusor_flags |= DB_WRITECURSOR;

   DBC * dbc = NULL;
//int
//DB->cursor(DB *db, DB_TXN *txnid, DBC **cursorp, u_int32_t flags);
   if( ( ret =  db->cursor(db, NULL, &dbc, cusor_flags) ) !=0 )
   {
       debug("Get cusor fail:%s",db_strerror(ret));
       return -1;
   }

   DBT key, val;
   memset(&key, 0, sizeof key);
   memset(&val, 0, sizeof val);
   char *key_val = "test_multi_val"; 
   int buffer[1024];
   key.data = key_val; 
   key.size = strlen(key_val);
   key.flags |= DB_DBT_READONLY;// the key.data will not be writen by BDB then

   int *pV = &data;
   val.data = pV;
   val.size = sizeof(int);
   val.flags |= DB_DBT_USERMEM;

   unsigned int pFlags = 0;
   pFlags |= DB_KEYFIRST;

   for(i=0;i<100;i++)
   {
       data = i;
       debug("put:%d",data);
       if( (ret = dbc->put(dbc, &key, &val,pFlags))!= 0 )
       {
	   debug("put fail:%s",db_strerror(ret));
	   return -1;
       }
   }
//   int
//   DBcursor->get(DBC *DBcursor,
//   DBT *key, DBT *data, u_int32_t flags);

//   val.size = sizeof(buffer);
//   val.ulen = sizeof(buffer);
//   val.data = buffer;

   
   debug("************************************************");
   data = -1;
   val.ulen = sizeof(int);
   if( (ret = dbc->get(dbc, &key, &val,DB_SET)) != 0)
   { 
       debug("%s",db_strerror(ret));
   }else{
   
       debug("%d",data);
   }
   do{
       if( (ret = dbc->get(dbc, &key, &val,DB_NEXT)) == 0)
       { 
	   debug("%d",data);
       }else{
       
	   debug("read error:%d",db_strerror(ret));
       }
   }while(ret == 0);

   return 0;
}

int dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2)
{
   int * a = (int *)dbt1->data;
   int * b = (int *)dbt2->data;
  if( (*a) < (*b) ) return 1;
  else if( *a == *b ) return 0;
  else return -1;
}
