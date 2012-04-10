/*
 * =====================================================================================
 *
 *       Filename:  pomme_data_server.c
 *
 *    Description:  the implement of the pomme data server
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:15:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_msg.h"
#include "pomme_storage.h"
#include "pomme_protocol.h"
#include "pomme_data_server.h"

#define MAX_PENDING 100
#define MAX_CLIENTS 1000
#define PACKAGE_LENGTH 1024

extern int stop_log;

int pomme_env_init(pomme_env_t *env,
	unsigned int c_flags,
	unsigned int o_flags,
	char *home,
	char *meta_file,
	char *sfile_file,
	int mode)
{
    int ret = 0;
    assert( env != NULL );
    assert( home != NULL );
    
/*
 * cp ? or something
 * it may not be safe doing like this,
 * if the sfile is auto
 */
    env->sfile_file = sfile_file;
    env->meta_file = meta_file;

    ret = pthread_mutex_init(&env->mutex, NULL);

    if( ret != 0 )
    {
	debug("init mutex error");
	goto mutex_err;
    }

    ret = db_env_create( &env->db_env,c_flags); 

    if( ret < 0 )
    {
	debug("create db env error:%s",db_strerror(ret));
	goto env_err;
    }

   ret =  env->db_env->open(env->db_env, home, o_flags, mode);
   if( ret != 0 )
   {
       debug("open db env error: %s",db_strerror(ret));
       goto open_err;
   }

   ret = db_create( &env->db_meta, env->db_env, 0);

   if( ret != 0 )
   {
       debug("Create Db meta fail: %s",db_strerror(ret));
       goto create_db_err;
   }

   DB_ENV * pEnv = env->db_env;
   DB_TXN *tid;

   /*
    * No to thread will do the write operation
    * on the same record
    *
    */
   //TODO consider the flags
   ret = pEnv->txn_begin(pEnv, NULL, &tid,
	   DB_READ_COMMITTED);
   if( ret != 0 )
   {
       debug("txn_begin error");
       goto create_db_err;
   }

   DB *dbp = env->db_meta;

   if(( ret = dbp->open(dbp,tid,env->meta_file,NULL,DB_BTREE,DB_CREATE,0664))!=0)
   {
       dbp->err(dbp,ret,"open failed%s",env->meta_file);
       goto create_db_err;
   }
   
   ret = db_create( &env->sfile, env->db_env, 0 );
   if( ret != 0 )
   {
       debug("Create Db sfile fail: %s",db_strerror(ret));
       goto create_sfile_fail;
   }
   dbp = env->sfile;
   dbp->set_re_len(dbp, POMME_PATH_MAX);
   dbp->set_re_pad(dbp,0x0);
   if((ret = dbp->open(dbp, tid, env->sfile_file, NULL,DB_QUEUE,DB_CREATE,0664))!=0)
   {
       debug("open db env->sfile_file %s fail:%s",env->sfile_file,db_strerror(ret));
       goto create_sfile_fail;
   }
   return ret;
 create_sfile_fail:
	
create_db_err:

open_err:
	//TODO FLAGS
	env->db_env->close(env->db_env, 0);
env_err:
mutex_err:
	return ret;


}

int pomme_env_distroy(pomme_env_t *env)
{
    int ret = 0;

    env->db_env->close(env->db_env, DB_FORCESYNC);
    return ret;
}

int pomme_ds_init( pomme_ds_t *ds, 
	char *home,
	char *meta_file,
	char *sfile_file,
	unsigned int env_c_flags,
	unsigned int env_o_flags,
	int env_mode)
{
    int ret = 0;
    assert( ds != NULL); 
    memset( ds , 0, sizeof(ds));

    ret = pomme_env_init(&ds->env, 
	    env_c_flags,env_o_flags,
	    home,
	    meta_file,
	    sfile_file,
	    env_mode);
    if( ret < 0 )
    {
	debug("data server env init failure");
	goto err;
    }
	
    ret  = pomme_hash_int_int( 100,&ds->storage_file);
    if( ret < 0 )
    {
	debug("init hash fail");
	goto hash_err;
    }
    ds->cur_storage_id = -1;
    ds->data_home =home;
    debug("cur:%d",ds->cur_storage_id);
    ret = get_storage_files(ds->data_home, ds->storage_file,
	    &(ds->cur_storage_id),&(ds->cur_storage_fd));
    if( ret < 0 )
    {
	debug("get storage file fail");
	goto storage_err;
    }

    init_log();
    ds->ds_logger = create_logger(POMME_LOG_MAX,"data_server");
    if( NULL == ds->ds_logger )
    {
	debug("create logger failure");
	goto logger_err;
    }
    POMME_LOG_INFO("Data Server data structure init success",ds->ds_logger);

    if( ds->cur_storage_id == -1 )
    {
	//TODO  TXN
	ret = create_storage(ds->env.sfile, NULL, ds->data_home,
		&ds->cur_storage_id, &ds->cur_storage_fd);
	if( ret < 0 )
	{
	    debug("create storage fail");
	    goto storage_err;
	}
	debug("%d",ds->cur_storage_id);
    }
    return ret;
storage_err:
logger_err:
    pomme_hash_distroy(&ds->storage_file);
hash_err:
    pomme_env_distroy(&ds->env);
err:
    return ret;

}


