#ifndef NAVGRAPH
#define NAVGRAPH

#include "util/CommonIncludes.h"
#include <QList>

class OBJ;

class NavGraph
{
public:
    NavGraph(OBJ *o);
    ~NavGraph();

    struct Triangle {
        glm::vec3 vertices[3];
        glm::vec3 normal;
    };

    struct Portal {
        int index1;
        int index2;
        glm::vec3 center;
        glm::vec3 leftpoint;
        glm::vec3 rightpoint;
    };

    QList<glm::vec3> findpath(glm::vec3 start, glm::vec3 end);

private:
    int trianglecount_;
    int portalcount_;
    Triangle* triangles_;
    Portal* portals_;

    int query(glm::vec3 point);
    GLfloat shootRayTriangle(Triangle* tri, glm::vec3 pos, glm::vec3 dir);
    QList<int> Dijkstras(glm::vec3 start, glm::vec3 end);
};

#endif // NAVGRAPH

