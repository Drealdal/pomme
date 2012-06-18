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

static inline void rpc_clinet_hfile(char *prefix, 
	char *serverName, char *path)
{
    assert( serverName  != NULL );
    sprintf(path, "%s_client_%s.h",prefix, serverName);
}

static inline void rpc_clinet_cfile(char *prefix, 
	char *serverName, char *path)
{
    assert( serverName  != NULL );
    sprintf(path, "%s_client_%s.c",prefix, serverName);
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
static inline void print_csync_func_name(FILE *fd, char *prefix, funcgen_t *func)
{
    fprintf(fd, "%s_sync_%s",prefix, func->name);
}
static inline void print_casyn_func_name(FILE *fd, char *prefix, funcgen_t *func)
{
    fprintf(fd, "%s_asyn_%s",prefix, func->name);
}

static inline int has_return(funcgen_t *func)
{
    assert( func != NULL );
    if( strcmp(RPC_NORET,func->rparam.type.name) == 0)
    {
	return 0;
    }
    return 1;
}
/*
 * all the names are defined as macros
 */
int pomme_gen_macro(rpcgen_t *server)
{
    int ret = 0,i;
    char *uname;
    assert(server != NULL);
    char cpath[POMME_PATH_MAX];

    rpc_const_file(server->prefix,server->name, cpath);
    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	err_exit("open %s fail",cpath);
    }	

    uname = to_uper(server->name);

    /*  header */
    fprintf(fd,"#ifndef _%s_CONST_H\n",uname);
    fprintf(fd,"#define _%s_CONST_H\n\n",uname);


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
	fprintf(fd,"\n");
    }
    /*  tail */
    fprintf(fd,"#endif\n\n");

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

    fprintf(fd, "#include \"utils.h\"\n");
    fprintf(fd, "#include \"pomme_utils.h\"\n\n");

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

    fprintf(fd,"#endif\n\n");

    free(uname);
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

    char *nshort = pomme_short_name(server->name,RPC_SHORT_LEN);
    fprintf(fd,"/* register function declear  */\n");
    fprintf(fd,"static int register_funcs(%s * %s);\n", 
	    cpath, nshort);

    /* init function init*/
    fprintf(fd,"\n/*  The init function of the structure */\n");
    fprintf(fd,"int %s_%s_init(%s *%s)\n{\n", server->prefix,
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

    fprintf(fd,"}\n");
    /* init function over*/
    /* register function begin*/ 
    fprintf(fd,"/* register function */\n");
    fprintf(fd,"static int register_funcs(%s * %s)\n{\n", 
	    cpath, nshort);

    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;
	fprintf(fd,"/* %s*/\n",f->name);

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

    fprintf(fd,"}\n");
    /* register function over*/ 

    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t *f = server->funcs+i;

	fprintf(fd, "DEF_POMME_RPC_FUNC(");
	print_func_name_macro(fd, server->name, server->funcs+i);
	fprintf(fd, ")\n{\n");
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
	print_func_imp_name(fd, server->prefix, f);
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
	print_func_imp_name(fd, server->prefix, f);

   	char *nshort = pomme_short_name(server->name,RPC_SHORT_LEN);
	fprintf(fd,"(%s *%s",cpath,nshort);
	free(nshort);

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    if( p->ispointer )
	    {
		fprintf(fd,", %s * %s",t->name,p->name);
	    }else{
		fprintf(fd,", %s %s",t->name,p->name);
	    }
	}
	fprintf(fd,");\n");
	
    }
   
    /*  tail */
    free(uname);
    fprintf(fd,"#endif\n");
    debug("imp header file generated");

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
	fprintf(fd,"/*  %s */\n",f->name);
	fprintf(fd,"pomme_data_t * ");
	print_func_imp_name(fd, server->prefix, f);
   	 char *nshort = pomme_short_name(server->name,RPC_SHORT_LEN);
	fprintf(fd,"(%s *%s",cpath,nshort);
	free(nshort);

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
	fprintf(fd,")\n{\n");
	fprintf(fd,"}\n");
    }

    debug("imp cfile generated");
    return ret;
}

