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

static inline void rpc_const_file(char *prefix,
	char *serverName,char *path)
{
    assert( serverName != NULL );
    assert( prefix != NULL );
    sprintf(path,"%s_%s_const.h",prefix,serverName);
}
static inline void rpc_server_hfile(char*prefix,
	char *serverName, char *path)
{
    assert( serverName != NULL );
    assert( prefix != NULL );
    sprintf(path, "%s_%s.h",prefix,serverName);
}

static inline void rpc_server_cfile(char *prefix,
	char *serverName, char *path)
{
    assert(serverName != NULL);
    sprintf(path, "%s_%s.c",prefix, serverName);
}

static inline void rpc_server_imph(char *prefix,
	char *serverName, char *path)
{
    assert( serverName != NULL );
    sprintf(path, "%s_%s_imp.h",prefix, serverName);
}

static inline void rpc_server_impc(char *prefix,
	char *serverName, char *path)
{
    assert( serverName != NULL );
    sprintf(path, "%s_%s_imp.c",prefix, serverName);
}

static inline void rpc_server_structname(rpcgen_t *server,
	char *path)
{
    assert( server != NULL );
    assert( path != NULL );
    sprintf(path,"%s_%s_t",server->prefix, server->name);
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

static inline void print_func_imp_name(FILE *fd, 
	char *prefix, funcgen_t *func)
{
    fprintf(fd, "%s_%s",prefix, func->name);
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

    rpc_const_file(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

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
	debug("func_name:%s",server->funcs[i].name);
	fprintf(fd, "#define ");

	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd," %s \n",(server->funcs+i)->name);

	fprintf(fd,"#define ");
	print_func_name_str_macro(fd, server->name, server->funcs+i);

	fprintf(fd," \"%s\" \n",(server->funcs+i)->name);
    }
    /*  tail */
    fprintf(fd,"#endif");

    free(uname);
    fclose(fd);
    return ret;
}
/*
 * data_server.h
 */
int pomme_gen_struct(rpcgen_t *server)
{
    int ret = 0;
    int i = 0;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];

    rpc_server_hfile(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	

    char *uname = to_uper(server->name);
    /*  header */
    fprintf(fd,"#ifndef _%s_H\n",uname);
    fprintf(fd,"#define _%s_H\n",uname);

    fprintf(fd,"\n");
    /*  body */
    fprintf(fd,"typedef struct %s{\n",server->name);

    fprintf(fd,"/*  struct member */\n\n");

    fprintf(fd,"pomme_rpcs_t rpcs;\n");
    fprintf(fd,"/*  rpc function */\n\n");


    for( i = 0; i < server->funcnum; i++)
    {
	debug("func_name:%s",server->funcs[i].name);

	fprintf(fd, "pomme_data_t * ( *");

	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd, 
		" ) (const void *%s, const int n , const pomme_data_t *arg)\n\n",
		server->name);

    }
    rpc_server_structname(server, cpath);
    fprintf(fd,"}%s;",cpath);


    return ret;
}
/*
 *data_server.c 
 */
