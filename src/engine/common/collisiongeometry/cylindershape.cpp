#include "engine/common/collisiongeometry/boundingbox.h"
#include "engine/common/collisiongeometry/cylindershape.h"

CylinderShape::CylinderShape(glm::vec3 position, GLfloat radius, GLfloat height) :
    CollisionShape(position),
    radius_(radius),
    height_(height)
{
    type_ = Cylinder;
}

CylinderShape::~CylinderShape() {
}

glm::vec3 CylinderShape::collides(CollisionShape* s) {
    return s->collidesWithCylinder(this);
}

glm::vec3 CylinderShape::collidesWithCylinder(CylinderShape* c){
    glm::vec3 pos = c->getPosition();
    glm::vec2 interval1 = glm::vec2(pos.y-c->getHeight()*.5f, pos.y+c->getHeight()*.5f);
    glm::vec2 interval2 = glm::vec2(position_.y-height_*.5f, position_.y+height_*.5f);
    GLfloat m1 = calculateMTV(interval1, interval2);
    if (m1 == -1) {
        return glm::vec3(0.f,0.f,0.f);
    } else {
        glm::vec3 mtv = m1*glm::vec3(0.f,1.f,0.f);
        glm::vec3 dir;
        if (glm::all(glm::equal(glm::vec2(pos.x, pos.z), glm::vec2(position_.x, position_.z)))) {
            dir = glm::vec3(1,0,0);
        } else {
            dir = glm::normalize(glm::vec3(position_.x, 0,position_.z) - glm::vec3(pos.x, 0, pos.z));
        }
        interval1 = glm::vec2(glm::dot(glm::vec3(pos.x,0,pos.z), dir)-c->getRadius(),
                              glm::dot(glm::vec3(pos.x,0,pos.z), dir)+c->getRadius());
        interval2 = glm::vec2(glm::dot(glm::vec3(position_.x, 0, position_.z), dir)-radius_,
                              glm::dot(glm::vec3(position_.x, 0, position_.z), dir)+radius_);
        GLfloat m2 = calculateMTV(interval1, interval2);
        if (m2 == -1) {
            return glm::vec3(0.f,0.f,0.f);

        } else {
            if (glm::abs(m2) < glm::abs(m1)) {
                mtv = m2*dir;
            }

            return mtv+mtv*EPSILON;
        }
    }
}

glm::vec3 CylinderShape::collidesWithAABB(BoundingBox*) {
    return glm::vec3();
}
