/*
 * =====================================================================================
 *
 *       Filename:  pomme_meta_server.c
 *
 *    Description:  the implement of meta server
 *
 *        Version:  1.0
 *        Created:  03/03/2012 05:47:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"
#include "pomme_log.h"
#include "pomme_meta_server.h"
#include "pomme_meta.h"
#include "pomme_ms_impl.h"

extern int errno;
int object_dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2);

static const int create_file_arg_num = 2; 
static const int create_inode_arg_num = 3;

static const int read_file_arg_num = 1; 
static const int write_file_arg_num = 4;
static const int stat_file_arg_num = 1;
static const int heart_beat_arg_num = 1;
static const int get_ds_arg_num = 1;
static const int all_ds_arg_num = 0;

static const int lock_arg_num = 2;
static const int extend_lock_arg_num = 3;
static const int release_lock_arg_num = 2; 

DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE);
DEF_POMME_RPC_FUNC(POMME_META_GET_INODE);

DEF_POMME_RPC_FUNC(POMME_META_READ_FILE);
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE);

DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE);
DEF_POMME_RPC_FUNC(POMME_META_HEART_BEAT);

DEF_POMME_RPC_FUNC(POMME_META_GET_DS);
DEF_POMME_RPC_FUNC(POMME_META_ALL_DS);

DEF_POMME_RPC_FUNC(POMME_LOCK);
DEF_POMME_RPC_FUNC(POMME_EXTEND_LOCK);

DEF_POMME_RPC_FUNC(POMME_RELEASE_LOCK);
static int ms_register_funcs(pomme_ms_t *ms);
int pomme_ms_init(pomme_ms_t *ms,
	int my_id,
	pomme_log_level_t log_level,
	int env_o_flags,
	int env_o_mode,
	int hash_size,
	int max_thread,
	int max_waiting)
{

    debug("Meta Server init");
    int ret = 0;
    /* flags to open metadb*/
    unsigned int o_mdb_flags = 0, db_flags=0;
    assert( ms != NULL );
    memset(ms, 0, sizeof(pomme_ms_t));


    /*  config init */

    ms->lock_time = POMME_META_FILE_LOCK_TIME;
    ms->myid = my_id;

    if( ( ret = init_log() ) != 0 )
    {
	debug("Init log error");
    }
    ms->logger = create_logger(log_level, "Meta_Server");

    if( (ret = pomme_hash_int_longlong(hash_size,&ms->ds) ) < 0 )
    {
	debug("init hash error");
	POMME_LOG_ERROR("Meta server hash init failure",ms->logger);
	goto hash_err;
    }

    if( ( ret = db_env_create(&ms->env, 0) ) != 0 )
    {
	debug("create env error:%s",db_strerror(ret));
	POMME_LOG_ERROR("Meta Server create env error",ms->logger);
	goto env_err;
    }
    /*  open env */
    
    if( ( ret = ms->env->open(ms->env, POMME_META_ENV_HOME,env_o_flags,
		    env_o_mode) ) != 0 )
    {
	debug("open db env error:%s",db_strerror(ret));
	goto env_err;
    }

    if( ( ret = db_create(&ms->meta_db, ms->env, 0) ) != 0 )
    {
	debug("ms->meta_db create error");
	POMME_LOG_ERROR("Meta Server Create meta db hanle fail",
		ms->logger);
	goto meta_db_err;
    }
    /* open metadb*/

    o_mdb_flags |= DB_CREATE;
    o_mdb_flags |= DB_THREAD;

//    db_flags |= DB_DUPSORT;
      db_flags |= DB_DUP;
    


