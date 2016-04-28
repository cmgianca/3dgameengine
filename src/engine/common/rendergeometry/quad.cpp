#include "engine/common/rendergeometry/quad.h"

Quad::Quad() : Shape()
{
    GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
         0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    setVertexData(vertices, sizeof(vertices), 4);
    setIndexData(indices, sizeof(indices), 6);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

Quad::~Quad()
{
}

void  Quad::draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
