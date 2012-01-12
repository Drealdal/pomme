/*
 * =====================================================================================
 *
 *       Filename:  test_bdb.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/12/2011 02:44:48 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include"db.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"pomme_storage.h"

#define DATABASE "./test_create_storage.db"


char *storage_path = "/home/zhumeiqi/storage"
int main(int argc,char *argv[])
{
	DB *dbp=NULL;
	int ret = 0;
	if( (ret = db_create(&dbp,NULL,0))!=0)
	{
		fprintf(stderr,"db_create:%s\n",db_strerror(ret));
		exit(1);
	}
	if(( ret = dbp->open(dbp,NULL,DATABASE,NULL,DB_QUEUE,DB_CREATE,0664))!=0)
	{
		printf("%d\n",ret);
		dbp->err(dbp,ret,"open failed%s",DATABASE);
		exit(1);
	}

	size_t id;
	ret = create_storage( dbp, NULL, &id);
	if( ret < 0 )
	{
	    printf("create storage error\n");

	}else{
	    printf("create success %d\n",id);
	}


	dbp->close(dbp,DB_NOSYNC);

	return 0;
}
