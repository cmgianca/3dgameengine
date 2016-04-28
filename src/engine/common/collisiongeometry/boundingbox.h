#ifndef BOUNDINGBOX
#define BOUNDINGBOX

#include "engine/common/collisiongeometry/collisionshape.h"

class BoundingBox : public CollisionShape
{
public:
    BoundingBox(glm::vec3 position, GLfloat width, GLfloat height, GLfloat depth);
    ~BoundingBox();

    glm::vec3 collides(CollisionShape*);

    glm::vec3 collidesWithCylinder(CylinderShape* c);

    glm::vec3 collidesWithAABB(BoundingBox* b);

    GLfloat getWidth() { return width_; }

    GLfloat getHeight() { return height_; }

    GLfloat getDepth() { return depth_; }

protected:
    GLfloat width_;
    GLfloat height_;
    GLfloat depth_;
};

#endif // BOUNDINGBOX