int pomme_ds_distroy( pomme_ds_t *ds )
{
    int ret = 0;
    POMME_LOG_INFO("STOP DATA SERVER",ds->ds_logger);
    stop_log = 1;
    pomme_env_distroy(&ds->env);
    pomme_hash_distroy(&ds->storage_file);
    while(stop_log == 1)
    {
	sleep(1);
    }

    return ret;
}
int get_storage_files(char *path, pomme_hash_t *storage,
	int *cur_id,
	int *cur_fd)
{
   int ret= 0, len ;
   struct stat statbuf;
   struct dirent *dirp = NULL;
   DIR *dp;
   int fd;
   char tpath[POMME_PATH_MAX];
   debug("before: %d",*cur_id);

   if( (ret = lstat( path ,&statbuf))<0 )
   {
       debug("lstat %s failure: %s", path,strerror(ret));
       ret = POMME_LOCAL_DIR_ERROR;
       goto err;
   }
   
   if( S_ISDIR( statbuf.st_mode) == 0 )
   {
       debug("%s is not a dir",path);
       ret = POMME_LOCAL_DIR_ERROR;
       goto err;
   }
   
   if( (dp = opendir( path))== NULL )
   {
       debug("opendir %s fail",path);
       ret = POMME_LOCAL_DIR_ERROR;
       goto err;
   }
   strcpy(tpath, path);
   len = strlen(path);
   while( len>0 &&tpath[len-1] == '/' )
   {
       tpath[len-1]='0';
       len--;
   }
   pomme_ds_head_t head;
   while( (dirp = readdir( dp)) != NULL )
   {
       if(( strcmp( dirp->d_name, ".") == 0 )||
		 (  strcmp(dirp->d_name,"..") ==0 )
	  )
	       {
		   continue;
	       }
       snprintf(tpath+len,POMME_PATH_MAX-len,"/%s",dirp->d_name);

       if( (ret = lstat( tpath ,&statbuf))<0 )
       {
	   debug("lstat %s failure: %s", tpath,strerror(ret));
	   ret = 0;
	   continue;
       }

       if( S_ISDIR( statbuf.st_mode) == 0 )
       {
	   // not dir
	   //printf("%s\n",tpath);
	   fd = open(tpath,O_RDWR);
	   if( fd < 0 )
	   {
	       debug("open file %s failure: %d %s",tpath,fd,strerror(fd));
	       continue;
	   }
	  ret = is_file_valid(fd,&head); 

	  if( ret < 0 )
	  {
	      close(fd);
	      ret = 0;
	      debug("%s not valid",tpath);
	      continue;
	  }
	  if( head.status == CUR )
	  {
	      debug("cur %s %d %d\n",tpath, *cur_id,head.id);
	      if( *cur_id == -1 )
	      {
		  *cur_id = head.id;
		  *cur_fd = fd;
	      }else{
		  debug("more than 1 cur storage");
		  ret = POMME_FILE_MULTI_CUR;
		  break;
	      } 
	  }
	  ret = pomme_hash_put_2(storage,&head.id,sizeof(int),&fd,sizeof(int));
	  if( ret < 0 )
	  {
	      debug("put to hash table failure");
	      break;
	  }
	  printf("pair<%d,%d>\n",head.id,fd);

       }else{
	ret =  get_storage_files(tpath,storage,cur_id, cur_fd);
	if( ret < 0 )
	{
	    break;
	}
	   
       }
       
   }


err:
   return ret;

}
int setnonblocking(int sock);