/*  
    if( ( ret = ms->meta_db->set_dup_compare(
		    ms->meta_db,&object_dup_cmp)) != 0)
    {
	debug("Set object_dup_cmp failure:%s",db_strerror(ret));
	goto meta_db_err;
    }
*/
    /* not using txn */
    if( ( ret = ms->meta_db->open(ms->meta_db,
		    NULL, POMME_META_FILE,POMME_META_NAME,DB_BTREE,o_mdb_flags,0664)) != 0)
    {
	POMME_LOG_ERROR("Sever Open data base fail",ms->logger);
	debug("Open file failure:%s",db_strerror(ret));
	goto meta_db_err;
    } 

    /*  data node manage */
    if( ( ret = db_create(&ms->data_nodes, ms->env, 0 )) != 0 )
    {
	debug("ms->data_nodes create error");
	POMME_LOG_ERROR("Server create db data_nodes fail",ms->logger);
	goto data_nodes_err;
    }

    if( ( ret = ms->data_nodes->open(
		    ms->data_nodes, NULL, POMME_DATA_NODES_FILE, POMME_DATA_NODES_NAME,
		    DB_BTREE, DB_CREATE | DB_THREAD,0664)) != 0 )
    {
	debug("open data nodes db failure:%s",db_strerror(ret));
	POMME_LOG_ERROR("Server open data_nodes database fail",ms->logger);
	goto data_nodes_err;
    }
    /*  lock manager */
    if( ( ret = db_create(&ms->lock_manager, ms->env,0)) != 0 )
    {
	debug("ms->lock_manager create error");
	POMME_LOG_ERROR("Server Create db lock_manager failure",ms->logger);
	goto data_lock_err;
    }

    if( ( ret = ms->lock_manager->open(
		    ms->lock_manager, NULL, POMME_LOCK_MANAGER_FILE,
		    POMME_LOCK_MANAGER_NAME,DB_BTREE,DB_CREATE | DB_THREAD,0664)) != 0)
    {
	debug("open lock_manager db failure:%s",db_strerror(ret));
	POMME_LOG_ERROR("Server open lock manager failure",ms->logger);
	goto data_lock_err;
    }
    /*  inode mapping */

    if( ( ret = db_create(&ms->imap, ms->env,0)) != 0 )
    {
	debug("ms->imap create error");
	POMME_LOG_ERROR("Server Create db imap failure",ms->logger);
	goto imap_err;
    }

    if( ( ret = ms->imap->open(
		    ms->imap, NULL,POMME_IMAP_FILE,
		   POMME_IMAP_NAME,DB_BTREE,DB_CREATE,0664)) != 0 )
    {
	debug("open imap db failure: %s",db_strerror(ret));
	POMME_LOG_ERROR("Server open inode mapping failure",ms->logger);
	goto imap_err;
    }	


    if( pthread_mutex_init(&ms->lmutex,NULL) != 0 )
    {
	debug("Meta server  init Mutex Error\n");
	goto imap_err;
    }


    if( pthread_mutex_init(&ms->imutex,NULL) != 0 )
    {
	debug("Meta server  init Mutex Error\n");
	goto imap_err;
    }
    /*  init inodes */

    if (fb_init(&ms->inodes,max_height(),0,0) < 0 )
    {
	debug("init fast bit failure");
	goto imap_err;
    }

    
    //TODO
    DBC *dbc = NULL;
    if( ( ret = ms->imap->cursor(ms->imap,
		    NULL,&dbc,DB_CURSOR_BULK))!= 0 )
    {
	debug("Get read cursor for ms->imap failure:%s",db_strerror(ret));
	POMME_LOG_ERROR("Server get cursor failure",ms->logger);
	goto imap_err;
    }else{
    }

    DBT key, val;
    memset(&key, 0, sizeof(DBT));
    memset(&val, 0, sizeof(DBT));

    if(ms->myid == 0 )
    {
	fb_setn0_1(&ms->inodes, 0);
    }
    while(( ret = dbc->get(dbc, &key,
		    &val,DB_NEXT)) == 0 )
    {
	u_int64 localid = *(u_int64 *)val.data; 
	int id = localid%((long long)1)<<33;
	fb_setn0_1(&ms->inodes,id);
    }
    debug("Error?:%s\n",db_strerror(ret));


    ms->start = &ms_start;
    ms->POMME_META_CREATE_FILE = &POMME_META_CREATE_FILE;
    ms->POMME_META_GET_INODE = &POMME_META_GET_INODE;

    ms->POMME_META_READ_FILE = &POMME_META_READ_FILE;
    ms->POMME_META_WRITE_FILE = &POMME_META_WRITE_FILE;

    ms->POMME_META_STAT_FILE = &POMME_META_STAT_FILE;
    ms->POMME_META_HEART_BEAT = &POMME_META_HEART_BEAT;

    ms->POMME_META_GET_DS = &POMME_META_GET_DS;
    ms->POMME_META_ALL_DS = &POMME_META_ALL_DS;
    
    ms->POMME_LOCK = &POMME_LOCK;
    ms->POMME_EXTEND_LOCK = &POMME_EXTEND_LOCK;
    ms->POMME_RELEASE_LOCK = &POMME_RELEASE_LOCK;

    ms->get_ds_group = &pomme_map_ds_group;
    
    debug("Before rpcs_init");


    if( (ret = pomme_rpcs_init( &ms->rpcs,ms, max_thread,max_waiting,
		    5,POMME_META_RPC_PORT) ) != 0)
    {
	debug("init thread fail");
	goto rpcs_err;
    }
    ms_register_funcs(ms);
    
    debug("Meta server init over");
    return ret;
