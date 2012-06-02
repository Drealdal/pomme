/*
 * =====================================================================================
 *
 *       Filename:  pomme_data_main.c
 *
 *    Description:  the implement of the pomme data server
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:15:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "pomme_data_server.h"
extern pomme_ds_t GLOBAL_DS;
int main()
{
    pomme_ds_t *ds = &GLOBAL_DS;
    int ret = 0;
    memset(ds, 0 , sizeof(pomme_ds_t));
    char *home=NULL, *m_file=NULL, *s_file=NULL;
    int e_c_flags = 0 ,e_o_flags= 0 ,e_o_mode = 0;

/*
 * flags init
 */
    e_o_flags |= DB_CREATE ;// create if not exist
    e_o_flags |= DB_INIT_LOG; // init log
    e_o_flags |= DB_INIT_MPOOL; // init shared mem
    e_o_flags |= DB_INIT_TXN; // using txn 
    e_o_flags |= DB_RECOVER; // recover if neccery
//    e_o_flags |= DB_THREAD; // free thread

    e_o_mode |= S_IRUSR;
    e_o_mode |= S_IWUSR;


    
   if( (home = getenv("POMME_DATA_HOME")) == NULL )
   {
       home = POMME_DATA_HOME;
   }

   m_file = POMME_MFILE_NAME;
   s_file = POMME_SFILE_NAME;
   ret = pomme_ds_init( ds,
	   home,  m_file,
	   s_file, e_c_flags,
	   e_o_flags,  e_o_mode);

//ret = pomme_ds_init(&ds);
    if( ret < 0 )
    {
	debug("data server memory init failure");
	goto err;
    }
    server(ds);
err:
    return ret;
}
