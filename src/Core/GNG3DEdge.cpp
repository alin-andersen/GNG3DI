#include "Core/GNG3DEdge.h"

void GNG3DEdge_Init(struct GNG3DEdge* edge, struct GNG3DList* list, void* b, struct GNG3DList* b_list, void* e, struct GNG3DList* e_list)
{
    edge->check = 69;
    edge->age = 0;

    // connection
    edge->b = b;
    edge->e = e;

    GNG3DLIST_LINK_BEFORE(b_list, &edge->b_list);
    edge->b_list.ptr = edge;

    GNG3DLIST_LINK_BEFORE(e_list, &edge->e_list);
    edge->e_list.ptr = edge;

    GNG3DLIST_LINK_BEFORE(list, &edge->list);
    edge->list.ptr = edge;
}

void GNG3DEdge_Dest(struct GNG3DEdge* edge)
{
    GNG3DLIST_UNLINK(&edge->b_list);
    GNG3DLIST_UNLINK(&edge->e_list);
    GNG3DLIST_UNLINK(&edge->list);
}


void* GNG3DEdge_GetOtherNode(struct GNG3DEdge* edge, void* node)
{
    if(node == edge->b) return edge->e;
    else return edge->b;
}

