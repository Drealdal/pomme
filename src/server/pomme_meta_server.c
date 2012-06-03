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

int object_dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2);

static const int create_file_arg_num = 2; 
static const int read_file_arg_num = 1; 
static const int write_file_arg_num = 4;
static const int stat_file_arg_num = 1;
static const int heart_beat_arg_num = 1;
static const int get_ds_arg_num = 1;
static const int all_ds_arg_num = 0;
DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE);
DEF_POMME_RPC_FUNC(POMME_META_READ_FILE);
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE);
DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE);
DEF_POMME_RPC_FUNC(POMME_META_HEART_BEAT);

DEF_POMME_RPC_FUNC(POMME_META_GET_DS);
DEF_POMME_RPC_FUNC(POMME_META_ALL_DS);

static int ms_register_funcs(pomme_ms_t *ms);


int pomme_ms_init(pomme_ms_t *ms,
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

    if( ( ret = init_log() ) != 0 )
    {
	debug("Init log error");
    }
    ms->logger = create_logger(log_level, "Meta_Server");
    debug("Hehe");

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
    
    if( ( ret = ms->meta_db->set_flags(ms->meta_db,db_flags))!= 0)
    {
	debug("Set metadb flags failure:%s",db_strerror(ret));
	goto meta_db_err;
    }
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


    if( pthread_mutex_init(&ms->lmutex,NULL) != 0 )
    {
	debug("Meta server  init Mutex Error\n");
	goto data_lock_err;
    }

    ms->start = &ms_start;
    ms->POMME_META_CREATE_FILE = &POMME_META_CREATE_FILE;
    ms->POMME_META_READ_FILE = &POMME_META_READ_FILE;
    ms->POMME_META_WRITE_FILE = &POMME_META_WRITE_FILE;
    ms->POMME_META_STAT_FILE = &POMME_META_STAT_FILE;
    ms->POMME_META_HEART_BEAT = &POMME_META_HEART_BEAT;
    ms->POMME_META_GET_DS = &POMME_META_GET_DS;
    ms->POMME_META_ALL_DS = &POMME_META_ALL_DS;

    ms->get_ds_group = &pomme_map_ds_group;
    
    debug("Before rpcs_init");


    if( (ret = pomme_rpcs_init( &ms->rpcs,ms, max_thread,max_waiting,
		    1,POMME_META_RPC_PORT) ) != 0)
    {
	debug("init thread fail");
	goto rpcs_err;
    }
    ms_register_funcs(ms);
    return ret;
data_lock_err:
    ms->lock_manager->close(ms->lock_manager, 0 );
data_nodes_err:
    ms->meta_db->close(ms->meta_db, 0);
rpcs_err:
sdb_err:
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
    arg[0].size = -1;
    arg[1].size = sizeof(int);
    ms->rpcs.func_register(&ms->rpcs,POMME_META_CREATE_FILE_S,ms->POMME_META_CREATE_FILE,
	    create_file_arg_num,arg);
    // read file

    arg = malloc( sizeof(pomme_data_t)*read_file_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*read_file_arg_num);
    arg[0].size = -1;
    ms->rpcs.func_register(&ms->rpcs,POMME_META_READ_FILE_S,ms->POMME_META_READ_FILE,
	    read_file_arg_num,arg);
    // stat file
    arg = malloc( sizeof(pomme_data_t)*stat_file_arg_num);
    memset(arg,0,sizeof(pomme_data_t)*stat_file_arg_num);
    arg[0].size = -1;
    ms->rpcs.func_register(&ms->rpcs,POMME_META_STAT_FILE_S,ms->POMME_META_STAT_FILE,
	    stat_file_arg_num,arg);
    // write file
    arg = malloc( sizeof(pomme_data_t)*write_file_arg_num); 
    memset(arg, 0, sizeof(pomme_data_t)*write_file_arg_num);

    arg[0].size = -1;
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
    
    return 0;
}

DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE)
{
    assert( n == create_file_arg_num );
    assert( extra != NULL);
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    char *path = (char *)arg[0].data;
    int mode = *(int *)arg[1].data;
    return pomme_create_file(ms, path, mode);
}

DEF_POMME_RPC_FUNC(POMME_META_READ_FILE)
{
    assert( n== read_file_arg_num );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    char *path = (char *)arg[0].data;

    return pomme_read_file(ms,path);

}
DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE)
{
    assert( n == stat_file_arg_num );
    assert( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *)extra;
    char *path = (char *)arg[0].data;

    return pomme_stat_file(ms, path);
}
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE)
{
    assert( n == write_file_arg_num );
    assert ( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *) extra;
    char *path = ( char *) arg[0].data;
    uuid_t id;
    memcpy(id, arg[1].data, sizeof(uuid_t));
    u_int64 off= *(u_int64 *)arg[1].data;
    u_int64 len = *(u_int64 *)arg[2].data; 

    return pomme_write_file( ms, path, id, off, len );

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
    u_int32 id = *(u_int32 *)arg[1].data;

    return pomme_get_ds(ms,id);
}
DEF_POMME_RPC_FUNC(POMME_META_ALL_DS)
{
    assert( n == all_ds_arg_num ); 
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *) extra;

    return pomme_all_ds(ms);
}
