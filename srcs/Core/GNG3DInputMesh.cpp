#include "GNG3DInputMesh.h"

GNG3DInputMesh::GNG3DInputMesh(aiMesh* mesh)
{
    this->mesh = mesh;
    this->vertices = (Vec3*)malloc(sizeof(struct Vec3) * mesh->mNumVertices);
    this->vertices_count = mesh->mNumVertices;

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[i].x = mesh->mVertices[i].x;
        vertices[i].y = mesh->mVertices[i].y;
        vertices[i].z = mesh->mVertices[i].z;
    }
}

GNG3DInputMesh::~GNG3DInputMesh()
{
    free(vertices);
}
