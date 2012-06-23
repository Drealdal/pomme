/*
 * =====================================================================================
 *
 *       Filename:  pomme_meta_main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/28/2012 10:39:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_meta_server.h"

int main()
{
    pomme_ms_t ms;
    int e_o_flags = 0, e_o_mode = 0;
    int id = 0;


    e_o_flags |= DB_CREATE ;// create if not exist
    e_o_flags |= DB_INIT_LOG; // init log
    e_o_flags |= DB_INIT_MPOOL; // init shared mem
    e_o_flags |= DB_INIT_TXN; // using txn 
    e_o_flags |= DB_RECOVER; // recover if neccery
    e_o_flags |= DB_THREAD; // free thread

    e_o_mode |= S_IRUSR;
    e_o_mode |= S_IWUSR;

    pomme_ms_init(&ms,POMME_META_LOG_LEVEL,
	    id,
	    e_o_flags,e_o_mode,
	    10000,
	    10,
	    10000);
    POMME_LOG_INFO("META Sever Inited",ms.logger);
    debug("started");
    ms.start(&ms);
    //will not be here
    return 0;
}
