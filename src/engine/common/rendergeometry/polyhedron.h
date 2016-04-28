#ifndef POLYHEDRON
#define POLYHEDRON

#include "engine/common/rendergeometry/shape.h"

class Polyhedron : public Shape{
public:
    Polyhedron(QList<glm::vec3> vertices, QList<glm::vec3> normals, QList<glm::vec2> texCoords);
    ~Polyhedron();
};

#endif // POLYHEDRON

