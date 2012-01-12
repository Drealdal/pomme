/*
 * =====================================================================================
 *
 *       Filename:  test_cfunc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/20/2011 06:04:05 PM
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

#ifdef DEBUG
#define debug(msg,argc...) fprintf(stderr,msg,##argc);
#else
#define debug(msg,arg...) 0
#endif
int main()
{
	debug("nihao\n");
	debug("nihao%s\n"," test");


	return 0;
}
