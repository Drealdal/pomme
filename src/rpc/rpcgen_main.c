/*
 * =====================================================================================
 *
 *       Filename:  rpcgen_main.c
 *
 *    Description:  the main of rpcgen
 *
 *        Version:  1.0
 *        Created:  06/09/2012 08:08:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_rpcparse.h"
#include "pomme_rpcgen.h"
int expect_argc = 2;

int usage(int argc, char **argv)
{
    if( argc != expect_argc )
    {
	printf("usage: %s /path/to/xml\n",argv[0]);
	return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0 ;
    if( ( ret = usage(argc, argv)) != 0 )
    {
	exit(-1);
    }
    char *path = argv[1];

    xmlDocPtr pdoc = NULL;
    xmlNodePtr proot = NULL; 

    pdoc = xmlReadFile(path, "UTF-8",XML_PARSE_NOBLANKS);
    if( pdoc == NULL )
    {
	error("Open file %s failure",path);
	exit(-1);
    }
    proot = xmlDocGetRootElement(pdoc);
    if( proot == NULL )
    {
	error("Empty file\n");
	exit(-1);
    }



    rpcgen_t *server = NULL;
    int i, server_count = 0;
    if(( server_count = pomme_parse_init( proot,
		    &server)) < 0 )
    {
	error("Parse xml file(%s) failure",path);
	exit(-1);
    }

    debug("%s",server->name);
    if(( ret = pomme_gen_macro(server)) < 0 )
    {
	error("Fail generate Const file for server(%s)",(server+i)->name);
    }

    pomme_gen_struct(server);
    pomme_gen_main(server);
    pomme_gen_imph(server);
    pomme_gen_impc(server);
    return 0;
}

