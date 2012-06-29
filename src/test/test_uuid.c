/*
 * =====================================================================================
 *
 *       Filename:  test_uuid.c
 *
 *    Description:  test the use of uuid
 *
 *        Version:  1.0
 *        Created:  06/29/2012 06:33:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "debug.h"
#include <uuid/uuid.h>
#define DEBUG_TEST 1
void print(uuid_t id);
int main()
{
    uuid_t id,id2;
    char out[36];
    uuid_unparse(id,out);
    debug(DEBUG_TEST,"The object id is:%s",out);
    uuid_copy(id2,id);
    char out2[36];
    uuid_unparse(id,out2);
    debug(DEBUG_TEST,"The object id is:%s",out2);
    print(id);

    return 0;
}
void print(uuid_t id)
{

    char out[36];
    uuid_unparse(id,out);
    debug(DEBUG_TEST,"print The object id is:%s",out);
}
