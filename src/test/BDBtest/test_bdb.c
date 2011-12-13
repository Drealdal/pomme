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

#define DATABASE "./test.db"


int main(int argc,char *argv[])
{
	DB *dbp=NULL;
	int ret = 0;
	if( (ret = db_create(&dbp,NULL,0))!=0)
	{
		fprintf(stderr,"db_create:%s\n",db_strerror(ret));
		exit(1);
	}
	if(( ret = dbp->open(dbp,NULL,DATABASE,NULL,DB_BTREE,DB_CREATE,0664))!=0)
	{
		printf("%d\n",ret);
		dbp->err(dbp,ret,"open failed%s",DATABASE);
		exit(1);
	}
	DBT key,key2,data,data2;
	memset(&key,0,sizeof(key));
	memset(&key2,0,sizeof(key2));
	memset(&data,0,sizeof(data));
	memset(&data2,0,sizeof(data2));
	key.data = "test";
	key2.data= "aaaa";
	key.size = sizeof("test");
	key2.size = sizeof("aaaa");
	data.data = "testvalue";
	printf("%p\n",data.data);
	data.size = sizeof("testvalue");
	data2.flags = DB_DBT_MALLOC;
	data.flags = DB_DBT_MALLOC;
	if((ret=dbp->put(dbp,NULL,&key,&data,0))==0)
	{
		printf("put ok\n");
	}else{
		dbp->err(dbp,ret,"DB_put");
	}	
	data2.size = sizeof("tellmewhy");
	data2.flags = DB_DBT_MALLOC;
	data.flags = DB_DBT_MALLOC;
	data2.data = "tellmewhy";
	if((ret=dbp->put(dbp,NULL,&key2,&data2,0))==0)
	{
		printf("put ok\n");
	}else{
		dbp->err(dbp,ret,"DB_put");
	}	

	memset(&data,0,sizeof(data));
	memset(&data2,0,sizeof(data2));
	data2.flags = DB_DBT_MALLOC;
	data.flags = DB_DBT_MALLOC;
	if((ret = dbp->get(dbp,NULL,&key,&data,0))==0)
	{
		printf("%s\n",(char *)data.data);

	}else{

		dbp->err(dbp,ret,"DB_get");
	}
//	free(data.data);
	printf("%p %s\n",data.data,(char *)data.data);
	if((ret = dbp->get(dbp,NULL,&key2,&data2,0))==0)
	{
		printf("%p %s\n",data2.data,(char *)data2.data);
		printf("%p %s\n",data.data,(char *)data.data);
	}else{

		dbp->err(dbp,ret,"DB_get");
	}

	dbp->close(dbp,DB_NOSYNC);

	return 0;
}
