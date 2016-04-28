#include "engine/common/rendergeometry/background.h"

Background::Background() : Shape()
{
    GLfloat vertices[] = {
        -1.f, -1.f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom left
         1.f, -1.f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom right
         1.f,  1.f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // Top right
         1.f,  1.f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // Top right
        -1.f,  1.f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // Top left
        -1.f, -1.f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f  // bottom left
    };
    setVertexData(vertices, sizeof(vertices), 6);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

Background::~Background()
{
}
