#ifndef COLLISIONSHAPE
#define COLLISIONSHAPE

#include "util/CommonIncludes.h"

class CylinderShape;
class BoundingBox;
class PolyhedraShape;

class CollisionShape
{
public:
    GLfloat const EPSILON = .0001f;

    enum ShapeType{
        Null = 0, Box = 1, Cylinder = 2, Polyhedron = 3
    };

    CollisionShape(const glm::vec3 &pos);
    virtual ~CollisionShape();

    virtual glm::vec3 collides(CollisionShape*) {return glm::vec3(0,0,0);}

    virtual glm::vec3 collidesWithCylinder(CylinderShape*) {return glm::vec3(0,0,0);}

    virtual glm::vec3 collidesWithAABB(BoundingBox*) {return glm::vec3(0,0,0);}

    virtual glm::vec3 collidesWithPoly(PolyhedraShape*) { return glm::vec3(0,0,0);}

    virtual void setScale(const glm::vec3 &) {}

    virtual void setRotate(const glm::quat &) {}

    virtual void setPosition(const glm::vec3 &v) { position_ = v;}

    virtual glm::vec3 getPosition() { return position_;}

    ShapeType getType() {return type_;}

protected:
    glm::vec3 position_;
    ShapeType type_;

    GLfloat calculateMTV(const glm::vec2 &interval1, const glm::vec2 &interval2);
};

#endif // COLLISIONSHAPE

