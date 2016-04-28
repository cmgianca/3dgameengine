#ifndef CYLINDERSHAPE
#define CYLINDERSHAPE

#include "engine/common/collisiongeometry/collisionshape.h"

class CylinderShape : public CollisionShape
{
public:
    CylinderShape(glm::vec3 position, GLfloat radius, GLfloat height);
    ~CylinderShape();

    glm::vec3 collides(CollisionShape* c);

    glm::vec3 collidesWithCylinder(CylinderShape* c);

    glm::vec3 collidesWithAABB(BoundingBox* c);

    GLfloat getRadius() { return radius_; }

    GLfloat getHeight() { return height_; }

protected:
    GLfloat radius_;
    GLfloat height_;
};

#endif // CYLINDERSHAPE
