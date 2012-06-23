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
#include "pomme_file.h"
#include "pomme_meta.h"
/**
 * @brief pomme_create_inode 
 *
 * @param ms
 * @param pinode the inode of the parrent dir
 * @param mode
 *
 * @return 
 */
pomme_data_t * pomme_get_inode(pomme_ms_t *ms,
	u_int64 pinode,
	char *name,
	int create)
{
    int ret = 0;
    pomme_data_t *re = NULL ; 
    char id[POMME_PATH_MAX];
    u_int64 inode;

    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    snprintf(id, POMME_PATH_MAX - 1, "%llu/%s",pinode,name);
    u_int64 rinode;

    key.size = pomme_strlen(id);
    key.data = (void *)id;

    val.data = &rinode;
    val.flags |= DB_DBT_USERMEM;
    val.ulen = sizeof(u_int64);

    if( ( ret = ms->imap->get(ms->imap,NULL,&key, &val,0 )) != 0 )
    {
	if( create == 0 )
	{
	    pomme_data_init(&re, POMME_META_FILE_NOT_FOUND);
	    goto ret;
	}
    }else{
	inode = rinode;
	goto ok;	
    }

    if( ( ret = pomme_get_id(ms,&inode)) < 0 )
    {
       	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto ret;
    }
    memset(&val,0,sizeof(DBT));
    val.size = sizeof(u_int64);
    val.data = &inode;

    int flags = 0;
    flags |= DB_NOOVERWRITE;

    debug("Before put");
    if( ( ret = ms->imap->put(ms->imap, NULL, 
		    &key, &val, flags)) != 0 )
    {
	debug("duplicate file");
	pomme_data_init(&re,POMME_META_FILE_EXIST);
	goto ret;
    }
ok:

    pomme_data_init(&re,sizeof(u_int64));
    memcpy(re->data, &inode, sizeof(pomme_file_t));

ret:
    return re;
}
pomme_data_t *pomme_create_file(
	pomme_ms_t *ms,
	u_int64 inode,
	int mode)
{
    int ret = 0;
    pomme_data_t *re = NULL ; 

    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    key.size = sizeof(u_int64); 
    key.data = (void *)&inode;

    pomme_file_t file;
    memset(&file, 0 , sizeof(pomme_file_t));
    time(&file.c_time);
    
    file.acl = mode;
    file.dsgroup = ms->get_ds_group(inode);

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
    pomme_data_init(&re,sizeof(pomme_file_t));
    memcpy(re->data, &file, sizeof(pomme_file_t));

ret:
    return re;
}


pomme_data_t *pomme_read_file(pomme_ms_t *ms, u_int64 inode)
{
    int  ret = 0, i;
    pomme_data_t * re = NULL;

    DB *pdb = ms->meta_db;
    DBC *dbc = NULL;
    if ( ( ret = pdb->cursor(pdb, NULL, &dbc,
		    0) ) != 0 )
    {
	debug("Create cursor fail:%s",db_strerror(ret));
	POMME_LOG_ERROR("Create cursor fail",ms->logger);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto e_exit;
    }
    pomme_data_init(&re, sizeof(pomme_file_t) );// the length of the return object will re extrend
    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    key.data = &inode;
    key.size = sizeof(u_int64); 

    key.flags |= DB_DBT_READONLY;

    val.ulen = sizeof(pomme_file_t); 
    val.flags |= DB_DBT_USERMEM;
    val.data = re->data;

    if( ( ret = dbc->get(dbc, &key, &val, DB_SET)) != 0 )
    {
	re->size = POMME_META_FILE_NOT_FOUND;
	goto e_exit;
    }
    db_recno_t count;
    if ( (ret = dbc->count(dbc, &count, 0) ) != 0 )
    {
	POMME_LOG_INFO("Count failure",ms->logger);
	debug("count dbc error:%s",db_strerror(ret));
	re->size = POMME_META_INTERNAL_ERROR;
	goto e_exit;
    }
    int tSize = sizeof(ms_object_t)*(count-1);
    if ( (ret = pomme_data_extend(re, tSize) ) != 0 )
    {
	POMME_LOG_INFO("Extend data Error",ms->logger);
	re->size = POMME_META_INTERNAL_ERROR;
	goto e_exit;
    }
    ms_object_t *pObject = re->data + sizeof(pomme_file_t);
    for( i = 1 ; i < count ; i++ )
    {
	val.ulen = sizeof(ms_object_t);
	val.data = pObject;
	if ( (ret = dbc->get(dbc, &key,&val, DB_NEXT)) != 0 )
	{
	    debug("Get db error:%s",db_strerror(ret));
	    POMME_LOG_ERROR("Read DB ERROR",ms->logger);
	    re->size = POMME_META_INTERNAL_ERROR;
	    goto e_exit;
	}
    }

   //TODO 
e_exit:
    return re; 
} 

