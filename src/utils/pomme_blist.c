
/*
 * =====================================================================================
 *
 *       Filename:  linklist.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2011 11:13:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "pomme_blist.h"
static inline void __link_add(struct link_head *new, struct link_head *prev,struct link_head *next)
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;

}
inline void link_add(struct link_head *new, struct link_head *head)
{
	__link_add(new,head,head->next);
}
static inline void __link_del(struct link_head *prev, struct link_head *next)
{
	prev->next = next;
	next->prev = prev;
}
inline void link_del(struct link_head *entry)
{
	__link_del(entry->prev, entry->next);
	entry->prev = LIST_POSITION_1;
	entry->next = LIST_POSITION_2;
}
inline void init_link(pomme_link_t *head)
{
	if( head == NULL)
	{
		return;
	}
	head->prev = head;
	head->next = head;
}
