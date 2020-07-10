#include "GNG3DSimplifiedMesh.h"

GNG3DSimplifiedMesh::GNG3DSimplifiedMesh(aiMesh* mesh)
{
    struct GNG3DNode* node1 = (struct GNG3DNode*) malloc(sizeof(struct GNG3DNode));
    GNG3DNode_Init(node1);
    struct GNG3DNode* node2 = (struct GNG3DNode*) malloc(sizeof(struct GNG3DNode));
    GNG3DNode_Init(node2);

    node1->pos.x = mesh->mVertices[0].x;
    node1->pos.y = mesh->mVertices[0].y;
    node1->pos.z = mesh->mVertices[0].z;

    node2->pos.x = mesh->mVertices[1].x;
    node2->pos.y = mesh->mVertices[1].y;
    node2->pos.z = mesh->mVertices[1].z;

    // init nodes list
    GNG3DLIST_INIT(&nodes);
    GNG3DLIST_LINK_BEFORE(&nodes, &node1->list);
    GNG3DLIST_LINK_BEFORE(&nodes, &node2->list);
    nodes_count = 2;

    // init edges list
    GNG3DLIST_INIT(&edges);
    edges_count = 0;

    UpdateVertices();
    UpdateEdges();
}

GNG3DSimplifiedMesh::~GNG3DSimplifiedMesh()
{
    GNG3DLIST_FOREACH_BEG(&nodes);
    {
        struct GNG3DNode* node = (struct GNG3DNode*) GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);
        GNG3DLIST_UNLINK(&node->list);
        free(node);
    }
    GNG3DLIST_FOREACH_END;

    GNG3DLIST_FOREACH_BEG(&edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM(struct GNG3DEdge, list);
        GNG3DLIST_UNLINK(&edge->list);
        free(edge);
    }
    GNG3DLIST_FOREACH_END;

    if(vertices != NULL) free(vertices);
    if(_edges != NULL) free(_edges);
}


