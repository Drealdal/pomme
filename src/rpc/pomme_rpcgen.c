/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcgen.c
 *
 *    Description:  rpc code gen
 *
 *        Version:  1.0
 *        Created:  06/07/2012 03:30:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcgen.h"
const pomme_type_t POMME_CHAR={
.name="char",
.len=sizeof(char)
};

const pomme_type_t POMME_INT={
.name="int",
.len=sizeof(int)
};

const pomme_type_t POMME_LL={
.name="long long",
.len=sizeof(long long)
};

const pomme_type_t POMME_SHORT={
.name="SHORT",
.len=sizeof(short)
};

const pomme_type_t POMME_USHORT={
.name="unsigned short",
.len=sizeof(unsigned short)
};

const pomme_type_t POMME_ULL={
.name="unsigned long long",
.len=sizeof(unsigned long long)
};

const pomme_type_t POMME_UINT={
.name="unsigned int",
.len=sizeof(unsigned int)
};

const pomme_type_t POMME_UCHAR={
.name="unsigned char",
.len=sizeof(unsigned char)
};

static inline void rpc_const_file(char *serverName,char path[])
{
    assert( serverName != NULL );
    sprintf(path,"pomme_%s_const.h",serverName);
}
static inline void print_func_name_macro(FILE *fd, char *server_name, funcgen_t *func)
{
    char *upers = to_uper(server_name);
    char *uperf = to_uper(func->name);
    fprintf(fd, "RPC_%s_%s",upers, uperf);
}
static inline void print_func_name_str_macro(FILE *fd, char *server_name, funcgen_t *func)
{
    char *upers = to_uper(server_name);
    char *uperf = to_uper(func->name);
    fprintf(fd, "RPC_%s_%s_STR",upers, uperf);
}
/*
 * all the names are defined as macros
 */
int pomme_gen_macro(rpcgen_t *server)
{
    int ret = 0;
    int i = 0;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];
    rpc_const_file(server->name, cpath);
    FILE *fd = fopen(cpath,w+);
    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    char *uname = to_uper(server->name);
    /*  header */
    fprintf(fd,"#ifndef _%s_CONST_H\n",uname);
    fprintf(fd,"#define _%s_CONST_H\n",uname);
    /*  body */
    fprintf(fd,"#define SERVER_NAME %s\n",server->name);
    for( i = 0; i < server->funcnum; i++)
    {
	fprintf(fd, "#define ");
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd," %s \n",(server->funcs+i)->name);

	fprintf(fd,"#define ");
	print_func_name_str_macro(fd, server->name, server->funcs+i);
	fprintf(fd," \"%s\" \n",(server->funcs+i)->name);
    }
    /*  tail */
    fprintf("#endif");

    
    free(uname);
    return ret;
}