int pomme_client_hgen(rpcgen_t *server)
{
    int ret = 0,i,j;
    char cpath[POMME_PATH_MAX];
    char *uname;
    rpc_clinet_hfile(server->prefix, 
	    server->name,cpath);

    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    uname = to_uper(server->name);
    /*  header  */
    fprintf(fd, "#ifndef _%s_CLIENT_H\n",uname);
    fprintf(fd, "#define _%s_CLIENT_H\n",uname);
    free(uname);

    fprintf(fd, "#include \"pomme_rpcc.h\"\n\n");

    /*  body  */

    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t * f = server->funcs+i;

	fprintf(fd,"/* *\n* @bref:");
	print_csync_func_name(fd, server->prefix,f);
	fprintf(fd,"*/\n");
	fprintf(fd,"int ");
	print_csync_func_name(fd,server->prefix,f);
	fprintf(fd,"rpcc_t *rct");

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    /* pass pointer */
	    fprintf(fd,",\n%s * %s",t->name,p->name);
	}
	if(strcmp(f->rparam.name,"void") == 0 )
	{
	    fprintf(fd,",\n%s **%s",f->rparam.type.name,f->rparam.name);
	}
	fprintf(fd,");\n");
    }

    /*  tail */
    fprintf(fd, "#endif\n\n");
    free(uname);
    debug("client header file generated");

    return ret;
}
int pomme_client_cgen(rpcgen_t *server)
{
    int ret = 0,i,j;
    char cpath[POMME_PATH_MAX];
    rpc_clinet_cfile(server->prefix, 
	    server->name,cpath);

    FILE *fd = fopen(cpath, "w+");

    if(fd == NULL)
    {
	error("open %s fail",cpath);
	exit(-1);
    }	
    /*  header  */
    rpc_clinet_hfile(server->prefix, 
	    server->name,cpath);

    fprintf(fd, "#include \"%s\"\n\n",cpath);

    /*  body  */

    for( i = 0; i < server->funcnum; i++)
    {
	funcgen_t * f = server->funcs+i;

	fprintf(fd,"int ");
	print_csync_func_name(fd,server->prefix,f);
	fprintf(fd,"rpcc_t *rct");

	for( j = 0; j < f->argnum; j++ )
	{
	    pomme_param_t  *p = f->params+j;
	    pomme_type_t *t = &p->type;
	    /* pass pointer */
	    fprintf(fd,",\n%s * %s",t->name,p->name);
	}
	if(has_return(f) == 1 )
	{
	    fprintf(fd,",\n%s **%s",f->rparam.type.name,f->rparam.name);
	}
	fprintf(fd,")\n{\n");

	int len = f->argnum +1;
	fprintf(fd,"pomme_data_t *arg = malloc(%d*sizeof(pomme_data_t));\n",len);
	fprintf(fd,"assert(arg!=NULL)\n");

	fprintf(fd,"char *name = ");
	print_func_name_str_macro(fd,  server->name,  f);

	fprintf(fd,"\n");
	fprintf(fd,"arg[0].size = pomme_strlen(name);\n");
	fprintf(fd,"arg[0].data = name;\n");

	for( j  = 0; j < f->argnum; j++ )
	{
	    pomme_param_t *p = f->params + j ;
	    pomme_type_t *t = &p->type;
	    
	    /*  gen size */
	    if( (strcmp(t->name,"char") == 0 ) 
		    &&(p->ispointer == 1))
	    {
		fprintf(fd, "arg[%d].size = strlen(%s);\n",j+1,p->name);
	    }else{	
		fprintf(fd, "arg[%d].size = sizeof(%s));\n",j+1,p->name);
	    }
	    if( p->ispointer == 1 )
	    {
		fprintf(fd, "arg[%d].data = %s;\n",j+1,p->name);
	    }else{
		fprintf(fd, "arg[%d].data = &%s;\n",j+1,p->name);
	    }
	    fprintf(fd,"\n");
	}

	fprintf(fd,"pomme_data_t res;\n");
	fprintf(fd,"memset(&res, 0, sizeof(pomme_data_t));\n");

	fprintf(fd,"if( (ret = rct->sync_call(rct,3,arg,&res,0)) < 0 )\n{\n");
	fprintf(fd,"//TODO DEALING ERROR\n debug(\"Error Call\");\n goto err;\n");

	fprintf(fd,"\n}\nesle{\n//TODO SUCCESS\ndebug(\"Success Call\"\n);\n}");

	if(has_return(f) == 1 )
	{
	    fprintf(fd,"*%s = (%s *)res.data);\n",f->rparam.name, f->rparam.type.name);
	}

	fprintf(fd,"err:\n");
	fprintf(fd,"free(arg);\n");

	fprintf(fd,"return ret;\n");
	fprintf(fd,"\n}\n");
    }

    debug("client c file generated");

    return ret;

}
int pomme_gen_makefile( rpcgen_t *server )
{
    return 0;
}