void GNG3DSimplifiedMesh::UpdateVertices(void)
{
    mutex.lock();

    if(vertices != NULL) free(vertices);

    vertices_count = nodes_count;
    vertices = (struct Vec3*) malloc(sizeof(struct Vec3) * vertices_count);

    vertices_winner_index  = -1;
    vertices_winner2_index = -1;

    // prepare nodes for rendering
    int index = 0;
    GNG3DLIST_FOREACH_BEG(&nodes);
    {
        struct GNG3DNode* node = (struct GNG3DNode*) GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);
        vertices[index] = node->pos;

        if(winner == node)       vertices_winner_index  = index;
        else if(winner2 == node) vertices_winner2_index = index;

        //qDebug() << index << " VERTEX " << node->pos.x << "," << node->pos.y  << "," << node->pos.z;
        index++;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::UpdateEdges(void)
{
    mutex.lock();

    if(_edges != NULL) free(_edges);

    _edges_count = edges_count * 2;
    _edges = (struct EdgeInfo*) malloc(sizeof(struct EdgeInfo) * _edges_count);

    // prepare edges for rendering
    int index = 0;
    GNG3DLIST_FOREACH_BEG(&edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM(struct GNG3DEdge, list);

        struct GNG3DNode* b = (struct GNG3DNode*) edge->b;
        struct GNG3DNode* e = (struct GNG3DNode*) edge->e;

        _edges[2*index+0].pos = b->pos;
        _edges[2*index+0].age = edge->age;
        _edges[2*index+1].pos = e->pos;
        _edges[2*index+1].age = edge->age;

        index++;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::UpdateResult(GNG3DResult* result)
{
    mutex.lock();
    result->winner  = vertices_winner_index;
    result->winner2 = vertices_winner2_index;
    mutex.unlock();
}

void GNG3DSimplifiedMesh::SetSignal(struct Vec3* signal)
{
    mutex.lock();
    this->signal = *signal;
    mutex.unlock();
}

void GNG3DSimplifiedMesh::FindWinners(void)
{
    struct Vec3 tmp;

    float winner_distance  = FLT_MAX;
    float winner2_distance = FLT_MAX;

    mutex.lock();

    winner  = NULL;
    winner2 = NULL;

    GNG3DLIST_FOREACH_BEG(&nodes);
    {
        struct GNG3DNode* node = (struct GNG3DNode*) GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);

        Vec3_sub(&tmp, &signal, &node->pos);
        float distance = Vec3_len(&tmp);

        // new winner node found
        if(winner_distance > distance)
        {
            // copy old winner to winner2
            if(winner != NULL)
            {
                winner2 = winner;
                winner2_distance = winner_distance;
            }

            // set winner
            winner = node;
            winner_distance = distance;

            goto cont;
        }

        // new winner 2 node found
        if(winner2_distance > distance)
        {
            // set winner 2
            winner2 = node;
            winner2_distance = distance;

            goto cont;
        }

        cont: ;
    }
    GNG3DLIST_FOREACH_END;

    //qDebug() << "Winner 1 " << winner_distance;
    //qDebug() << "Winner 2 " << winner2_distance;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::MoveNodes(float epsilon_b, float epsilon_n)
{
    mutex.lock();

    struct Vec3 tmp, tmp2;

    Vec3_sub(&tmp, &signal, &winner->pos);
    Vec3_scale(&tmp, epsilon_b);
    Vec3_add(&tmp2, &winner->pos, &tmp);
    winner->pos = tmp2;

    GNG3DLIST_FOREACH_BEG(&winner->edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;
        struct GNG3DNode* node = (struct GNG3DNode*) GNG3DEdge_GetOtherNode(edge, winner);

        Vec3_sub(&tmp, &signal, &node->pos);
        Vec3_scale(&tmp, epsilon_n);
        Vec3_add(&tmp2, &node->pos, &tmp);
        node->pos = tmp2;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}


void GNG3DSimplifiedMesh::IncrementWinnerEdgeAges(void)
{
    mutex.lock();

    GNG3DLIST_FOREACH_BEG(&winner->edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;
        edge->age++;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::IncrementWinnerActivationAndAddError(void)
{
    mutex.lock();

    winner->activation_counter++;

    struct Vec3 tmp;
    Vec3_sub(&tmp, &signal, &winner->pos);
    winner->error_counter += pow(Vec3_len(&tmp), 2);

    mutex.unlock();
}

void GNG3DSimplifiedMesh::EdgeResetOrCreate(void)
{
    mutex.lock();

    struct GNG3DEdge* edge = GNG3DNode_GetEdge(winner, winner2);

    if(edge != NULL)
    {
        // reset age
        edge->age = 0;
    }
    else
    {
        // not connected, then connect them
        struct GNG3DEdge* e = (struct GNG3DEdge*) malloc(sizeof(struct GNG3DEdge));
        GNG3DEdge_Init(e, &edges, winner, &winner->edges, winner2, &winner2->edges);
        edges_count++;
    }

    mutex.unlock();
}

void GNG3DSimplifiedMesh::RemoveOldEdgesAndNodes(int alpha_max)
{
    mutex.lock();

    GNG3DLIST_FOREACH_BEG(&winner->edges);
    {
        struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;

        // if edge age limit reached, remove it
        if(edge->age > alpha_max)
        {
            GNG3DEdge_Dest(edge);
            free(edge);
            edges_count--;
        } 
    }
    GNG3DLIST_FOREACH_END;

    // remove node if no edges
    if(GNG3DLIST_EMPTY(&winner->edges))
    {
        if(nodes_count > 2)
        {
            GNG3DLIST_UNLINK(&winner->list);
            free(winner);
            winner = NULL;
            nodes_count--;
        }
    }

    mutex.unlock();
}

void GNG3DSimplifiedMesh::MultiplyNodeErrorCounters(float d)
{
    mutex.lock();

    GNG3DLIST_FOREACH_BEG(&nodes);
    {
        struct GNG3DNode* node = GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);
        node->error_counter *= d;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::ResetNodeActivationCounters(int mu, int* mu_counter)
{
    mutex.lock();

    if(*mu_counter < mu * nodes_count)
    {
        mutex.unlock();
        return;
    }

    *mu_counter = 0;

    GNG3DLIST_FOREACH_BEG(&nodes);
    {
        struct GNG3DNode* node = (struct GNG3DNode*) GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);

        if(node->activation_counter == 0)
        {
            if(nodes_count > 2)
            {
                GNG3DLIST_FOREACH_BEG(&node->edges);
                {
                    struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;

                    GNG3DEdge_Dest(edge);
                    free(edge);
                    edges_count--;
                }
                GNG3DLIST_FOREACH_END;

                // remove node
                GNG3DLIST_UNLINK(&node->list);
                if(node == winner)  winner = NULL;
                if(node == winner2) winner2 = NULL;
                free(node);
                nodes_count--;
            }
        }
        else node->activation_counter = 0;
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::CreateOrDeleteNode(int max_nodes, float alpha)
{
    mutex.lock();

    if(nodes_count < max_nodes)
    {
        // get node with biggest error

        struct GNG3DNode* _node;
        float error_counter = -1;

        GNG3DLIST_FOREACH_BEG(&nodes);
        {
            struct GNG3DNode* node = (struct GNG3DNode*) GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);
            if(node->error_counter > error_counter)
            {
                // NOTE:
                // The algorithm didn't mention the case when
                // the node with the biggest error doesn't have edges.
                // If so, we just get the next node.
                if(!GNG3DLIST_EMPTY(&node->edges))
                {
                    error_counter = node->error_counter;
                    _node = node;
                }
            }
        }
        GNG3DLIST_FOREACH_END;

        if(_node == NULL)
        {
            mutex.unlock();
            return;
        }

        // find neighbor with biggest error

        struct GNG3DNode* _node2 = GNG3DNode_GetBiggestError(_node);

        // insert new node between those two nodes

        struct GNG3DNode* new_node = (struct GNG3DNode*) malloc(sizeof(struct GNG3DNode));
        GNG3DNode_Init(new_node);
        GNG3DLIST_LINK_BEFORE(&nodes, &new_node->list);
        nodes_count++;

        // find mid point

        struct Vec3 tmp;
        Vec3_sub(&tmp, &_node->pos, &_node2->pos);
        Vec3_scale(&tmp, 0.5);
        Vec3_add(&new_node->pos, &_node2->pos, &tmp);

        // remove old connections

        struct GNG3DEdge* e = GNG3DNode_GetEdge(_node, _node2);
        GNG3DEdge_Dest(e);
        free(e);
        edges_count--;

        // add new connections

        // first new edge
        struct GNG3DEdge* new_edge = (struct GNG3DEdge*) malloc(sizeof(struct GNG3DEdge));
        GNG3DEdge_Init(new_edge, &edges, new_node, &new_node->edges, _node, &_node->edges);
        edges_count++;

        // second new edge
        struct GNG3DEdge* new_edge2 = (struct GNG3DEdge*) malloc(sizeof(struct GNG3DEdge));
        GNG3DEdge_Init(new_edge2, &edges, new_node, &new_node->edges, _node2, &_node2->edges);
        edges_count++;

        // decrease error counter
        _node->error_counter *= alpha;
        _node2->error_counter *= alpha;

        // set error counter
        new_node->error_counter = _node->error_counter;
    }
    else
    {
        // get node with lowest error

        if(nodes_count > 2)
        {
            struct GNG3DNode* _node;
            float error_counter = FLT_MAX;

            GNG3DLIST_FOREACH_BEG(&nodes);
            {
                struct GNG3DNode* node = GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);
                if(node->error_counter < error_counter)
                {
                    error_counter = node->error_counter;
                    _node = node;
                }
            }
            GNG3DLIST_FOREACH_END;

            // remove all edges of this node

            GNG3DLIST_FOREACH_BEG(&_node->edges);
            {
                struct GNG3DEdge* edge = (struct GNG3DEdge*) GNG3DLIST_FOREACH_ITEM2;
                GNG3DEdge_Dest(edge);
                free(edge);
                edges_count--;
            }
            GNG3DLIST_FOREACH_END;

            // remove node
            GNG3DLIST_UNLINK(&_node->list);
            if(_node == winner)  winner = NULL;
            if(_node == winner2) winner2 = NULL;
            free(_node);
            nodes_count--;
        }
    }

    mutex.unlock();
}

void GNG3DSimplifiedMesh::DeleteAllEdges(void)
{
    mutex.lock();

    GNG3DLIST_FOREACH_BEG(&edges);
    {
        struct GNG3DEdge* edge = GNG3DLIST_FOREACH_ITEM(struct GNG3DEdge, list);
        GNG3DEdge_Dest(edge);
        free(edge);
    }
    GNG3DLIST_FOREACH_END;

    mutex.unlock();
}

void GNG3DSimplifiedMesh::SetEdgeIfNotExist(struct GNG3DNode* n1, struct GNG3DNode* n2)
{
    mutex.lock();

    struct GNG3DEdge* edge = GNG3DNode_GetEdge(n1, n2);

    if(edge == NULL)
    {
        // not connected, then connect them
        struct GNG3DEdge* e = (struct GNG3DEdge*) malloc(sizeof(struct GNG3DEdge));
        GNG3DEdge_Init(e, &edges, n1, &n1->edges, n2, &n2->edges);
        edges_count++;
    }

    mutex.unlock();
}





