#include "engine/voxel/blockcover.h"
#include "engine/common/direction.h"

BlockCover::BlockCover(int x, int y, int z, Direction d, int texIndex, int texelDimensions) {
    glm::vec2 texelMin = indexToTexel(texIndex,texelDimensions);
    glm::vec2 texelMax = texelMin+glm::vec2(1.f/texelDimensions,1.f/texelDimensions);
    GLuint indices[] = {0, 1, 3, 1, 2, 3};
    if (d == top) {
        GLfloat vertices[] = {
            // Positions          // Normals         // Texture Coords
            x+1.f,y+1.01f,z+1.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMax.y,
            x+1.f,y+1.01f,z+0.f,  0.0f, 1.0f, 0.0f,  texelMax.x, texelMin.y,
            x+0.f,y+1.01f,z+0.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMin.y,
            x+0.f,y+1.01f,z+1.f,  0.0f, 1.0f, 0.0f,  texelMin.x, texelMax.y};
        setVertexData(vertices, sizeof(vertices), 4);
    } else if (d == bottom) {
        GLfloat vertices[] = {
            // Positions         // Normals         // Texture Coords
            x+1.f,y-.01f,z+1.f,  0.0f,-1.0f, 0.0f,  texelMax.x, texelMax.y,
            x+0.f,y-.01f,z+1.f,  0.0f,-1.0f, 0.0f,  texelMin.x, texelMax.y,
            x+0.f,y-.01f,z+0.f,  0.0f,-1.0f, 0.0f,  texelMin.x, texelMin.y,
            x+1.f,y-.01f,z+0.f,  0.0f,-1.0f, 0.0f,  texelMax.x, texelMin.y};
        setVertexData(vertices, sizeof(vertices), 4);
    } else if (d == right) {
        GLfloat vertices[] = {
            // Positions          // Normals         // Texture Coords
            x+1.f,y+1.f,z+1.01f,  0.0f, 0.0f, 1.0f,  texelMax.x, texelMax.y,
            x+0.f,y+1.f,z+1.01f,  0.0f, 0.0f, 1.0f,  texelMin.x, texelMax.y,
            x+0.f,y+0.f,z+1.01f,  0.0f, 0.0f, 1.0f,  texelMin.x, texelMin.y,
            x+1.f,y+0.f,z+1.01f,  0.0f, 0.0f, 1.0f,  texelMax.x, texelMin.y};
        setVertexData(vertices, sizeof(vertices), 4);
    } else if (d == left) {
        GLfloat vertices[] = {
            // Positions         // Normals         // Texture Coords
            x+1.f,y+1.f,z-.01f,  0.0f, 0.0f,-1.0f,  texelMax.x, texelMax.y,
            x+1.f,y+0.f,z-.01f,  0.0f, 0.0f,-1.0f,  texelMax.x, texelMin.y,
            x+0.f,y+0.f,z-.01f,  0.0f, 0.0f,-1.0f,  texelMin.x, texelMin.y,
            x+0.f,y+1.f,z-.01f,  0.0f, 0.0f,-1.0f,  texelMin.x, texelMax.y};
        setVertexData(vertices, sizeof(vertices), 4);
    } else if (d == back) {
        GLfloat vertices[] = {
            // Positions          // Normals         // Texture Coords
            x+1.01f,y+1.f,z+1.f,  1.0f, 0.0f, 0.0f,  texelMax.x, texelMax.y,
            x+1.01f,y+0.f,z+1.f,  1.0f, 0.0f, 0.0f,  texelMax.x, texelMin.y,
            x+1.01f,y+0.f,z+0.f,  1.0f, 0.0f, 0.0f,  texelMin.x, texelMin.y,
            x+1.01f,y+1.f,z+0.f,  1.0f, 0.0f, 0.0f,  texelMin.x, texelMax.y};
        setVertexData(vertices, sizeof(vertices), 4);
    } else{
        GLfloat vertices[] = {
            // Positions        // Normals         // Texture Coords
            x-.01f,y+1.f,z+1.f, -1.0f, 0.0f, 0.0f,  texelMax.x, texelMax.y,
            x-.01f,y+1.f,z+0.f, -1.0f, 0.0f, 0.0f,  texelMin.x, texelMax.y,
            x-.01f,y+0.f,z+0.f, -1.0f, 0.0f, 0.0f,  texelMin.x, texelMin.y,
            x-.01f,y+0.f,z+1.f, -1.0f, 0.0f, 0.0f,  texelMax.x, texelMin.y};
        setVertexData(vertices, sizeof(vertices), 4);
    }
    setIndexData(indices, sizeof(indices), 6);
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

inline glm::vec2 BlockCover::indexToTexel(unsigned char index, int texelDimensions) {
    return glm::vec2(1.f*(index%texelDimensions)/texelDimensions, 1.f*(texelDimensions-1-index/texelDimensions)/texelDimensions);
}

BlockCover::~BlockCover(){

}

void BlockCover::draw(){
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