int pomme_gen_main(rpcgen_t *server)
{
    int ret = 0;
    int i = 0,j = 0;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];

    rpc_server_cfile(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    char *uname = to_uper(server->name);

    rpc_server_hfile(server->prefix,server->name, cpath);
    /*  header */
    fprintf(fd, "#include \"%s\"\n\n",cpath);
    rpc_server_imph(server->prefix, server->name, cpath);
    fprintf(fd, "#include \"%s\"\n\n",cpath);

    rpc_server_structname(server, cpath);

    /*  DEF_HEADER */

    for( i = 0; i < server->funcnum; i++)
    {
	debug("func_name:%s",server->funcs[i].name);

	fprintf(fd, "DEF_POMME_RPC_FUNC(");
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd, ");\n");
	if( i % 3 == 0 ) 
	{
	    fprintf(fd, "\n");
	}

    }

    char *nshort = pomme_short_name(server->name,2);
    fprintf(fd,"/* register function declear  */\n");
    fprintf(fd,"static int register_funcs(%s * %s);\n", 
	    cpath, nshort);

    /* init function init*/
    fprintf(fd,"\n/*  The init function of the structure */\n");
    fprintf(fd,"int %s_%s_init(%s *%s)\n{", server->prefix,
	    nshort, cpath, nshort);

    for( i = 0; i < server->funcnum; i++)
    {
	fprintf(fd, "%s->",nshort);
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd, " = &");
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd,";\n");
	if( i % 3 == 0 ) 
	{
	    fprintf(fd, "\n");
	}
    }
    fprintf(fd,"/*  Call register funcs */\n");
    fprintf(fd,"register_funcs(%s);\n",nshort);

    fprintf(fd,"}");
    /* init function over*/
    /* register function begin*/ 
    fprintf(fd,"/* register function */\n");
    fprintf(fd,"static int register_funcs(%s * %s)\n{", 
	    cpath, nshort);
    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;

	fprintf(fd,"pomme_data_t *arg = malloc(sizeof(pomme_data_t)*%d);\n",f->argnum);
	fprintf(fd,"memset(arg,0,sizeof(pomme_data_t)*%d);\n"
		,f->argnum);

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    if( p->ispointer == 1 )
	    {
		fprintf(fd,"arg[%d].size = -1;\n",j);
	    }else{
		fprintf(fd,"arg[%d].size = sizeof(%s);\n",j,
			t->name);
	    }
	}

	fprintf(fd,"%s->rpcs.func_register(&%s->rpcs,"
		,nshort,nshort);
	print_func_name_str_macro(fd, server->name, f);
	fprintf(fd, "%s->",nshort);
	print_func_name_macro(fd, server->name, f);
	fprintf(fd,",%d,arg);\n",f->argnum);

    }

    fprintf(fd,"}");
    /* register function over*/ 

    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;

	fprintf(fd, "DEF_POMME_RPC_FUNC(");
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd, ")\n{");
	fprintf(fd, "assert(n==%d);\n",f->argnum);
	fprintf(fd, "assert(extra != NULL);\n");
	rpc_server_structname(server, cpath);
	fprintf(fd, "%s * %s = (%s *)extra;\n",
		cpath, nshort,cpath);

	/*  for each param */
	for( j = 0 ; j < f->argnum ; j ++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    if( p->ispointer == 1 )
	    {
		fprintf(fd, "%s * %s = (%s *)arg[%d].data;\n",
			t->name, p->name, t->name, j);
	    }else{
		fprintf(fd, "%s %s = *(%s *)arg[%d].data;\n",
			t->name, p->name, t->name, j );
	    }

	}

	/*  call the function */
	fprintf(fd, "return ");
	print_func_imp_name(fd, server->prefix, f->name);
	fprintf(fd,"(%s", nshort);
	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    fprintf(fd,",%s",p->name);
	}
	fprintf(fd,");\n");
	fprintf(fd,"}\n");
    }
    free(nshort);
    return ret;
}

int pomme_gen_imph(rpcgen_t *server)
{
    int ret = 0,i,j;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];
    rpc_server_imph(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    char *uname = to_uper(server->name);
    /*  header */
    fprintf(fd,"#ifndef _%s_IMP_H\n",uname);
    fprintf(fd,"#define _%s_IMP_H\n",uname);
    fprintf(fd,"\n");
    /*  function declear */
    
    rpc_server_structname(server,cpath);
    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;
	fprintf(fd,"pomme_data_t * ");
	print_func_imp_name(fd, server->prefix, f->name);
	fprintf(fd,"(%s",cpath);

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    if( p->ispointer )
	    {
		fprintf(fd,",%s * %s",t->name,p->name);
	    }else{
		fprintf(fd,", %s %s",t->name,p->name);
	    }
	}
	fprintf(fd,");\n");
	
    }
    /*  tail */
    free(uname);
    fprintf(fd,"#endif\n");

    return ret;
}
int pomme_gen_impc(rpcgen_t *server)
{
    int ret = 0,i,j;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];
    rpc_server_impc(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    /*  header */
    rpc_server_imph(server->prefix,server->name, cpath);
    fprintf(fd,"#include \"%s\"\n",cpath);

    fprintf(fd,"\n");
    /*  function declear */
    
    rpc_server_structname(server,cpath);
    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;
	fprintf(fd,"pomme_data_t * ");
	print_func_imp_name(fd, server->prefix, f->name);
	fprintf(fd,"(%s",cpath);

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    if( p->ispointer )
	    {
		fprintf(fd,",%s * %s",t->name,p->name);
	    }else{
		fprintf(fd,", %s %s",t->name,p->name);
	    }
	}
	fprintf(fd,")\n{");
	fprintf(fd,"}\n");
    }

    return ret;
}
