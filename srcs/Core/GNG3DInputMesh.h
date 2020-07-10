#ifndef GNG3DINPUTMESH_H
#define GNG3DINPUTMESH_H

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "GNG3DVec.h"

class GNG3DInputMesh
{

public:

   GNG3DInputMesh(aiMesh* mesh);
   ~GNG3DInputMesh();

   aiMesh* mesh;
   struct Vec3* vertices;
   int vertices_count;
};

#endif