pomme_data_t  *pomme_write_file(pomme_ms_t *ms,
	u_int64 inode,
	uuid_t id, 
	u_int64 off, 
	u_int64 len)
{
    int ret = 0;
    pomme_data_t * re = NULL;
    ms_object_t ob;
    
    uuid_copy(ob.id, id);
    ob.off = off;
    ob.len = len;

    DBT key,val;
    memset(&key, 0 , sizeof(DBT));
    memset(&val, 0 , sizeof(DBT)); 

    key.size = sizeof(u_int64);
    key.data = &inode;

    val.size = sizeof(ms_object_t);
    val.data = &ob;

    DBC *dbc = NULL;
    if ( (ret = ms->meta_db->cursor(ms->meta_db , NULL ,
		    &dbc , 0 )) != 0 )
    {
	debug("Get cursor fail:%s",db_strerror(ret));
	pomme_data_init(&re,POMME_META_INTERNAL_ERROR);
	POMME_LOG_ERROR("Create cursor Error",ms->logger );
	goto err;
    }	

    int put_flag = 0;
    put_flag |= DB_KEYLAST;

   if ( ( ret =  dbc->put(dbc, &key, &val,
		   put_flag) ) != 0 )
   {
       debug("Put to Db fail");
       pomme_data_init(&re, POMME_META_INTERNAL_ERROR);

       POMME_LOG_ERROR("Write DB Error",ms->logger);
       goto err;
   }

   pomme_data_init(&re,0);
   return re;
err:
    return re;
}

pomme_data_t *pomme_stat_file(pomme_ms_t *ms, u_int64 inode)
{
    int ret = 0;
    pomme_data_t *re = NULL;
    DBT key, val;
    
    pomme_data_init(&re, sizeof(pomme_file_t));

    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    key.size = sizeof(u_int64);
    key.data = &inode; 

    val.ulen = sizeof(pomme_file_t);
    val.data = re->data; 
    val.flags |= DB_DBT_USERMEM; 

    DB *pdb = ms->meta_db;

    if( ( ret = pdb->get(pdb,NULL, &key,&val,0)) != 0 )
    {
	debug("Read Db Error:%s",db_strerror(ret));
	pomme_data_distroy(&re);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto e_exit;
    }
e_exit:
    return re;
}
pomme_data_t *pomme_heart_beat(pomme_ms_t *ms, pomme_hb_t *hb)
{
    int ret = 0;
    assert( NULL != hb);
    assert( NULL != ms);

    pomme_data_t *re = NULL;
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
    key.data = &hb->myid;

    val.size = sizeof(pomme_hb_t) ;
    val.data = (u_int32 *)hb ;

    debug("OK");
      if ( (ret = ms->data_nodes->put(ms->data_nodes, 
		    NULL,&key, &val, 0 )) != 0 )
    {
	debug("Put to data_nodes fail:%s",db_strerror(ret));
	POMME_LOG_ERROR("Put to data_nodes fail",ms->logger);
	if( re != NULL )
	{
	    pomme_data_distroy(&re);
	}
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
    }
    if( re == NULL )
    {
	pomme_data_init(&re,0);
    }

    return re;
}

pomme_data_t *pomme_get_ds(pomme_ms_t *ms, u_int32 id)
{
    int ret = 0;
    pomme_data_t *re = NULL;

    DBT key, val;
    memset(&key, 0, sizeof(key));
    memset(&val, 0, sizeof(val));

    pomme_hb_t hb;
    key.size = sizeof(u_int32);
    key.data = (void *)&id;


    val.ulen = sizeof(pomme_hb_t); 
    val.flags |= DB_DBT_USERMEM;
    val.data = &hb;

    //int
    if( ( ret = ms->data_nodes->get(ms->data_nodes, NULL,
		    &key, &val, 0 )) != 0)
    {
	if( ret == DB_NOTFOUND )
	{
	    debug("Data node with id %u not found", id);
	    pomme_data_init(&re, POMME_META_DATA_NODE_NOTFOUND);
	    goto e_xit;
	}
	debug("GET data nodes info failure:%s",db_strerror(ret));
	POMME_LOG_ERROR("get data nodes info failure",ms->logger);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
    }
    pomme_data_init(&re, sizeof(ds_node));
    ds_node *p = re->data;
    p->ip = hb.ip;
    p->port = hb.port;

e_xit:
    return re; 
}
pomme_data_t *pomme_all_ds(pomme_ms_t *ms)
{
    pomme_data_t *re = NULL;
    pomme_data_init(&re, POMME_RPC_NOT_IMPL);
    return re;
}
/*  local method */
pomme_data_t * pomme_lock(pomme_ms_t *ms, u_int64 inode,int interval)
    // time_t *expire)
{
    int ret = 0;
    pomme_data_t *re = NULL;
    DBT key,val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));
    pomme_lock_t lock;
    key.size = sizeof(u_int64);
    key.data = &inode;
    
    val.size = sizeof(pomme_lock_t);
    val.data = &lock;
    val.flags |= DB_DBT_USERMEM;

    DB *db = ms->lock_manager;

    pthread_mutex_lock(&ms->lmutex); 
    if( ( ret =db->get(db, NULL, &key, &val,0) ) == 0)
    {
	if( time(NULL) > lock.wt + ms->lock_time)
	{
	    lock.wt = time(NULL) + interval;

	    if( ( ret = db->put(db, NULL, &key, &val, 0) ) != 0 )
	    {
		debug("Write Db fail:%s",db_strerror(ret));
		pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
		goto err;
	    }
	    pomme_data_init(&re,sizeof(int));
	    *(int *)re->data = lock.wt;
	}
    }
