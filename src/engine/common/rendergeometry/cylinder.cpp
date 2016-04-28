#include "engine/common/rendergeometry/cylinder.h"
#include "engine/common/rendergeometry/NewCylinderData.h"

Cylinder::Cylinder() : Shape()
{
    setVertexData(vertexBufferData, arraySize, vertexCount);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

Cylinder::~Cylinder()
{
}
