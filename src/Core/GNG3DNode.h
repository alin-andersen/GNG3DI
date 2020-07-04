#ifndef GNG3DNODE_H
#define GNG3DNODE_H

#include <vector>
#include <QDebug>

#include "Core/GNG3DEdge.h"
#include "Core/GNG3DVec.h"
#include "Core/GNG3DList.h"

struct  GNG3DNode
{
    int check;
    struct Vec3 pos;
    int activation_counter;
    float error_counter;

    struct GNG3DList list;  // main list of nodes
    struct GNG3DList edges; // list of connected edges
};

extern void              GNG3DNode_Init            (struct GNG3DNode* node);
extern struct GNG3DEdge* GNG3DNode_GetEdge         (struct GNG3DNode* n1,   struct GNG3DNode* n2);
extern struct GNG3DNode* GNG3DNode_GetBiggestError (struct GNG3DNode* node);

#endif // GNG3DNODE_H
