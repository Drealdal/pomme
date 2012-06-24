/*
 * =====================================================================================
 *
 *       Filename:  test_path.c
 *
 *    Description:  test the implement of path
 *
 *        Version:  1.0
 *        Created:  06/23/2012 09:43:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_path.h"
int main()
{
    int ret = 0;
    char *path = "/zhumeiqi/test";
    printf("%s \n",get_parrent(path));
    printf("%s \n",get_name(path));
    return ret;
}
