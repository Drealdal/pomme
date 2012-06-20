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
#include "pomme_client_file.h"

typedef struct pomme_client pomme_client_t;
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
    /*  open fileCount */
    u_int32 count;
    u_int32 nextfd;
    /*parrelel access from several client*/
    pthread_mutex_t mutex;
    /**/
    pomme_queue_t files;
    int max_count;


    int ( *get_ds)(pomme_client_t *client, u_int32 id, u_int32 *ip, u_int16 *port); 
    int ( *get_ms)(pomme_client_t *client, u_int32 id, u_int32 *ip, u_int16 *port);
    int ( *get_ms_for_path)(pomme_client_t *client, const char *path);
};
int cmp_dsnode(void *node1, void *node2);
int cmp_msnode(void *node1, void *node2);
int pomme_clinet_init(pomme_client_t *client, u_int32 mip, u_int32 mport);
int pomme_client_distroy(pomme_client_t *client);

PFILE * pomme_open(const char *path,const char *mode);
void pomme_close(PFILE *file);
size_t pomme_write(const void *ptr,size_t size, size_t nmemb, PFILE *stream);
size_t pomme_read(void *ptr,size_t size, size_t nmemb, PFILE *stream);

#endif
