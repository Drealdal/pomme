/*
 * =====================================================================================
 *
 *       Filename:  pomme_client_util.c
 *
 *    Description:  utils of client
 *
 *        Version:  1.0
 *        Created:  05/03/2012 12:10:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_client_utils.h"
int pomme_file_print(FILE *f,pomme_file_t *file)
{
    fprintf(f, "%u %s %llu\n",file->dsgroup, ctime(file->c_time),len);
    return 0;
}


