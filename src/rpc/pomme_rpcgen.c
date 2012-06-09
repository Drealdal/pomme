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
.len=sizeof(unsigned long long);
};

const pomme_type_t POMME_UINT={
.name="unsigned int",
.len=sizeof(unsigned int)
};

const pomme_type_t POMME_UCHAR={
.name="unsigned char",
.len=sizeof(unsigned char)
};