int pomme_put_object(pomme_ds_t *ds,int handle, pomme_protocol_t *pro)
{

    int ret = 0;
    unsigned char buffer[POMME_PACKAGE_SIZE];
    unsigned int flags = 0;

    DBT key,val;
    assert( pro!=NULL );
    pomme_object_t object;
    memset(&object, 0 , sizeof(pomme_object_t));

    object.sfid = ds->cur_storage_id; 
    object.offset = pro->offset;
    object.len = pro->total_len;

    time(&object.time);

    object.ver = 1;

    size_t wl = pro->len,tmlen;

    object.start = -1;
    ret = put_data_2_storage(ds->cur_storage_fd,
	    pro->data,pro->len,&object.start);

    if( ret < 0 )
    {
	debug("write file error:%s",strerror(ret));
    }
    while( wl < pro->total_len)
    {
	int rets = pro->total_len -wl < POMME_PACKAGE_SIZE ? pro->total_len - wl:POMME_PACKAGE_SIZE; 
	ret = pomme_recv(handle,buffer,rets,&tmlen,0);
	if( ret < 0 )
	{
	    debug("recv data error");
	    goto err;
	}
	ret = put_data_2_storage(ds->cur_storage_fd,
		pro->data,tmlen,&object.start);
	if(ret<0)
	{
	    debug("write fail");
	    goto err;
	}
	wl += ret;
    }
    memset(&key,0,sizeof(key));
    memset(&key,0,sizeof(val));
    val.data = &object;
    val.size = sizeof(pomme_object_t);

    key.data = &pro->id;
    key.size = sizeof(pro->id); 
    DB *pdb = ds->env.db_meta;

    debug("Create object:%lld",pro->id);

    ret = pdb->put(pdb, NULL, &key, &val,flags);
    if( ret < 0 )
    {
	debug("put to database fail, %s",db_strerror(ret));
	POMME_LOG_ERROR("write to database fail",ds->ds_logger);
	goto put_err;
    }
    return ret;
    
put_err:
    ftruncate(ds->cur_storage_fd, object.start);

err:	
    return ret;
}
int pomme_get_object(pomme_ds_t *ds,int handle, pomme_protocol_t *pro)
{
    int fd;
    int ret = 0;
    unsigned int flags = 0;
    DBT key,val;

    assert( ds!=NULL );
    assert( pro != NULL );

    memset( &key, 0,sizeof(key));
    memset( &val, 0,sizeof(val));

    pomme_object_t object;
    memset(&object, 0 , sizeof(pomme_object_t));

    key.data = &pro->id;
    key.size = sizeof(pro->id);

    val.data = &object;
    val.ulen = sizeof(pomme_object_t);
    
    val.flags |= DB_DBT_USERMEM; 
    DB *pdb = ds->env.db_meta;

    debug("Get object:%lld",pro->id);

    if( ( ret = pdb->get(pdb, NULL, &key,&val,flags) ) < 0 )
    {
	debug("read err, %s",db_strerror(ret));
	goto err;
    }

    pomme_data_t k,v;
    memset(&k, 0, sizeof(pomme_data_t));
    memset(&v, 0, sizeof(pomme_data_t));


    k.size = sizeof(object.sfid);
    k.data = &object.sfid;

    v.size = sizeof(int); 
    v.data = &fd;

    if( (ret = pomme_hash_get(ds->storage_file, 
	    &k,&v)) < 0 )
    {
	debug("get storage failure");
	goto err;
    }

    debug("sfid:%u fd:%d",object.sfid,fd);
    int l2r = pro->total_len;

    if( pro->total_len > object.len)
    {
	l2r = object.len;
	debug("Too long read:%lld",object.len);
    }

    unsigned char buffer[POMME_PACKAGE_SIZE];
    int first_to_send = l2r > POMME_MAX_PROTO_DATA ? 
	POMME_MAX_PROTO_DATA:l2r;

    debug("seek %d %d %lld",
	    ds->cur_storage_fd,fd,(long long)object.start);
    lseek(fd,object.start,SEEK_SET);

    if( ( ret = read(fd,buffer,first_to_send))
	    < first_to_send)
    {
	debug("read data fail");
	goto err;
    }
    debug("seek");

    pomme_protocol_t spro;
    memset(&spro, 0, sizeof(spro));
    debug("1");

    spro.op = put_data;
    spro.len = first_to_send;
    spro.total_len = l2r;
    spro.id = pro->id;
    spro.offset = pro->offset;
    spro.data = buffer;

    pomme_pack_t *buf = NULL;
    if( ( ret = pack_msg( &spro, &buf) ) < 0 )
    {
	debug("pack msg fail");
	goto err;
    }

    if( ( ret = pomme_send(handle,buf->data,
		    pomme_msg_len((&spro)),flags)) < 0 )
    {
	debug("send error");
	goto err;
    }

    int sent = first_to_send;
    while( sent < l2r )
    {
	int re = read(fd, buffer, POMME_PACKAGE_SIZE);
	if( re < 0 )
	{
	    debug("error:%s",strerror(ret));
	}
	if( (ret = pomme_send(handle, buffer, re, flags) ) < 0 )
	{
	    debug("send error");
	    goto err;
	}else{
	sent += re;
	}
    }
    debug("over: %d",sent);
err:
    return ret;
}
static int handle_request(pomme_ds_t *ds,int handle)
{
    unsigned char buffer[PACKAGE_LENGTH];
    int flags=0, ret = 0;
    size_t r_len = 0;

    ret = pomme_recv(handle, buffer, PACKAGE_LENGTH, &r_len, flags);
    if( ret < 0 )
    {
	debug("recev fail");
	goto err;
    }
   
   pomme_protocol_t pro;
   memset( &pro, 0, sizeof(pomme_protocol_t));

   pomme_pack_t *p_buffer = NULL;
   ret = pomme_pack_create( &p_buffer,
       &buffer,r_len);
   if( ret < 0 )
   {
       debug("create pack fail");
       goto err;
   }

   ret = unpack_msg( &pro, p_buffer);
   if( ret < 0 )
   {
       debug("unpack msg failure");
       goto err;
   }

    pomme_print_proto(&pro,NULL);

   switch( pro.op )
   {
       case put_data:
	   pomme_put_object(ds,handle,&pro);
	   break;
       case get_data:
	   debug("get_data");
	  pomme_get_object(ds,handle,&pro);
	   break;
       default:
	   debug("unknown protocol msg");
	   ret = POMME_UNKNOWN_MSG;
	   break;
   }
   pomme_pack_distroy(&p_buffer);
err:
    return ret;
}

