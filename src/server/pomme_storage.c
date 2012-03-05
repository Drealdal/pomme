/*
 * =====================================================================================
 *
 *       Filename:  pomme_storage.c
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  01/11/2012 06:22:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "utils.h"
#include "pomme_storage.h"


extern int file_count_strict;

static int get_dir_child_num(char *path, int *count);
static int create_local_file(char *path,size_t id);

int create_storage(DB *db_handle,
	DB_TXN *txnid,
	char *storage_path,
	size_t *id)
{
    int ret = 0, count, len;
    unsigned int flags = 0;
    DBT key,val;

    if( storage_path == NULL )
    {
	storage_path = "./";
    }
    debug("%p %s ",storage_path,storage_path);

    len = strlen(storage_path) +20;
    char fullpath[len];

    debug("%s",storage_path);
    if( ( ret = get_dir_child_num(storage_path, &count) < 0 ))
    {
	debug("Create Storage failure");
	goto err;
    }
    strcpy( fullpath, storage_path);
    char *ptr = fullpath + strlen(storage_path) - 1;

    while(*ptr == '/') ptr--;

    *(ptr+1)='/';
    sprintf(ptr+2, "%d", count);

    if( (ret = create_local_file(fullpath,count) ) < 0 )
    {
    	debug("create local file(%s) error",fullpath);
    	goto err;
    }
    memset(&key, 0, sizeof(key));
    memset(&val, 0, sizeof(val));

    val.data = fullpath;
    val.size = strlen(fullpath) + 1;


    flags |= DB_APPEND;

    ret = db_handle->put(db_handle, txnid, &key, &val, flags);
    if( ret < 0 )
    {
	debug("put to database fail");
    }
    *id = *(unsigned int *)(key.data);

err:
    return ret;
}


/**
 * @brief get_dir_child_num : get the number of the given directory
 *
 * @param path
 *
 * @return: 0 for success , < 0 for failure 
 */
static int get_dir_child_num(char *path,int *num)
{
    int ret , len, file_count = 0;
    struct stat statbuf;
    struct dirent *dirp = NULL;
    DIR *dp;


    if( (ret = lstat(path, &statbuf)) < 0 )
    {
	debug("lstat %s failure: %s",path,strerror(ret));
	ret =  POMME_LOCAL_DIR_ERROR;
	goto err;
    }

    if( S_ISDIR(statbuf.st_mode) == 0 )
    {
	debug("%s is not a dir",path);
	ret = POMME_LOCAL_DIR_ERROR;
	goto err;
    }
	
    len = strlen(path);
    if( path[len-1] != '/' )
    {
	path[len]='/';
	path[len+1] = 0;
    }

    if( ( dp = opendir( path ) ) == NULL )
    {
	debug("open %s fail",path);
	ret =  POMME_LOCAL_DIR_ERROR;
	goto err;
    }

    while( ( dirp = readdir(dp)) != NULL )
    {
	if( (strcmp(dirp->d_name, ".") == 0 )
		|| (strcmp( dirp->d_name, "..") == 0)
	  )
	{
	    continue;
	}
	file_count ++;
    }
    *num =  file_count;
    ret = POMME_SUCCESS;
err:
    return ret;

}
static int create_local_file(char *path,size_t id)
{
    int ret = 0;
    if( (ret = open(path,O_RDONLY)) != -1 )
    {
    	debug("File %s path is already exists",path);
    	ret = POMME_LOCAL_FILE_ERROR;
    	goto err;
    }
    if( ( ret = open(path,O_CREAT) ) < 0 )
    {
    	debug("Create File %s failed",path);
    	ret = POMME_LOCAL_FILE_ERROR;
    	goto err;
    }
    ret =  set_file_head(ret , id);
    if( ret < 0 )
    {
	debug("set file head failure");
	goto err;
    }
err:
   return ret; 
}

int put_data_2_storage(int file_handle,
	void *data,
       	size_t len,
	off_t *start) 
{
    int ret = 0;

    assert(data != NULL);
    assert( len>0 );
/*
 * if the start is set <0 then
 * we should seek the file_handle,
 * else the file is at the end
 */
    if( start < 0 )
    {
	ret  = lseek( file_handle, 0, SEEK_END ); 
	if( ret < 0 )
	{
	    debug("seek file fail , %s",strerror(ret));
	    goto err;
	}
	*start = ret;
    }
    ret = write( file_handle, data, len);
    if( ret < len )
    {
	ftruncate( file_handle, *start);
	debug("write file fail %s", strerror(ret));
	ret = POMME_WRITE_FILE_ERROR;
	goto err;
    }
err:
    return ret;
}

/**
 * @brief is_file_valid : tell if an given file is valid
 *
 * @param fd:the filehandle
 *
 * @return == 0 if is valid, < 0 not valid 
 */
int is_file_valid(int fd )
{
    int ret = 0;
    pomme_ds_head_t head;

    ret = lseek( fd, 0,SEEK_SET );
   if( ret < 0 )
   {
       debug("seek file failure: %s",strerror(ret));
       goto err;
   } 
   ssize_t rl = 0;
   rl = read(fd, &head,sizeof(pomme_ds_head_t) );
   if( rl < sizeof(pomme_ds_head_t))
   {
       debug("read file not enough");
       ret = POMME_FILE_NOT_VALID;
       goto err;
   }
   if( head.magic != POMME_STORAGE_MAGIC )
   {
       ret = POMME_FILE_NOT_VALID;
   }else{
       ret = 0;
   } 
err:
   return ret;
}

/**
 * @brief set_file_head : set the header
 *
 * @param fd
 *
 * @return 
 */
int set_file_head( int fd, size_t id)
{
    int ret = 0;
    pomme_ds_head_t head;
    memset(&head, 0 ,sizeof(head));

    head.magic = POMME_STORAGE_MAGIC;
    head.id = id;

    ret = lseek( fd, 0,SEEK_SET);
    if( ret < 0 )
    {
	debug("seek file error: %s", strerror(ret));
	goto err;
    }
    ssize_t wl = 0;
    wl = write(fd, &head, sizeof(pomme_ds_head_t));
    if( wl < sizeof(pomme_ds_head_t) )
    {
	debug("write file failure");
	ret = POMME_WRITE_FILE_ERROR;
	goto err;
    }
err:
    return ret;
} 
