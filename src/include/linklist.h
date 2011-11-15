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
#ifndef POMME_LIST_H
#define POMME_LIST_H
#define LIST_POSITION_1 ((void *) 0x00100100)
#define LIST_POSITION_2 ((void *) 0x00200200)
struct link_head{
	struct link_head *prev,*next;
}

static inline __link_add(struct link_head *new, struct link_head *prev,struct link_head *next)
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}
static inline link_add(struct link_head *new, struct link_head *head)
{
	__link_add(new,head,head->next);
}
static inline __link_del(struct link_head *prev, struct link_head *next)
{
	prev->next = next;
	next->prev = prev;
}
static inline link_del(struct link_head *entry)
{
	__link_del(entry->prev, entry->next);
	entry->prev = LIST_POSITION_1;
	entry->next = LIST_POSITION_2;
}
#define offset_of(TYPE,MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr,type,member) ({	\
		const typeof( ((type *)0)->member) *__mptr = (ptr);\
		(type *)( (char *)__mptr - offsetof(type,member) );});
#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define list_for_each_entry(pos, head,member) for ( pos = list_entry((head)->next, typeof(*pos), member);\
		&pos->member!=(head);\
		pos = list_entry(pos->member.next,typeof(*pos),member))
#endif
