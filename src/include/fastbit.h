/*
 * =====================================================================================
 *
 *       Filename:  fastbitmap.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/21/2012 01:19:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _FASTBITMAP_H
#define _FASTBITMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "debug.h"


#ifdef __cplusplus
extern "C" {
#endif
#define FB_LEN 32
#define FB_MAX_DEPTH 6
/*  the length of each leaf */
#define DEBUG_FASTBIT 1<<2
typedef struct fastbit fastbit_t;
struct fastbit
{
    int totallen;// the length cur struct represent
    int max_depth;
    int depth;
    unsigned char bits[FB_LEN/8];
    /*  sub bitmap */
    fastbit_t **sbit; 
};
/**
 * @brief fb_init: init a fastit map
 *
 * @param fb
 * @param maxdepth: max depth of the bitmap tree, the height of
 *                  tree determine the total size of the bitmap
 * @param depth: depth of the curnode
 * @param set_1: 1 indicate a full bitmap, otherwise empty
 *
 * @return: 0 for success , 1 for failure 
 */
int fb_init(fastbit_t *fb, int maxdepth,
       	int depth, int set_1);
/**
 * @brief fb_set_first0_1 set the first 0 in the bitmap to 1
 *
 * @param fb: the struct
 *
 * @return: the rank of the first 0 
 */
int fb_set_first0_1( fastbit_t *fb );
/**
 * @brief fb_setn1_0: set the nth bitmap to 0
 *
 * @param fb:
 * @param n
 *
 * @return 0 for success , -1 for failure 
 */
int fb_setn1_0(fastbit_t *fb, int n);
int fb_setn0_1(fastbit_t *fb, int n);
int max_height();


#ifdef __cplusplus
}
#endif

#endif
