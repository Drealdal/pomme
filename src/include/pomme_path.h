/*
 * =====================================================================================
 *
 *       Filename:  pomme_path.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2012 01:41:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_PATH_H
#define _POMME_PATH_H

#define PATH_SUB_SEPERATOR "$"
#define PATH_SLASH '/'

char *make_dir_path(char *path);
char *make_sub_dir(char *parrent, char *name);

char *make_sub_file(char *parrent, char *name);
char *get_parrent(char *path);

#endif
