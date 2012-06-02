/*
 * =====================================================================================
 *
 *       Filename:  pomme_client.h
 *
 *    Description:  client
 *
 *        Version:  1.0
 *        Created:  05/31/2012 10:22:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_CLIENT_H
#define _POMME_CLIENT_H
#include "pomme_hash.h"
#include "pomme_meta.h"
typedef struct node ds_node;
typedef struct node ms_node;

struct pomme_client
{
    /*  the ip of the master node */
    u_int32 mip;
    /*  the port of the master node */
    u_int32 mport;
    /*  buffer of the meta storage nodes info */
    pomme_hash_t *ms_nodes;
    /*  buffer of the data storage nodes info */
    pomme_hash_t *ds_nodes;

    int ( *get_ds)(u_int32 id, u_int32 *ip, u_int16 *port); 
    int ( *get_ms)(u_int32 id, u_int32 *ip, u_int16 *port);
}pomme_client_t;
int cmp_dsnode(void *node1, void *node2);
int cmp_msnode(void *node1, void *node2);
int pomme_clinet_init(pomme_client_t *client, u_int32 mip, u_int32 mport);
int pomme_client_distroy(pomme_client_t *client);

#endif
