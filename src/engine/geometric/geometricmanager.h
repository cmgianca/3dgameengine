#ifndef GEOMETRICMANAGER
#define GEOMETRICMANAGER

#include "engine/common/manager.h"
#include "util/CommonIncludes.h"
#include <qlist.h>

class Camera;
class Graphics;
class NavGraph;
class OBJ;
class World;

class GeometricManager : public Manager{
public:
    struct Triangle {
        glm::vec3 vertices[3];
        glm::vec3 normal;
    };

    struct Collision {
        GLfloat t;
        glm::vec3 normal;
    };

    GeometricManager();
    virtual ~GeometricManager();

    void initialize();

    void drawTerrain(Graphics* g);

    void drawNav(Graphics* g);

    GLfloat shootRay(const glm::vec3 &position, const glm::vec3 &direction, GLfloat length, bool navmesh = false);

    void addTerrain(OBJ* o, QString s);

    void addNav(OBJ* o, QString s);

    QList<glm::vec3> findpath(glm::vec3 start, glm::vec3 end);
protected:
    OBJ* terrainmesh_;
    QString terrainrender_;
    OBJ* navmesh_;
    QString navrender_;
    NavGraph* navgraph_;

    void continuousDetection(GLfloat seconds);

    GLfloat solve(GLfloat a, GLfloat b, GLfloat c);

    GLfloat shootRaySphere(glm::vec3 pos, glm::vec3 dir);

    GLfloat shootRayTriangle(Triangle* tri, glm::vec3 pos, glm::vec3 dir);

    Collision sphereTriangleCollision(Triangle* tri, glm::vec3 cdir, GLfloat max);

    glm::vec3 collideEllipse(glm::vec3 position, glm::vec3 radius, glm::vec3 velocity);
};

#endif // GEOMETRICMANAGER
