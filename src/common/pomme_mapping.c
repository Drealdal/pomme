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
    map->inode2rpcc = inode;

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
rpcc_t * inode2rpcc(pomme_client_t *client
	u_int64 inode)
{
	int ret = 0;
	u_int32 ms,mip;
	u_int16 mport;

	if ( ( ret = inode2ms(inode,&ms)) < 0 )
	{
	    debug("Map inode:%llu error",inode);
	    goto clear;
	}

	if( (ret = client->get_ms_info(client,
		       	ms, &msip, &msport) ) < 0 )
	{
	    debug("get meta server info for %u failure",ms);
	    goto clear;
	}
	rpcc_t *rct = malloc(sizeof(rpcc_t));
	if( rct == NULL )
	{
	    return rct;
	}
	if( ( ret = pomme_rpcc_init(&rct, msip,msport,0))
		!= 0 )
	{
	    debug("init rpc client error");
	    return NULL;
	}
	return rct;
}
