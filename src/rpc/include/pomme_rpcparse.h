/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcparse.h
 *
 *    Description:  parse an xml file to a structure
 *
 *        Version:  1.0
 *        Created:  06/07/2012 07:07:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_RPCPARSE_H
#define _POMME_RPCPARSE_H
#include "pomme_rpcgen.h"
#include <libxml/parser.h>
int pomme_parse_server(xmlNodePtr node, rpcgen_t *server);
/**
 * @brief pomme_parse_init: read a xml file. and create 
 *
 * @param path: the parse of the file
 *
 * @return: < 0 for failure 
 */
int pomme_parse_init(xmlNodePtr node, rpcgen_t **server);
#endif
