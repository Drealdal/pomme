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
    return re; 
} 

pomme_data_t  *pomme_write_file(pomme_ms_t *ms, const char *path, u_int64 off, u_int64 len)
{
    pomme_data_t * re = NULL;
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
    return 1;
}
int pomme_map_ds_group(const char *path)
{
    return 0;
}
