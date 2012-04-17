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
#include "pomme_meta_server.h"
int pomme_ms_init(pomme_ms_t *ms,
	pomme_log_level_t log_level,
	int hash_size)
{
    int ret = 0;
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

meta_db_err:


env_err:
    pomme_hash_destroy(&ms->ds);
hash_err:
    stop_logger();

    return ret;
}
