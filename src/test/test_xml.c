/*
 * =====================================================================================
 *
 *       Filename:  test_xml.c
 *
 *    Description:  test the use of libxml
 *
 *        Version:  1.0
 *        Created:  06/08/2012 06:37:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
int main(int argc, char **argv)
{
    xmlDocPtr pdoc = NULL;
    xmlNodePtr proot=NULL,curNode=NULL;
    char *psfilename;
    if( argc < 1 )
    {
	printf("usage:%s xmlfile\n",argv[0]);
	exit(1);
    }
    psfilename = argv[1];
    pdoc = xmlReadFile(psfilename,"UTF-8",XML_PARSE_RECOVER);
    if( pdoc == NULL )
    {
	printf("open file %s error\n", psfilename);
	exit(1);
    }
    proot = xmlDocGetRootElement(pdoc);
    if(proot == NULL)
    {
	printf("Empty file\n");
	exit(1);
    }	
    printf("rootName:%s",xmlGetProp(proot,BAD_CAST "name"));


}
