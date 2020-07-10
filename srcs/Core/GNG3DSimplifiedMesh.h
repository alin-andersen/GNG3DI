#ifndef GNG3DSIMPLIFIEDMESH_H
#define GNG3DSIMPLIFIEDMESH_H

#include <float.h>
#include <vector>

#include <assimp/mesh.h>

#include "Core/GNG3DVec.h"
#include "Core/GNG3DResult.h"
#include "Core/GNG3DList.h"
#include "Core/GNG3DEdge.h"
#include "Core/GNG3DNode.h"

#include <QtDebug>
#include <QMutex>

struct EdgeInfo
{
    struct Vec3 pos;
    int age;
};

class GNG3DSimplifiedMesh
{

public:

    QMutex mutex;

    struct Vec3 signal;
    GNG3DNode* winner  = NULL;
    GNG3DNode* winner2 = NULL;

    GNG3DSimplifiedMesh(aiMesh* mesh);
    ~GNG3DSimplifiedMesh();

    // used for the GNG3D algorithm
    int nodes_count;
    struct GNG3DList nodes;
    int edges_count;
    struct GNG3DList edges;

    // used for rendering
    int vertices_count;
    struct Vec3* vertices = NULL;
    void UpdateVertices(void);

    int vertices_winner_index  = -1;
    int vertices_winner2_index = -1;

    // used for rendering
    int _edges_count;
    struct EdgeInfo* _edges = NULL;
    void UpdateEdges(void);

    // used for rendering; updates the winner indices
    void UpdateResult(GNG3DResult* result);

    void SetSignal                     (struct Vec3* signal);
    void FindWinners                   (void);
    void MoveNodes                     (float epsilon_b, float epsilon_n);
    void IncrementWinnerEdgeAges       (void);
    void IncrementWinnerActivationAndAddError(void);
    void EdgeResetOrCreate             (void);
    void RemoveOldEdgesAndNodes        (int alpha_max);
    void MultiplyNodeErrorCounters     (float d);
    void CreateOrDeleteNode            (int max_nodes, float alpha);
    void ResetNodeActivationCounters   (int mu, int* mu_counter);

    void DeleteAllEdges(void);
    void SetEdgeIfNotExist(struct GNG3DNode* n1, struct GNG3DNode* n2);
};

#endif
