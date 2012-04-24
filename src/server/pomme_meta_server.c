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

int object_dup_cmp(DB *db,const DBT*dbt1, const DBT* dbt2);
int pomme_ms_init(pomme_ms_t *ms,
	pomme_log_level_t log_level,
	int hash_size)
{
    int ret = 0;
    /* flags to open metadb*/
    unsigned int o_mdb_flags = 0, db_flags=0;
    assert( ms != NULL );
    if( ( ret = init_log() ) != 0 )
    {
	debug("Init log error");
    }
    ms->ds_logger = create_logger(log_level, "Meta_Server");

    if( (ret = pomme_hash_int_longlong(hash_size,&ms->ds) ) < 0 )
    {
	POMME_LOG_ERROR("Meta server hash init failure",ms->ds_logger);
	debug("init hash error");
	goto hash_err;
    }

    if( ( ret = db_env_create(&ms->env, 0) ) != 0 )
    {
	POMME_LOG_ERROR("Meta Server create env error",ms->ds_logger);
	debug("create env error:%s",db_strerror(ret));
	goto env_err;
    }

    if( ( ret = db_create(&ms->meta_db, ms->env, 0) ) != 0 )
    {
	POMME_LOG_ERROR("Meta Server Create meta db hanle fail",
		ms->ds_logger);
	debug("ms->meta_db create error");
	goto meta_db_err;
    }
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


sdb_err:
meta_db_err:
env_err:
    pomme_hash_distroy(&ms->ds);
hash_err:
    stop_logger();

    return ret;
}
