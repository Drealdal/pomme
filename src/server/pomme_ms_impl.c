/*
 * =====================================================================================
 *
 *       Filename:  pomme_ms_iml.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/02/2012 11:02:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_ms_impl.h"

pomme_data_t * pomme_create_file(pomme_ms_t *ms,const char *path,const int mode)
{
    int ret = 0;
    debug("Create file:%s\n",path);
    pomme_data_t *re = NULL ; 

    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    DBC *dbc = NULL;

    key.size = strlen(path);
    key.data = (void *)path;
    pomme_file_t file;
    memset(&file, 0 , sizeof(pomme_file_t));
    time(&file.c_time);
    
    file.acl = mode;
    file.dsgroup = ms->get_ds_group(path);

    val.size = sizeof(pomme_file_t);
    val.data = &file;

    int flags = 0;
    flags |= DB_NOOVERWRITE;

    debug("Before put");
    if( ( ret = ms->meta_db->put(ms->meta_db, NULL, 
		    &key, &val, flags)) != 0 )
    {
	debug("duplicate file");
	pomme_data_init(&re,POMME_META_FILE_EXIST);
	goto ret;
    }
    pomme_data_init(&re,0);

ret:
    return re;
}

pomme_data_t *pomme_read_file(pomme_ms_t *ms, const char *path, u_int64 off,u_int64 len)
{
    pomme_data_t * re = NULL;

    DB *pdb = ms->meta_db;
    DBC *dbc = NULL;
    if ( ( ret = pdb->cursor(pdb, NULL, &dbc,
		    0) ) != 0 )
    {
	debug("Create cursor fail:%s",db_strerror(ret));
	POMME_LOG_ERROR("Create cursor fail",ms->ms_logger);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto e_exit;
    }
    pomme_data_init(&re, 0 );

   //TODO 
e_exit:
    return re; 
} 

pomme_data_t  *pomme_write_file(pomme_ms_t *ms, const char *path, u_int64 off, u_int64 len)
{
    int ret = 0;
    pomme_data_t * re = NULL;
    ms_object_t ob;
    if ( (ret = ms_create_object(&ob.id) ) !=0 )
    {
	debug("Get object id failure");
	pomme_data_init(&re,POMME_META_INTERNAL_ERROR);
	goto err;
    }
    ob.off = off;
    ob.len = len;

    DBT key,val;
    memset(&key, 0 , sizeof(DBT));
    memset(&val, 0 , sizeof(DBT)); 
    key.size = pomme_strlen(path);
    key.data = path;

    val.size = sizeof(ms_object_t);
    val.data = &ob;

    DBC *dbc = NULL;
    if ( (ret = ms->meta_db->cursor(ms->meta_db , NULL ,
		    &dbc , cursor_flags )) != 0 )
    {
	debug("Get cursor fail:%s",db_strerror(ret));
	pomme_data_init(&re,POMME_META_INTERNAL_ERROR);
	POMME_LOG_ERROR(ms->ms_logger, "Create cursor Error");
	goto err;
    }	

    int put_flag = 0;
    put_flag |= DB_KEYLAST;
   if ( ( ret =  dbc->put(dbc, &key, &val,
		   put_flag) ) != 0 )
   {
       debug("Put to Db fail");
       pomme_data_init(&re, POMME_META_INTERNAL_ERROR);

       POMME_LOG_ERROR(ms->ms_logger,"Write DB Error")
       goto err;
   }
   pomme_data_init(&re,sizeof(u_int64));
   *(u_int64 *)re->data =  ob.off;

    return re;
err:

    return re;
}

pomme_data_t *pomme_stat_file(pomme_ms_t *ms, const char *path)
{
    int ret = 0;
    pomme_data_t *re = NULL;
    debug("Stat file %s",path);
    DBT key, val;
    
    pomme_init_data(&re, sizeof(pomme_file_t));

    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    key.size = pomme_strlen(path);
    key.data = (void *)path; 

    val.size = sizeof(pomme_file_t);
    val.data = re.data; 
    val.flags |= DB_DBT_USERMEM; 

    DB *pdb = ms->meta_db;

    if( ( ret = pdb->get(pdb,NULL, &key,&val,0)) != 0 )
    {
	debug("Read Db Error");
	pomme_data_distroy(&re);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto re;
    }
    return re;
}
pomme_data_t *pomme_heart_beat(pomme_ms_t *ms, pomme_hb_t *hb)
{
    pomme_data_t *re = NULL;
    // if hb->myid == -1 and hb->mygroup == -1
    if( hb->mygroup == -1 )
    {
	pomme_data_init(&re, 2*sizeof(u_int32));
	ms_create_ds(ms, re->data, (u_int32 *)re->data + 1);
	hb->mygroup = *((u_int32 *)re->data + 1 );
	hb->myid = *((u_int32 *)re->data );
    }

    DBT key, val;
    memset( &val, 0, sizeof(DBT));
    memset( &key, 0, sizeof(DBT));

    key.size = sizeof(u_int32); 
    key.data = hb->id;

    val.size = sizeof(pomme_hb_t) - sizeof(u_int32);
    val.data = (u_int32 *)hb + 1;

   if ( (ret = ms->data_nodes->put(ms->data_nodes, 
		   &key, &val, 0 )) != 0 )
   {
       debug("Put to data_nodes fail:%s",db_strerror(ret));
       POMME_LOG_ERROR("Put to data_nodes fail",ms->ms_logger);
       if( re != NULL )
       {
	   pomme_data_distroy(&re);
       }
       pomme_data_init(&re, POMME_META_INTENAL_ERROR);
   }

e_xit:
    return re;

}

int ms_start(pomme_ms_t *ms)
{
    assert( ms != NULL);
    int ret  = 0;
    if( (ret = ms->rpcs.start(&ms->rpcs) ) != 0 )
    {
	POMME_LOG_ERROR("RPC SERVER start error",ms->ms_logger);
	goto err;
    }
err:
    return ret;
}
int ms_stop(pomme_ms_t *ms)
{
    ms->rpcs.stop(&ms->rpcs);
    stop_logger();
    return 0;
}
int object_dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2)
{
    return -1;
}
int pomme_map_ds_group(const char *path)
{
    return 0;
}
int ms_create_object(pomme_ms_t *ms, u_int64 *id)
{
    uuid_generate_time(id);
    return 0;
}
int  ms_create_ds(pomme_ms_t *ms, u_int32 *id, u_int32 *group)
{
    return 0;
}
