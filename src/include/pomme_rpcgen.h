/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcgen.h
 *
 *    Description:  code generator for pomme rpc
 *
 *        Version:  1.0
 *        Created:  06/07/2012 03:30:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_RPCGEN_H
#define _POMME_RPCGEN_H
typedef struct type
{
    char *name;
    int len;
}pomme_type_t;
extern const pomme_type_t POMME_CHAR; 
extern const pomme_type_t POMME_INT;
extern const pomme_type_t POMME_LL; // LONG LONG
extern const pomme_type_t POMME_SHORT; 

extern const pomme_type_t POMME_USHORT;
extern const pomme_type_t POMME_ULL;
extern const pomme_type_t POMME_UINT; // 
extern const pomme_type_t POMME_UCHAR;

typedef struct funcgen
{
    char *name;
    int argNum;
    pomme_type_t *args;
    pomme_link_t next;

}funcgen_t;
typedef struct rpcgen
{
}rpcgen_t;
#endif
