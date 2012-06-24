/*
 * =====================================================================================
 *
 *       Filename:  pomme_path.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2012 01:58:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_path.h"
#include "utils.h"
/**
 * @brief make_path: the path of a file or directory
 *
 * @param path
 *
 * @return: a new malloced string , need free by the caller 
 */
char *make_path(char *path)
{
    assert( NULL != path );
    int len = strlen(path)+2;
    char *rPath = malloc(len);
    if(rPath == NULL)
    {
	return rPath;
    }
    memset(rPath, 0, len);

    char *re = rPath;
    int is_slash=0;
    while(*path != '\0' )
    {
	if( *path == PATH_SLASH )
	{
	    if( is_slash )
	    {
		path++;
		continue;
	    }
	    is_slash=1;
	}else{
	    is_slash=0;
	} 
	*rPath++=*path++;
    }
    if(is_slash == 1)
    {
	*(rPath -1) =0;
    }
    return re;
}

char *make_sub_dir(char *parrent, char *name)
{
    assert( NULL != parrent );
    assert( NULL != name );

    int len = strlen(parrent)+strlen(name)+2;
    char *rPath = malloc(len);
    if(rPath == NULL)
    {
	return rPath;
    }

    char *re = rPath;
    memset(rPath, 0, len);
    strcpy(rPath, parrent);
    rPath+=strlen(parrent);

    while(*name == PATH_SLASH && *name != '\0')name++;
    int is_slash = 0;

    while( *name != '\0' )
    {
	*rPath++ = *name;
	if( *name == PATH_SLASH )
	{
	    is_slash = 1;
	    break;
	}
	name++;
    }

    if( is_slash == 0 )
    {
	*rPath = '\0';
    }
    return re;
}

char *get_parrent(char *path)
{
    assert(path != NULL);
    assert(*path == PATH_SLASH);

    int len = strlen(path);
    char *pP = path+len-1;
    while(*pP == PATH_SLASH )
    {
	pP--;
    }
    while(*pP != PATH_SLASH)pP--;
    while(*path == PATH_SLASH)path++;
    path--;

    len = pP - path +2;
    char *rPath = malloc(len);
   if(rPath == NULL )
   {
	return rPath;
   }
   memset(rPath,0,len);
   memcpy(rPath, path, len-1);
   return rPath;
}
char *get_sub_file(char *parrent, char *name)
{

    assert( NULL != parrent );
    assert( NULL != name );

    int len = strlen(parrent)+strlen(name)+2;
    char *rPath = malloc(len);
    if(rPath == NULL)
    {
	return rPath;
    }
    char *re = rPath;
    memset(rPath, 0, len);
    strcpy(rPath, parrent);
    rPath+=strlen(parrent);
    while(*name == PATH_SLASH )name++;
    int is_slash = 0;
    while(*name != '\0' )
    {
	if( *name == PATH_SLASH )
	{
	    is_slash = 1;
	    break;
	}
	*rPath++ = *name++;
    }
    return re;
}

char *get_name(char *fullpath)
{
    char *ret = NULL;
    int i, j;

    assert( fullpath != NULL );
    
    j = strlen(fullpath) -1;
    while(fullpath[j] == PATH_SLASH && j >= 0 )
    {
	debug("%c",fullpath[j]);
	j--;
    }
    debug("%d",j);
    if( j < 0  )
    {
	ret = "";
	return ret;
    }
    i = j;
    while( fullpath[i] != PATH_SLASH && i >= 0 )
    {
	i--;
    }
    i++;
    debug("%d",i);
    debug("%s %s",fullpath+i, fullpath+j);

    ret = malloc(j-i+2);
    assert(ret != NULL);

    memset(ret, 0, j - i + 2 );
    memcpy(ret, fullpath + i, j - i + 1);

    return ret;
}
