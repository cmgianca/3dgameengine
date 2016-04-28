#include "engine/voxel/chunk.h"
#include "engine/voxel/voxelmanager.h"
#include "engine/voxel/blocks.h"

Chunk::Chunk(int chunkSize) :
    m_data(new unsigned char[chunkSize*chunkSize*chunkSize])
{
}

Chunk::~Chunk() {
    delete m_data;
}

inline glm::vec2 Chunk::indexToTexel(int index, int texelDimensions) {
    return glm::vec2(1.f*(index%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-index/texelDimensions)/texelDimensions);
}

void Chunk::setBlock(int x, int y, int z, int chunkSize, unsigned char type) {
    m_data[x*chunkSize*chunkSize+z*chunkSize+y] = type;
}

unsigned char Chunk::getType(int x, int y, int z, int chunkSize) {
    return m_data[x*chunkSize*chunkSize+z*chunkSize+y];
}

void Chunk::update() {
}

void Chunk::draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Chunk::genEmpty() {
    GLfloat i[0];
    GLuint j[0];
    setVertexData(i, 0, 0);
    setIndexData(j, 0, 0);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE,  8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

void Chunk::genChunkData(int chunkSize, Blocks* blocks) {
    int texelDimensions = blocks->getTexelDimensions();
    GLfloat* vertices = new GLfloat[8*4*6*chunkSize*chunkSize*chunkSize]; //urg
    GLuint*  indices =  new GLuint[6*6*chunkSize*chunkSize*chunkSize];    //double urg
    unsigned int index = 0;

    for (int x = 0; x < chunkSize; x++) {
        for (int y = 0; y < chunkSize; y++) {
            for (int z = 0; z < chunkSize; z++) {
                if (m_data[x*chunkSize*chunkSize+z*chunkSize+y]) {
                    Block* b = blocks->getBlock(m_data[x*chunkSize*chunkSize+z*chunkSize+y]);
                    if (y+1 >= chunkSize || blocks->getBlock(m_data[x*chunkSize*chunkSize+z*chunkSize+y+1])->transparent) { //top
                        glm::vec2 texelMin = indexToTexel(b->top,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat upVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+1.f,y+1.f,z+1.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
                            x+1.f,y+1.f,z+0.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y,
                            x+0.f,y+1.f,z+0.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
                            x+0.f,y+1.f,z+1.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y
                        };
                        memcpy(vertices + index*32, upVertices, sizeof(upVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                    if (y-1 < 0 || blocks->getBlock(m_data[x*chunkSize*chunkSize+z*chunkSize+y-1])->transparent) { //bottom
                        glm::vec2 texelMin = indexToTexel(b->bottom,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat newVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+1.f,y+0.f,z+1.f,  0.0f,-1.0f, 0.0f,  texelMax.x, texelMax.y,
                            x+0.f,y+0.f,z+1.f,  0.0f,-1.0f, 0.0f,  texelMin.x, texelMax.y,
                            x+0.f,y+0.f,z+0.f,  0.0f,-1.0f, 0.0f,  texelMin.x, texelMin.y,
                            x+1.f,y+0.f,z+0.f,  0.0f,-1.0f, 0.0f,  texelMax.x, texelMin.y};
                        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                    if (z+1 >= chunkSize || blocks->getBlock(m_data[x*chunkSize*chunkSize+(z+1)*chunkSize+y])->transparent) { //right
                        glm::vec2 texelMin = indexToTexel(b->right,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat newVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+1.f,y+1.f,z+1.f,  0.0f, 0.0f, 1.0f,  texelMax.x, texelMax.y,
                            x+0.f,y+1.f,z+1.f,  0.0f, 0.0f, 1.0f,  texelMin.x, texelMax.y,
                            x+0.f,y+0.f,z+1.f,  0.0f, 0.0f, 1.0f,  texelMin.x, texelMin.y,
                            x+1.f,y+0.f,z+1.f,  0.0f, 0.0f, 1.0f,  texelMax.x, texelMin.y};
                        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                    if (z-1 < 0 || blocks->getBlock(m_data[x*chunkSize*chunkSize+(z-1)*chunkSize+y])->transparent) { //left
                        glm::vec2 texelMin = indexToTexel(b->left,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat newVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+1.f,y+1.f,z+0.f,  0.0f, 0.0f,-1.0f,  texelMax.x, texelMax.y,
                            x+1.f,y+0.f,z+0.f,  0.0f, 0.0f,-1.0f,  texelMax.x, texelMin.y,
                            x+0.f,y+0.f,z+0.f,  0.0f, 0.0f,-1.0f,  texelMin.x, texelMin.y,
                            x+0.f,y+1.f,z+0.f,  0.0f, 0.0f,-1.0f,  texelMin.x, texelMax.y};
                        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                    if (x+1 >= chunkSize || blocks->getBlock(m_data[(x+1)*chunkSize*chunkSize+z*chunkSize+y])->transparent) { //back
                        glm::vec2 texelMin = indexToTexel(b->back,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat newVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+1.f,y+1.f,z+1.f,  1.0f, 0.0f, 0.0f,  texelMax.x, texelMax.y,
                            x+1.f,y+0.f,z+1.f,  1.0f, 0.0f, 0.0f,  texelMax.x, texelMin.y,
                            x+1.f,y+0.f,z+0.f,  1.0f, 0.0f, 0.0f,  texelMin.x, texelMin.y,
                            x+1.f,y+1.f,z+0.f,  1.0f, 0.0f, 0.0f,  texelMin.x, texelMax.y};
                        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                    if (x-1 < 0 || blocks->getBlock(m_data[(x-1)*chunkSize*chunkSize+z*chunkSize+y])->transparent) { //front
                        glm::vec2 texelMin = indexToTexel(b->front,texelDimensions);
                        glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
                        GLfloat newVertices[] = {
                            // Positions        // Normals         // Texture Coords
                            x+0.f,y+1.f,z+1.f, -1.0f, 0.0f, 0.0f,  texelMax.x, texelMax.y,
                            x+0.f,y+1.f,z+0.f, -1.0f, 0.0f, 0.0f,  texelMin.x, texelMax.y,
                            x+0.f,y+0.f,z+0.f, -1.0f, 0.0f, 0.0f,  texelMin.x, texelMin.y,
                            x+0.f,y+0.f,z+1.f, -1.0f, 0.0f, 0.0f,  texelMax.x, texelMin.y};
                        memcpy(vertices + index*32, newVertices, sizeof(newVertices));
                        GLuint newIndices[] = {index*4, index*4+1, index*4+3, index*4+1, index*4+2, index*4+3};
                        memcpy(indices + index*6, newIndices, sizeof(newIndices));
                        index++;
                    }
                }
            }
        }
    }
    setVertexData(vertices, sizeof(GLfloat)*index*32, index*4);
    setIndexData(indices, sizeof(GLfloat)*index*6, index*6);
    delete[] vertices;
    delete[]  indices;
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE,  8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}
