#ifndef SHAPE
#define SHAPE

#include "util/CommonIncludes.h"
#include <qgl.h>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    virtual void draw();

    void setVertexData(GLfloat* vertexData, GLsizeiptr size, int numVertices);

    void setIndexData(GLuint* indexData, GLsizeiptr size, int numIndices);

    void setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer);

protected:
    GLuint m_VBO, m_VAO, m_EBO;
    int m_numVertices;
    int m_numIndices;
};


#endif // SHAPE

