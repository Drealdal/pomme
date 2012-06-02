/*
 * =====================================================================================
 *
 *       Filename:  pomme_cimp.c
 *
 *    Description:  the implement of client
 *
 *        Version:  1.0
 *        Created:  06/01/2012 07:57:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
int cmp_dsnode(void *node1, void *node2)
{
    ds_node *n1 = node1;
    ds_node *n2 = node2;
    if(n1->ip == n2->ip && n1->port == n2->port)
    {
	return 0;
    }
    if( n1->ip < n2->ip ) return -1;

    return 1;
}

int cmp_msnode(void *node1, void *node2)
{
    ms_node *n1 = node1;
    ms_node *n2 = node2;
    if(n1->ip == n2->ip && n1->port == n2->port)
    {
	return 0;
    }
    if( n1->ip < n2->ip ) return -1;

    return 1;
}

int pomme_client_init(pomme_client_t *client, 
	u_int32 mip, 
	u_int16 mport)
{
    int ret = 0;
    assert( NULL != client );
    client->mip = mip;
    client->mport = mport;

    ret = pomme_hash_init(POMME_CLIENT_BUFFER_DS,
	    

  


}
