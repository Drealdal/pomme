/*
 * =====================================================================================
 *
 *       Filename:  create_storage.c
 *
 *    Description:  test the functionality of create local storage files
 *
 *        Version:  1.0
 *        Created:  01/11/2012 11:14:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_storage.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX_PATH_LEN 250
char storage_path[MAX_PATH_LEN];

#define test_data_base "./test_create_storage"
int main()
{
    char *sp = getenv("POMME_STORE_HOME");
    int fd, id;
    int ret = 0;

    if( sp == NULL )
    {
	printf("POMME_STORAGE_HOME not set, trying $(POMME_HOME)/storage as data dir\n");
	sp = getenv("POMME_HOME");

	if( sp == NULL )
	{
	    printf("POMME_HOME not set using $(current_dir_name)/storage as data dir\n");
	    sp = get_current_dir_name();
	}
    }
    sp = "/home/zhumeiqi/work/pomme/data";
    snprintf(storage_path, MAX_PATH_LEN, "%s",sp);
    char *ptr = storage_path+ strlen(sp) -1;

    while(*ptr == '/')
    {
	ptr--;
    }
    *(ptr+1) = '/';
    *(ptr+2) = 0;

    debug("%s",storage_path);


    DB *dbp = NULL;

    if( ( ret = db_create(&dbp , NULL , 0)) != 0)
    {
	printf("create db handle error : %s\n",db_strerror(ret));
	goto err;
    }
//    DB->set_re_len(DB *db, u_int32_t re_len);
//    DB->set_re_pad(DB *db, int re_pad);

    dbp->set_re_len(dbp, MAX_PATH_LEN);
    dbp->set_re_pad(dbp, 0x0);
    debug("");

    if(( ret = dbp->open(dbp, NULL, test_data_base, NULL, DB_QUEUE, DB_CREATE, 0644)) !=0 )
    {
	dbp->err(dbp, ret , "open %s failed\n",test_data_base);
	goto err;
    }

    debug("all pass");

    if( (ret = create_storage(dbp , NULL , storage_path,&id, &fd) ) < 0 )
    {
	printf("create storage error\n");
    }else
    {
	printf("create storage success :%d\n",id);
    }
    printf("all pass\n");
    dbp->close(dbp, DB_NOSYNC);
    get_storage_files("/home/zhumeiqi/work/pomme/data",NULL);

err:
    return ret;
}
