/*
 * =====================================================================================
 *
 *       Filename:  pomme_mapping.c
 *
 *    Description:  mapping between inode and msserver
 *
 *        Version:  1.0
 *        Created:  06/23/2012 05:45:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_mapping.h"
#include "const.h"

static int inode2ms(u_int64 inode, u_int32 *ms);

int pomme_mapping_init(map_t *map)
{
    assert( map != NULL );
    map->inode2ms = inode2ms;

    return RET_SUCCESS;
}
/**
 * @brief inode2ms: only one meta server used
 *
 * @param inode
 * @param ms
 *
 * @return 
 */
static int inode2ms(u_int64 inode, u_int32 *ms)
{
    *ms = 0; 
    return 0;
}
