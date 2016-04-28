#ifndef NULLSHAPE
#define NULLSHAPE

#include "engine/common/collisiongeometry/collisionshape.h"

class NullShape : public CollisionShape
{
public:
    NullShape() : CollisionShape(glm::vec3(0,0,0)) {}
    ~NullShape() {}

    glm::vec3 collides(CollisionShape *) {return glm::vec3(0,0,0);}
    glm::vec3 collidesWithCylinder(CylinderShape*) {return glm::vec3(0,0,0);}
    glm::vec3 collidesWithAABB(BoundingBox*) {return glm::vec3(0,0,0);}
};


#endif // NULLSHAPE

