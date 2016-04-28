#include "engine/common/rendergeometry/shape.h"

Shape::Shape() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Shape::~Shape() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Shape::setVertexData(GLfloat* vertexData, GLsizeiptr size, int numVertices) {
    m_numVertices = numVertices;

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Shape::setIndexData(GLuint* indexData, GLsizeiptr size, int numIndices) {
    m_numIndices = numIndices;

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indexData, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Shape::setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer) {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(index, size, type, normalized, stride, (GLvoid*) pointer);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); //Unbind VAO
}

void Shape::draw() {
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    glBindVertexArray(0);
}
