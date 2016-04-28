#ifndef POLYHEDRASHAPE
#define POLYHEDRASHAPE

#include "engine/common/collisiongeometry/collisionshape.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include <QList>

struct GJK {
    glm::vec3 v;
    glm::vec3 b, c, d;
    unsigned int n;
};

class PolyhedraShape : public CollisionShape
{
public:
    PolyhedraShape(QList<glm::vec3> vertices);
    ~PolyhedraShape();

    glm::vec3 collides(CollisionShape* c);

    glm::vec3 collidesWithPoly(PolyhedraShape* p);

    void setScale(const glm::vec3 &s);

    void setRotate(const glm::quat &r);

    void setPosition(const glm::vec3 &v);

    QList<glm::vec3> getVertices() { return currentvertices_;}

    BoundingBox* getBoundingBox();

    glm::vec3 supportFunction(glm::vec3 axis);
protected:
    QList<glm::vec3> unrotatedvertices_;
    QList<glm::vec3> currentvertices_;
    BoundingBox* bb_;

    void calculateBoundingBox();
};

#endif // POLYHEDRASHAPE

