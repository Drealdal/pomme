/*
 * =====================================================================================
 *
 *       Filename:  fastbit.c
 *
 *    Description:  the implement of fast bit
 *
 *        Version:  1.0
 *        Created:  06/21/2012 01:19:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "fastbit.h"

#ifdef __cplusplus
extern "C" {
#endif
    int max_height()
    {
	int ret = 1,i = 0;
	while(FB_LEN *ret > ret )
	{
	    ret *= FB_LEN;
	    i++;
	}
	return i;
    }

    int fb_subsize(int maxdepth, int depth)
    {
	int ret = 1;

	while( maxdepth > depth )
	{
	    ret *= FB_LEN;
	    maxdepth --;
	}
	return ret;

    }
    int array_first0(unsigned char *bits, int len)
    {
	int i = 0,j = 0;
	while(*bits == 0xff && i < len )
	{
	    i++;
	    bits++;
	}

	i = i*8;

	if( i == len ) return -1;
	unsigned char t = *bits;

	while( (t%2) != 0 )
	{
	    j++;
	    t/=2;
	}
	return i+j;
    }

    int array_n0_1(unsigned char *bits, int len, int n)
    {
	if( bits == NULL || n >= len )
	{
	    debug(DEBUG_FASTBIT," illegal number %d %d", len, n);
	    return RET_FAILURE;
	}	

	bits[n/8] |= 1<<(n%8);
	return RET_SUCCESS;
    }
    int array_n1_0(unsigned char *bits, int len , int n)
    {
	if( bits == NULL || n >= len )
	{
	    debug(DEBUG_FASTBIT," illegal number");
	    return RET_FAILURE;
	}	

	bits[n/8] &= ~(1<<(n%8));
	return RET_SUCCESS;
    }

    int fb_init(fastbit_t *fb, int maxdepth,
	    int depth, int set_1)
    {
	if( fb == NULL )
	{
	    debug(DEBUG_FASTBIT,"null fb error");
	    return RET_FAILURE; 
	}
	if( maxdepth > max_height() )
	{
	    debug(DEBUG_FASTBIT, "Too large bitmap exceed integer");
	    return RET_FAILURE;
	}

	memset(fb, 0, sizeof(fastbit_t));
	fb->max_depth = maxdepth;

	fb->depth = depth;
	fb->sbit = (fastbit_t **)malloc(sizeof(fastbit_t*)*FB_LEN);
	memset(fb->sbit,0,sizeof(fastbit_t*)*FB_LEN);

	if( fb->sbit == NULL )
	{
	    debug(DEBUG_FASTBIT,"MALLOC ERROR");
	    return RET_FAILURE;
	}

	if( set_1 == 1 )
	{
	    memset(fb->bits, 0xffffffff, FB_LEN/8);
	   // for( i = 0; i < FB_LEN/8; i++ )
	}	
	return RET_SUCCESS;
    }

    int fb_set_first0_1( fastbit_t *fb )
    {
	int ret , i, cnum = 0, snum = 0;

	if( fb->max_depth == fb->depth ) 
	{
	    ret = array_first0(fb->bits, FB_LEN);
	    array_n0_1(fb->bits, FB_LEN,ret);

	    return ret;
	}

	cnum = array_first0(fb->bits, FB_LEN);
	if( cnum == -1 )
	{
	    return RET_FAILURE;
	}

	if( fb->sbit[cnum] == NULL )
	{
	    fb->sbit[cnum] = (fastbit_t *)malloc(sizeof(fastbit_t));

	    if( fb->sbit[cnum] == NULL )
	    {
		debug(DEBUG_FASTBIT, "malloc error");
		return RET_FAILURE;
	    }

	    if( ( fb_init(fb->sbit[cnum],fb->max_depth,
			    fb->depth + 1,0)) < 0 )
	    {
		free(fb->sbit[cnum]);
		fb->sbit[cnum] = NULL;
		return RET_FAILURE;
	    }
	}

	snum = fb_set_first0_1(fb->sbit[cnum]);
	if( snum == -1 )
	{
	    debug(DEBUG_FASTBIT,"not find at child %d",cnum);
	    array_n0_1(fb->bits, FB_LEN, cnum);
	    /*  the sub bit map is full */
	    free(fb->sbit[cnum]);
	    fb->sbit[cnum] = NULL;

	    return fb_set_first0_1( fb );
	}

	for( i = fb->max_depth ; i > fb->depth; i-- )
	{
	    cnum *= FB_LEN;
	}	

	return cnum + snum;
    }

    int fb_setn(fastbit_t *fb, int n, int set)
    {
	int subsize, cnum = 0;

	if( fb->max_depth == fb->depth )
	{

	    if( set == 0 )  
	    {
		if( array_n1_0(fb->bits, FB_LEN,
			    n) < 0 )
		{
		    debug(DEBUG_FASTBIT,"set %d for %d error",
			    n, FB_LEN);
		    return RET_FAILURE;
		}
	    }else{
		if( array_n0_1(fb->bits, FB_LEN,
			    n) < 0 )
		{
		    debug(DEBUG_FASTBIT,"set %d for %d error",n,FB_LEN);
		    return RET_FAILURE;
		}

	    }
	    return RET_SUCCESS;
	}

	subsize = fb_subsize(fb->max_depth, fb->depth);

	while( n >= subsize )
	{
	    n-= subsize;
	    cnum ++;
	}	
	if( cnum >= FB_LEN )
	{
	    debug(DEBUG_FASTBIT, "illegal num");
	    return RET_FAILURE;
	}
	if( fb->sbit[cnum] == NULL )
	{
	    fb->sbit[cnum] = (fastbit_t *)malloc(sizeof(fastbit_t));

	    if( fb->sbit[cnum] == NULL )
	    {
		debug(DEBUG_FASTBIT, "malloc error");
		return RET_FAILURE;
	    }

	    if( ( fb_init(fb->sbit[cnum],fb->max_depth,
			    fb->depth + 1,1)) < 0 )
	    {
		debug(DEBUG_FASTBIT,"init failure");
		free(fb->sbit[cnum]);
		fb->sbit[cnum] = NULL;
		return RET_FAILURE;
	    }
	}

	if( (fb_setn1_0(fb->sbit[cnum],n) ) < 0 )
	{
	    debug(DEBUG_FASTBIT,"unknown error");
	    return RET_FAILURE;
	}

	if(set == 0)
	{
	  return  array_n1_0(fb->bits, FB_LEN , cnum);
	}
	return RET_SUCCESS;

    }
    int fb_setn1_0(fastbit_t *fb, int n)
    {
	return fb_setn(fb,n,0);
    }
    int fb_setn0_1(fastbit_t *fb, int n)
    {
	return fb_setn(fb,n,1);
    }

#ifdef __cplusplus
}
#endif
