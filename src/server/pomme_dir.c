/*
 * =====================================================================================
 *
 *       Filename:  pomme_dir.c
 *
 *    Description:  dir
 *
 *        Version:  1.0
 *        Created:  03/27/2012 07:24:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_dir.h"
#include "utils.h"

/**
 * @brief pomme_child_name ,
 * get a key of the child of a dir
 */
int pomme_child_name(char *dir, char *file, char *nName)
{
    sprintf(nName,"%s$%s",dir,file);
    return 0;
}
/* find the parrent of the path */
int pomme_child_parrent(char *path,char *par)
{
    int lp = strlen(path) -1 ;

    while( path[lp] == '/' && lp > 0 )lp--;

    while( lp != '/' ) lp--;
    memcpy(par, path, lp);
    return 0;
}
