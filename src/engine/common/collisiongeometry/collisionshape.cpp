#include "engine/common/collisiongeometry/collisionshape.h"

CollisionShape::CollisionShape(const glm::vec3 &pos) :
    position_(pos),
    type_(Null)
{
}

CollisionShape::~CollisionShape() {
}

GLfloat CollisionShape::calculateMTV(const glm::vec2 &a, const glm::vec2 &b) {
    assert(b.x < b.y && a.x < a.y);
    GLfloat aRight = b.y - a.x;
    GLfloat aLeft  = a.y - b.x;

    if (aLeft < 0 || aRight < 0)
        return -1;
    if (aRight < aLeft)
        return aRight;
    else
        return -aLeft;
}
