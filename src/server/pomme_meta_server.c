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
static const int read_file_arg_num = 3; 
static const int write_file_arg_num = 3;
static const int stat_file_arg_num = 1;
static const int heart_beat_arg_num = 1;
DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE);
DEF_POMME_RPC_FUNC(POMME_META_READ_FILE);
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE);
DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE);
DEF_POMME_RPC_FUNC(POMME_META_HEART_BEAT);

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
    if( ( ret = init_log() ) != 0 )
    {
	debug("Init log error");
    }
    ms->ms_logger = create_logger(log_level, "Meta_Server");
    debug("Hehe");

    if( (ret = pomme_hash_int_longlong(hash_size,&ms->ds) ) < 0 )
    {
	debug("init hash error");
	POMME_LOG_ERROR("Meta server hash init failure",ms->ms_logger);
	goto hash_err;
    }
    debug("Hehe");

    if( ( ret = db_env_create(&ms->env, 0) ) != 0 )
    {
	debug("create env error:%s",db_strerror(ret));
	POMME_LOG_ERROR("Meta Server create env error",ms->ms_logger);
	goto env_err;
    }
    /*  open env */
    debug("Hehe");
    
    if( ( ret = ms->env->open(ms->env, POMME_META_ENV_HOME,env_o_flags,
		    env_o_mode) ) != 0 )
    {
	debug("open db env error:%s",db_strerror(ret));
	goto env_err;
    }
    debug("Hehe");

    if( ( ret = db_create(&ms->meta_db, ms->env, 0) ) != 0 )
    {
	debug("ms->meta_db create error");
	POMME_LOG_ERROR("Meta Server Create meta db hanle fail",
		ms->ms_logger);
	goto meta_db_err;
    }
    debug("Hehe");
    /* open metadb*/

    o_mdb_flags |= DB_CREATE;
    o_mdb_flags |= DB_THREAD;

    db_flags |= DB_DUPSORT;
    
    if( ( ret = ms->meta_db->set_flags(ms->meta_db,db_flags))!= 0)
    {
	debug("Set metadb flags failure:%s",db_strerror(ret));
	goto meta_db_err;
    }

    if( ( ret = ms->meta_db->set_dup_compare(
		    ms->meta_db,&object_dup_cmp)) != 0)
    {
	debug("Set object_dup_cmp failure:%s",db_strerror(ret));
	goto meta_db_err;
    }

    /* not using txn */
    if( ( ret = ms->meta_db->open(ms->meta_db,
		    NULL, POMME_META_FILE,POMME_META_NAME,DB_BTREE,o_mdb_flags,0664)) != 0)
    {
	debug("Open file failure:%s",db_strerror(ret));
	goto meta_db_err;
    } 

    ms->start = &ms_start;
    ms->POMME_META_CREATE_FILE = &POMME_META_CREATE_FILE;
    ms->POMME_META_READ_FILE = &POMME_META_READ_FILE;
    ms->POMME_META_WRITE_FILE = &POMME_META_WRITE_FILE;
    ms->POMME_META_STAT_FILE = &POMME_META_STAT_FILE;
    ms->POMME_META_HEART_BEAT = &POMME_META_HEART_BEAT;

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
    arg[1].size = sizeof(u_int64);
    arg[2].size = sizeof(u_int64);
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
    arg[0].size = sizeof(u_int64);
    arg[0].size = sizeof(u_int64);

    ms->rpcs.func_register(&ms->rpcs, POMME_META_WRITE_FILE_S, ms->POMME_META_WRITE_FILE,
	    write_file_arg_num, arg);
    //heart beat
    arg = malloc( sizeof(pomme_data_t)*heart_beat_arg_num);
    memset(arg, 0, sizeof(pomme_data_t)*heart_beat_arg_num);
    arg[0].size = sizeof(pomme_hb_t);

    ms->rpcs.func_register(&ms->rpcs, POMME_META_HEART_BEAT_S, ms->POMME_META_HEART_BEAT,
	    heart_beat_arg_num, arg);

    return 0;
}

DEF_POMME_RPC_FUNC(POMME_META_CREATE_FILE)
{
    assert( n == 2 );
    assert( extra != NULL);
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    char *path = (char *)arg[0].data;
    int mode = *(int *)arg[1].data;
    return pomme_create_file(ms, path, mode);
}

DEF_POMME_RPC_FUNC(POMME_META_READ_FILE)
{
    assert( n== 3 );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *)extra;

    char *path = (char *)arg[0].data;
    u_int64 off= *(u_int64 *)arg[1].data;
    u_int64 len = *(u_int64 *)arg[2].data; 

    return pomme_read_file(ms,path, off, len);

}
DEF_POMME_RPC_FUNC(POMME_META_STAT_FILE)
{
    assert( n == 1 );
    assert( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *)extra;
    char *path = (char *)arg[0].data;

    return pomme_stat_file(ms, path);
}
DEF_POMME_RPC_FUNC(POMME_META_WRITE_FILE)
{
    assert( n == 3 );
    assert ( extra != NULL );

    pomme_ms_t *ms = (pomme_ms_t *) extra;
    char *path = ( char *) arg[0].data;
    u_int64 off= *(u_int64 *)arg[1].data;
    u_int64 len = *(u_int64 *)arg[2].data; 

    return pomme_write_file( ms, path, off, len );

}

DEF_POMME_RPC_FUNC(POMME_META_HEART_BEAT)
{
    assert( n == 1 );
    assert( extra != NULL );
    pomme_ms_t *ms = (pomme_ms_t *) extra;
    pomme_hb_t *hb = (pomme_hb_t *) arg[1].data;

    return pomme_heart_beat(ms,hb);
}
