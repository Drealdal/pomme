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
typedef struct param
{
    char *name;
    pomme_type_t type;
}pomme_param_t;

typedef struct funcgen
{
    char *name;
    int argNum;
    pomme_param_t *args;
    pomme_param_t *rParam; // return type

}funcgen_t;
typedef struct rpcgen
{
    funcgen_t *funcs;
}rpcgen_t;
#endif
