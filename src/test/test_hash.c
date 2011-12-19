/*
 * =====================================================================================
 *
 *       Filename:  test_hash.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/14/2011 05:43:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"pomme_hash.h"
static int hash_f(void *,u_int32);
static int cmp_f(void *,void *);
static int cmp_s(void *,void *);

int main()
{
	int ret = 0;
	pomme_hash_t * test_hash = NULL;
	pomme_hash_t * str_str_hash = NULL;
	ret = pomme_hash_init(100,&hash_f,&cmp_f,&test_hash);
	if( ret < 0 )
	{
		fprintf(stderr,"Init hash fail\n");
		exit(-1);
	}
	ret = pomme_hash_init(100,&str_hash,cmp_s,&str_str_hash);
	if( ret < 0 )
	{
		fprintf(stderr,"Inint str_str_hash failure\n");
		return -1;
	}
	pomme_data_t key,data;
	memset(&key,0,sizeof(key));
	memset(&data,0,sizeof(data));
	int i = 0;
	for(i = 0; i < 1000; i++)
	{
		key.size = sizeof(int);
		key.data = &i;
		data.size = sizeof(int);
		data.data = &i;
		ret = pomme_hash_put(test_hash,&key,&data); 
		if( ret < 0 )
		{
			fprintf(stderr,"Put data fail\n");
			break;
		}
		printf("time %d\n",i);
	}
	i = 100;
	key.data = &i; 
	ret = pomme_hash_get(test_hash,&key,&data);
	if( ret < 0 )
	{
		printf("error\n");
	}else{
		printf("data:%d\n",*(int *)data.data);
	}
	pomme_hash_distroy(&test_hash);
	for(i = 0 ;i < 101;i++)
	{
		pomme_data_init(&key,20);
		pomme_data_init(&data,20);
		sprintf(key.data,"KeyIs%d",i);
		sprintf(data.data,"ValueIs%d",i);
		pomme_hash_put(str_str_hash,&key,&data);
	}
//	memset(&key,0,sizeof(key));
	pomme_data_init(&key,20);
	sprintf(key.data,"KeyIs%d",4);
	key.size = 20; 
	ret = pomme_hash_get(str_str_hash,&key,&data);
	if( ret == 0 && data.size > 0)
	{
		printf("%s\n",(char *)data.data);
	}
	else{
		if( ret < 0 && data.size > 0)
		{
			printf("Part Get\n");
		}	
	}
	pomme_hash_distroy(&str_str_hash);
	pomme_data_distroy(&key);
	pomme_data_distroy(&data);
	return 0;

}
static int hash_f(void *data,u_int32 lenght)
{
	return *(int *)data;
}
static int cmp_f(void *a,void *b)
{
	int ia = *(int *)a;
	int ib = *(int *)b;
	if( ia == ib ) return 0;
	else if( ia<ib ) return -1;
	else return 1;
}
static int cmp_s(void *a,void *b)
{
	return strcmp(a,b);
}
