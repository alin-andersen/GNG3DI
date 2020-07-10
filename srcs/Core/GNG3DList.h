#ifndef GNG3DLIST_H
#define GNG3DLIST_H

#include <stdlib.h>

#define GNG3DLIST_STRUCT_OFFSET(ptr,type,member) ((type*)((unsigned char*)(ptr)-(unsigned long)(&((type*)0)->member)))

struct GNG3DList
{
    struct GNG3DList* next;
    struct GNG3DList* prev;
    void* ptr;
};

#define GNG3DLIST_INIT(head)      (head)->next = (head); (head)->prev = (head); (head)->ptr = NULL;
#define GNG3DLIST_EMPTY(head)     ((head)->next == (head) && (head)->prev == (head))
#define GNG3DLIST_NOT_EMPTY(head) !GNG3DLIST_EMPTY(head)
#define GNG3DLIST_FIRST(head)     ((head)->next)
#define GNG3DLIST_LAST(head)      ((head)->prev)
#define GNG3DLIST_PTR(item)       ((head)->ptr)

#define GNG3DLIST_LINK_AFTER(head, entry) \
    (head)->next->prev = entry;          \
    (entry)->next = (head)->next;        \
    (head)->next = entry;                \
    (entry)->prev = head;
#define GNG3DLIST_LINK_BEFORE(head, entry)\
    (head)->prev->next = entry;          \
    (entry)->prev = (head)->prev;        \
    (head)->prev = entry;                \
    (entry)->next = head;
#define GNG3DLIST_UNLINK(entry)           \
    (entry)->next->prev = (entry)->prev; \
    (entry)->prev->next = (entry)->next;

#define GNG3DLIST_FOREACH_BEG(head)         for(struct GNG3DList* _ptr = (head)->next, *_next; _ptr != (head);) { _next = _ptr->next; {
#define GNG3DLIST_FOREACH_END               } _ptr = _next; }
#define GNG3DLIST_FOREACH_ITEM(type,member) GNG3DLIST_STRUCT_OFFSET(_ptr,type,member)
#define GNG3DLIST_FOREACH_ITEM2             ((_ptr)->ptr)
#define GNG3DLIST_FOREACH_LINK_AFTER(item)  GNG3DLIST_LINK_AFTER(_next, item)
#define GNG3DLIST_FOREACH_LINK_BEFORE(item) GNG3DLIST_LINK_BEFORE(_next, item)
#define GNG3DLIST_FOREACH_UNLINK            GNG3DLIST_UNLINK(_ptr)
#define GNG3DLIST_FOREACH_FREE              free(_ptr);

#endif
