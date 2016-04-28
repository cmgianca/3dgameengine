#include "engine/geometric/rendermesh.h"

RenderMesh::RenderMesh(OBJ* o) : Shape()
{
    GLfloat* vertices = new GLfloat[o->vboData.size()];
    for (int i = 0; i < o->vboData.size(); i++) {
        vertices[i] = o->vboData.at(i);
    }
    setVertexData(vertices, sizeof(GLfloat)*o->vboData.size(), o->vertexCount);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
    delete[] vertices;
}

RenderMesh::~RenderMesh()
{
}
