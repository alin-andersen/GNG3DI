#include "Core/GNG3DNode.h"

void GNG3DNode_Init(struct GNG3DNode* node)
{
    node->check = 69;
    GNG3DLIST_INIT(&node->edges);
    node->activation_counter = 0;
    node->error_counter = 0.0;
    Vec3_init(&node->pos, 0, 0, 0);
}

struct GNG3DEdge* GNG3DNode_GetEdge(struct GNG3DNode* node1, struct GNG3DNode* node2)
{
    GNG3DLIST_FOREACH_BEG(&node1->edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;
        if(edge->b == node2 || edge->e == node2)
            return edge;
    }
    GNG3DLIST_FOREACH_END;

    return NULL;
}

struct GNG3DNode* GNG3DNode_GetBiggestError(struct GNG3DNode* node)
{
    struct GNG3DNode* _node2 = NULL;
    int error_counter = -1;

    int index = 0;
    GNG3DLIST_FOREACH_BEG(&node->edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;
        struct GNG3DNode* _node = (struct GNG3DNode*) GNG3DEdge_GetOtherNode(edge, node);

        if(_node->error_counter > error_counter)
        {
            error_counter = _node->error_counter;
            _node2 = _node;
        }
        index++;
    }
    GNG3DLIST_FOREACH_END;

    return _node2;
}


