#include "engine/common/rendergeometry/skybox.h"

Skybox::Skybox() {
    rebuild(0,0,0);
}

Skybox::~Skybox() {

}

void Skybox::draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Skybox::rebuild(int x, int y, int z) {
    int texelDimensions = 4;
    GLfloat* vertices = new GLfloat[8*6*4];
    GLuint*  indices =  new GLuint[6*6];
    unsigned int index = 0;

    {
        glm::vec2 texelMin = glm::vec2(1.f*(2%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-2/texelDimensions)/texelDimensions);
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                 // Normals         // Texture Coords
            x-10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x+10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y,
            x+10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x-10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y
        };
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }
    {
        glm::vec2 texelMin = glm::vec2(1.f*(10%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-10/texelDimensions)/texelDimensions);
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                // Normals         // Texture Coords
            x-10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x-10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y,
            x+10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x+10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y};
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }
    {
        glm::vec2 texelMin = glm::vec2(1.f*(7%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-7/texelDimensions)/texelDimensions);
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                    // Normals         // Texture Coords
            x-10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x-10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y,
            x+10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x+10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y};
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }
    {
        glm::vec2 texelMin = glm::vec2(1.f*(5%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-5/texelDimensions)/texelDimensions);;
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                    // Normals         // Texture Coords
            x-10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x+10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y,
            x+10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x-10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y};
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }
    {
        glm::vec2 texelMin = glm::vec2(1.f*(4%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-4/texelDimensions)/texelDimensions);
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                    // Normals         // Texture Coords
            x+10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x+10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y,
            x+10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x+10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y};
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }
    {
        glm::vec2 texelMin = glm::vec2(1.f*(6%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-6/texelDimensions)/texelDimensions);
        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
        GLfloat newVertices[] = {
            // Positions                    // Normals         // Texture Coords
            x-10000.f,y-10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x-10000.f,y+10000.f,z-10000.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y,
            x-10000.f,y+10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x-10000.f,y-10000.f,z+10000.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y};
        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
        memcpy(indices + index*6, newIndices, sizeof(newIndices));
        index++;
    }

    setVertexData(vertices, sizeof(GLfloat)*8*6*4, index*4);
    setIndexData(indices, sizeof(GLfloat)*6*6, index*6);
    delete[] vertices;
    delete[]  indices;
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE,  8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}