int server(pomme_ds_t *ds)
{
    int ret = 0;
    struct sockaddr_in addr;
    int socid = socket( AF_INET, SOCK_STREAM, 0);	
    if( socid < 0 )
    {
	debug("create socket identifier fail: %s",strerror(socid));
	goto err_exit;
    }

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(POMME_DATA_PORT);

    ret = bind(socid, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if( ret < 0 )
    {
	debug("bind error: %s",strerror(ret));
	goto err_exit;
    }

    if( listen( socid, MAX_PENDING) < 0 )
    {
	debug("listen error");
	goto err_exit;
    }

    int epid = epoll_create(MAX_CLIENTS+1);
    if( epid < 0 )
    {
	debug("epoll create failure");
	goto err_exit;
    }

    struct epoll_event ev, events[MAX_CLIENTS+1];
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = socid;

    if( epoll_ctl(epid, EPOLL_CTL_ADD, socid, &ev) < 0 )
    {
	debug("epoll_ctl sockid fail");
	goto err_exit;
    }

    int nfds = 0, i, conn_sock;
    while(1)
    {
	nfds = epoll_wait(epid, events, MAX_CLIENTS+1, -1);
	if( nfds == -1 )
	{
	    debug("epoll wait failure");
	    goto err_exit;
	}

	for( i = 0; i< nfds; ++i )
	{
	    if( events[i].data.fd == socid )
	    {
		conn_sock = accept(socid, (struct sockaddr *)NULL, NULL);
		if( conn_sock == -1 )
		{
		    debug("accept error");
		    goto err_exit;
		}
		setnonblocking(conn_sock);
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = conn_sock;

		if( epoll_ctl(epid, EPOLL_CTL_ADD, conn_sock, &ev) < 0 )
		{
		    debug("epoll_ctl conn_sock fail");
		    goto err_exit;
		}
	    }else{
		handle_request(ds,conn_sock);
		debug("handle");
	    }
	}
    }


err_exit:
    return -1;
}


