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
#include "pomme_queue.h"
#include "pomme_client_file.h"
#include "pomme_client_data.h"
#include "pomme_mapping.h"


typedef struct pomme_client pomme_client_t;
extern pomme_client_t GLOBAL_CLIENT;

#define POMME_CREATE 1

struct pomme_client
{
    int inited;
    /*  the ip of the master node */
    u_int32 mip;
    /*  the port of the master node */
    u_int32 mport;
    /*  buffer of the meta storage nodes info */
    pomme_hash_t *ms_nodes;
    /*  buffer of the data storage nodes info */
    pomme_hash_t *ds_nodes;
    /*  pomme_hash_t inodes , mapping path to inodes num*/
    pomme_hash_t *imap;
    /*  mapping  */
    map_t msmap;
    /*  open fileCount */
    u_int32 count;
    u_int32 nextfd;
    /*parrelel access from several client*/
    pthread_mutex_t mutex;
    /**/
    pomme_queue_t *open_files;
    pomme_queue_t *closed_files;
    int max_count;


    int ( *get_ds_info )(
	    pomme_client_t *client,
	    u_int32 msid, u_int32 id,
	    u_int32 *ip, 
	    u_int16 *port); 

    int ( *get_ms_info )(pomme_client_t *client,
	    u_int32 id,
	    u_int32 *ip,
	    u_int16 *port);

    int ( *get_ms_for_path )(pomme_client_t *client,
	    const char *path);
    PFILE *( *get_pfile )(pomme_client_t *client);

    int ( *get_dsgroup)(pomme_client_t *client, 
	    u_int32 gid,int *dsnum, u_int32 **dsids);
};
int cmp_dsnode(void *node1, void *node2);
int cmp_msnode(void *node1, void *node2);

int pomme_client_init(pomme_client_t *client,
       	u_int32 mip, 
	u_int16 mport);

int pomme_client_distroy(pomme_client_t *client);

PFILE * pomme_open(const char *path,int mode);
void pomme_close(PFILE *file);

int pomme_write(const void *ptr,
	size_t size, 
	size_t nmemb, 
	PFILE *file);

int pomme_read(void *ptr,
	size_t size, 
	size_t nmemb, 
	PFILE *file);

typedef struct send_arg
{
    u_int32 dsid;
    uuid_t objectid;
    PFILE *file;
    void *data;
    int len;
    int flags;// reserved
}send_arg_t;
#endif