imap_err:
    ms->imap->close(ms->imap, 0);
data_lock_err:
    ms->lock_manager->close(ms->lock_manager, 0 );
data_nodes_err:
    ms->meta_db->close(ms->meta_db, 0);
rpcs_err:
//sdb_err:
meta_db_err:
env_err:
    pomme_hash_distroy(&ms->ds);
hash_err:
    stop_logger();

    return ret;
}

static int ms_register_funcs(pomme_ms_t *ms)
{
    // create file
    pomme_data_t *arg = malloc(sizeof(pomme_data_t)*create_file_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*create_file_arg_num);

    arg[0].size = sizeof(u_int64);
    arg[1].size = sizeof(int);

    ms->rpcs.func_register(&ms->rpcs,POMME_META_CREATE_FILE_S,ms->POMME_META_CREATE_FILE,
	    create_file_arg_num,arg);
    // create inode
    arg = malloc(sizeof(pomme_data_t)*create_inode_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*create_inode_arg_num);

    arg[0].size = sizeof(u_int64);
    arg[1].size = -1;
    arg[2].size = sizeof(int);

    ms->rpcs.func_register(&ms->rpcs,POMME_META_GET_INODE_S,ms->POMME_META_GET_INODE,
	    create_inode_arg_num,arg);

  // read file

    arg = malloc( sizeof(pomme_data_t)*read_file_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*read_file_arg_num);

    arg[0].size = sizeof(u_int64);
    ms->rpcs.func_register(&ms->rpcs,POMME_META_READ_FILE_S,ms->POMME_META_READ_FILE,
	    read_file_arg_num,arg);
    // stat file
    arg = malloc( sizeof(pomme_data_t)*stat_file_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*stat_file_arg_num);

    arg[0].size = sizeof(u_int64);
    ms->rpcs.func_register(&ms->rpcs,POMME_META_STAT_FILE_S,ms->POMME_META_STAT_FILE,
	    stat_file_arg_num,arg);
    // write file
    arg = malloc( sizeof(pomme_data_t)*write_file_arg_num); 
    memset(arg, 0, sizeof(pomme_data_t)*write_file_arg_num);

    arg[0].size = sizeof(u_int64);
    arg[1].size = sizeof(uuid_t);

    arg[2].size = sizeof(u_int64);
    arg[3].size = sizeof(u_int64);

    ms->rpcs.func_register(&ms->rpcs, POMME_META_WRITE_FILE_S, ms->POMME_META_WRITE_FILE,
	    write_file_arg_num, arg);
    //heart beat
    arg = malloc( sizeof(pomme_data_t)*heart_beat_arg_num);
    memset(arg, 0, sizeof(pomme_data_t)*heart_beat_arg_num);
    arg[0].size = sizeof(pomme_hb_t);


    ms->rpcs.func_register(&ms->rpcs, POMME_META_HEART_BEAT_S, ms->POMME_META_HEART_BEAT,
	    heart_beat_arg_num, arg);

    // get ds
    arg = malloc( sizeof(pomme_data_t)*get_ds_arg_num);
    memset(arg, 0, sizeof(pomme_data_t)*heart_beat_arg_num);
    arg[0].size = sizeof(u_int32);
    
    ms->rpcs.func_register(&ms->rpcs, POMME_META_GET_DS_S,ms->POMME_META_GET_DS,
	    get_ds_arg_num,arg);
    // all ds 
    arg = NULL;
    ms->rpcs.func_register(&ms->rpcs, POMME_META_ALL_DS_S,ms->POMME_META_ALL_DS,
	    all_ds_arg_num,arg);
    // lock
    arg = malloc( sizeof(pomme_data_t)*lock_arg_num);
    arg[0].size = sizeof(u_int64);
    arg[1].size = sizeof(int);

    ms->rpcs.func_register(&ms->rpcs, POMME_LOCK_S, ms->POMME_LOCK,
	    lock_arg_num,arg);

    // extend lock
    arg = malloc( sizeof(pomme_data_t)*extend_lock_arg_num);

    arg[0].size = sizeof(u_int64);
    arg[1].size = sizeof(int);
    arg[2].size = sizeof(int); 
    ms->rpcs.func_register(&ms->rpcs,  POMME_EXTEND_LOCK_S,
	    ms->POMME_EXTEND_LOCK,extend_lock_arg_num, arg);
    // release lock
    arg = malloc( sizeof(pomme_data_t)*release_lock_arg_num);

    arg[0].size = sizeof(u_int64);
    arg[1].size = sizeof(int);

    ms->rpcs.func_register(&ms->rpcs,  POMME_RELEASE_LOCK_S,
	    ms->POMME_RELEASE_LOCK,release_lock_arg_num, arg);
    
    return 0;
}

DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE)
{
    assert( n == create_file_arg_num );
    assert( extra != NULL);
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    u_int64 inode = *(u_int64 *)arg[0].data;
    int mode = *(int *)arg[1].data;

    return pomme_create_file(ms, inode,mode);
}

DEF_POMME_RPC_FUNC(POMME_META_GET_INODE)
{
    assert( n == create_inode_arg_num );
    assert( extra != NULL);
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    u_int64 pinode = *(u_int64 *)arg[0].data;
    char *name = (char *)arg[1].data;
    int create = *(int *)arg[2].data;

    return pomme_get_inode(ms, pinode, name,create);
}

DEF_POMME_RPC_FUNC(POMME_META_READ_FILE)
{
    assert( n== read_file_arg_num );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    u_int64 inode = *(u_int64 *)arg[0].data;

    return pomme_read_file(ms,inode);

}
DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE)
{
    assert( n == stat_file_arg_num );
    assert( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *)extra;
    u_int64 inode = *(u_int64 *)arg[0].data;

    return pomme_stat_file(ms, inode);
}
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE)
{
    debug("Write file");
    assert( n == write_file_arg_num );
    assert ( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *) extra;
    u_int64 inode = *(u_int64 *)arg[0].data;
    uuid_t id;

    uuid_copy(id, arg[1].data);


    u_int64 off= *(u_int64 *)arg[2].data;
    u_int64 len = *(u_int64 *)arg[3].data; 

    return pomme_write_file( ms, inode, id, off, len );

}
DEF_POMME_RPC_FUNC(POMME_META_HEART_BEAT)
{
    assert( n == heart_beat_arg_num );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *) extra;
    pomme_hb_t *hb = (pomme_hb_t *) arg[0].data;

    return pomme_heart_beat(ms,hb);
}

DEF_POMME_RPC_FUNC(POMME_META_GET_DS)
{
    assert( n == heart_beat_arg_num );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *) extra;
    u_int32 id = *(u_int32 *)arg[0].data;

    return pomme_get_ds(ms,id);
}
DEF_POMME_RPC_FUNC(POMME_META_ALL_DS)
{
    assert( n == all_ds_arg_num ); 
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *) extra;

    return pomme_all_ds(ms);
}
DEF_POMME_RPC_FUNC(POMME_LOCK)
{
    assert( n == lock_arg_num);
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    u_int64 inode = *(u_int64 *)arg[0].data;
    int  inteval = *(int *)arg[1].data;

    return pomme_lock(ms, inode, inteval);
}
DEF_POMME_RPC_FUNC(POMME_EXTEND_LOCK)
{
    assert( n == extend_lock_arg_num );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    u_int64 inode = *(u_int64 *)arg[0].data;
    int previous = *(int *) arg[1].data;
    int inteval = *(int *) arg[2].data;

    return pomme_extend_lock(ms, inode, previous, inteval);

}
DEF_POMME_RPC_FUNC(POMME_RELEASE_LOCK)
{
    assert( n == release_lock_arg_num );
    assert( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *)extra;
    u_int64 inode = *(u_int64 *)arg[0].data;
    int previous = *(int *)arg[1].data;

    return pomme_release_lock(ms, inode, previous);
}
