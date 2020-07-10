#ifndef GNG3DEDGE_H
#define GNG3DEDGE_H

#include <stdlib.h>

#include "Core/GNG3DList.h"

struct GNG3DEdge
{
    int age;
    int check;

    void* b;
    void* e;

    struct GNG3DList b_list; // list of edges for b
    struct GNG3DList e_list; // list of edges for e
    struct GNG3DList list; // main list of edges
};

extern void  GNG3DEdge_Init        (struct GNG3DEdge* edge, struct GNG3DList* list, void* b, struct GNG3DList* b_list, void* e, struct GNG3DList* e_list);
extern void  GNG3DEdge_Dest        (struct GNG3DEdge* edge);
extern void* GNG3DEdge_GetOtherNode(struct GNG3DEdge* edge, void* node);

#endif
