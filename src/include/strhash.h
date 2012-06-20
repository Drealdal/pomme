/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description:  a lot of hash functions
 *
 *        Version:  1.0
 *        Created:  06/18/2012 06:29:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _HASH_H
#define _HASH_H
#include <string.h>
  
#ifdef __cplusplus  
extern "C" {  
#endif  
      
    /*  A Simple Hash Function */  
    unsigned int simple_hash(char *str);  

    /*  RS Hash Function */  
    unsigned int RS_hash(char *str);  

    /*  JS Hash Function */  
    unsigned int JS_hash(char *str);  

    /*  P. J. Weinberger Hash Function */  
    unsigned int PJW_hash(char *str);  

    /*  ELF Hash Function */  
    unsigned int ELF_hash(char *str);  

    /*  BKDR Hash Function */  
    unsigned int BKDR_hash(char *str);  

    /*  SDBM Hash Function */  
    unsigned int SDBM_hash(char *str);  

    /*  DJB Hash Function */  
    unsigned int DJB_hash(char *str);  

    /*  AP Hash Function */  
    unsigned int AP_hash(char *str);  

    /*  CRC Hash Function */  
    unsigned int CRC_hash(char *str);  
  
#ifdef __cplusplus  
}  
#endif  
#endif //_HASH_H
  
