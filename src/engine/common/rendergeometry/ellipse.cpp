#include "engine/common/rendergeometry/ellipse.h"
#include "engine/common/rendergeometry/SphereData.h"

Ellipse::Ellipse() : Shape()
{
    setVertexData(sphereVertexBufferData, sphereArraySize, sphereVertexCount);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

Ellipse::~Ellipse()
{
}
