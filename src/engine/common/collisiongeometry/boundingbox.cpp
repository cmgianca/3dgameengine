#include "engine/common/collisiongeometry/boundingbox.h"
#include "engine/common/collisiongeometry/cylindershape.h"

BoundingBox::BoundingBox(glm::vec3 position, GLfloat width, GLfloat height, GLfloat depth) :
    CollisionShape(position),
    width_(width),
    height_(height),
    depth_(depth)
{
    type_ = Box;
}

BoundingBox::~BoundingBox() {
}


glm::vec3 BoundingBox::collides(CollisionShape* s) {
    return s->collidesWithAABB(this);
}

glm::vec3 BoundingBox::collidesWithCylinder(CylinderShape*) {
    return glm::vec3(0,0,0);
}

glm::vec3 BoundingBox::collidesWithAABB(BoundingBox* b) {
    glm::vec3 pos = b->getPosition();
    GLfloat xDist = fabs(position_.x - pos.x);
    GLfloat yDist = fabs(position_.y - pos.y);
    GLfloat zDist = fabs(position_.z - pos.z);

    GLfloat cw = (width_/2.f + b->getWidth()/2.f);
    GLfloat ch = (height_/2.f + b->getHeight()/2.f);
    GLfloat cd = (depth_/2.f + b->getDepth()/2.f);

    if (xDist > cw) return glm::vec3(0,0,0);
    if (yDist > ch) return glm::vec3(0,0,0);
    if (zDist > cd) return glm::vec3(0,0,0);

    float ox = cw-xDist;
    float oy = ch-yDist;
    float oz = cd-zDist;

    if (ox < oy && ox < oz) {
        int sign = (position_.x < pos.x) ? 1 : -1;
        return glm::vec3(sign*ox,0,0);
    } else if (oy < ox && oy < oz) {
        int sign = (position_.y < pos.y) ? 1 : -1;
        return glm::vec3(0,sign*oy,0);
    } else {
        int sign = (position_.z < pos.z) ? 1 : -1;
        return glm::vec3(0,0,sign*oz);
    }
}
