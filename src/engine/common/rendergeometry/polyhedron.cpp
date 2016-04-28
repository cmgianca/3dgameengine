#include "engine/common/rendergeometry/polyhedron.h"

Polyhedron::Polyhedron(QList<glm::vec3> vertices, QList<glm::vec3> normals, QList<glm::vec2> texCoords)
    : Shape()
{
    GLfloat vertexArray[vertices.length()*8];
    for (int i = 0; i < vertices.length(); i++) {
        vertexArray[i*8+0] = vertices.at(i).x;
        vertexArray[i*8+1] = vertices.at(i).y;
        vertexArray[i*8+2] = vertices.at(i).z;
        vertexArray[i*8+3] = normals.at(i).x;
        vertexArray[i*8+4] = normals.at(i).y;
        vertexArray[i*8+5] = normals.at(i).z;
        vertexArray[i*8+6] = texCoords.at(i).x;
        vertexArray[i*8+7] = texCoords.at(i).y;
    }
//    for (int i = 0; i < vertices.length(); i++) {
//        for (int j = 0; j < 8; j++) {
//            std::cout << vertexArray[i*8+j] << ",";
//        }
//        std::cout << std::endl;
//    }
    setVertexData(vertexArray, sizeof(vertexArray), vertices.length());
    setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    setAttribute(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));
}

Polyhedron::~Polyhedron()
{
}