err:
    pthread_mutex_unlock(&ms->lmutex);
    return re;
}
pomme_data_t *pomme_extend_lock(pomme_ms_t *ms, 
	u_int64 inode,
	time_t previous,
       	time_t interval)
{
   int ret = 0;
   pomme_data_t *re = NULL;

   DBT key,val;
   memset(&key, 0, sizeof(DBT));
   memset(&val, 0, sizeof(DBT));

   pomme_lock_t lock;
   lock.wt = previous;

   key.size = sizeof(u_int64);
   key.data = (void *)&inode;
   val.size = sizeof(pomme_lock_t);

   val.data = &lock;
   val.flags |= DB_DBT_USERMEM;
   
   u_int32 flags = DB_GET_BOTH;

   time_t cur = time(NULL);
   if( cur > previous )
   {
       debug("Expired lock");
       POMME_LOG_ERROR("Expired lock",ms->logger);
       pomme_data_init(&re, POMME_LOCK_EXPIRE);
       goto err;
   }

   DB *db = ms->lock_manager;
   if( ( ret = db->get(db, NULL, &key, &val, flags) ) != 0 )
   {
       debug("Read Lock info failure");
       POMME_LOG_ERROR("Read Lock info fail",ms->logger);
       pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
       goto err;
   } 

   lock.wt = time(NULL) + interval;
   if( ( ret = db->put(db, NULL, &key, &val, 0) ) != 0 )
   {

       debug("Put lock info failure");
       POMME_LOG_ERROR("write Lock info fail",ms->logger);
       pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
       goto err;
   }
   pomme_data_init(&re,sizeof(int));
   *(int *)re->data = lock.wt;
err:
   return re;
}

pomme_data_t * pomme_release_lock(pomme_ms_t *ms, 
	u_int64 inode, time_t previous)
{
    int ret = 0;
    pomme_data_t *re = NULL;
    DBT key,val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    pomme_lock_t lock;
    lock.wt = previous;

    key.size = sizeof(u_int64);
    key.data = (void *)&inode;

    val.size = sizeof(pomme_lock_t);
    val.data = &lock;
    val.flags |= DB_DBT_USERMEM;

    u_int32 flags = DB_GET_BOTH;

    DB *db = ms->lock_manager;
    if( ( ret = db->get(db, NULL, &key, &val, flags) ) != 0 )
    {
	debug("Read Lock info failure");
	POMME_LOG_ERROR("Read Lock info fail",ms->logger);
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto err;
    }
    if( time(NULL) > previous ){
	pomme_data_init(&re, POMME_META_INTERNAL_ERROR);
	goto err;
    }
    if( ( ret = db->del(db, NULL, &key, 0) ) != 0 )
    {
	debug("Del Lock info failure");
	POMME_LOG_ERROR("Del Lock info fail",ms->logger);
	pomme_data_init(&re,POMME_LOCK_EXPIRE); 
	goto err;
    }
    pomme_data_init(&re, 0);
err:
    return re;
}

int ms_start(pomme_ms_t *ms) {
    assert( ms != NULL);
    int ret  = 0;
    debug("Before Start rpcs");
    if( (ret = ms->rpcs.start(&ms->rpcs) ) != 0 )
    {
	POMME_LOG_ERROR("RPC SERVER start error",ms->logger);
	goto err;
    }
    debug("Start Success");
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
int pomme_map_ds_group(u_int64 inode)
{
    return 0;
}
int ms_create_object(pomme_ms_t *ms, uuid_t id)
{
    uuid_generate_time(id);
    return 0;
}
int  ms_create_ds(pomme_ms_t *ms, u_int32 *id, u_int32 *group)
{
    *id = 1;
    *group = 1;
    return 0;
}

int pomme_get_id(pomme_ms_t *ms,
	u_int64 *id)
{

    int ret = 0;
    assert( ms != NULL );
    u_int64 mid = ms->myid;
    u_int64 idr = mid << 33;
    pthread_mutex_lock(&ms->imutex);

    ret = fb_set_first0_1(&ms->inodes); 

    pthread_mutex_unlock(&ms->imutex);

    if( ret < 0 )
    {
	return -1;
    }
    *id = idr + ret;
    return 0;
}

int pomme_unset_id(pomme_ms_t *ms, 
	u_int64 id)
{
    int ret = 0;
    int idr = id%((long long)1 << 33 );

    pthread_mutex_lock(&ms->imutex);

    ret = fb_setn1_0(&ms->inodes, idr); 

    pthread_mutex_unlock(&ms->imutex);

    return ret;
}
