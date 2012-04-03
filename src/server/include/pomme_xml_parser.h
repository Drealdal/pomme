/*
 * =====================================================================================
 *
 *       Filename:  pomme_xml_parser.h
 *
 *    Description:  xml parser of pomme, learn to handle string and file.
 *    may encode of file content. An xml file is an ascii readable file.Used to
 *    deal with xml configure file
 *
 *        Version:  1.0
 *        Created:  03/27/2012 12:33:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_XML_PARSER_H
#define _POMME_XML_PARSER_H

/*
 * KMP is used to find substr
 *
 * regex parser
 *
 */


typedef struct xml_node xml_node;
struct xml_node
{
    char *name;
    union{
	char *text;
	struct{
	    int num;
	    xml_node *ch;
	}child;

    }content;
};


#endif
