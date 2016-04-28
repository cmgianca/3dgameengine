#include "engine/common/collisiongeometry/polyhedrashape.h"
#include "engine/common/collisiongeometry/boundingbox.h"

PolyhedraShape::PolyhedraShape(QList<glm::vec3> vertices) :
    CollisionShape(glm::vec3(0,0,0)),
    bb_(NULL)
{
    currentvertices_ = vertices;
    unrotatedvertices_ = vertices;
    type_ = Polyhedron;

    glm::vec3 centroid = glm::vec3(0,0,0);
    for (int i = 0; i < currentvertices_.length(); i++) {
        centroid = centroid + currentvertices_.at(i);
    }
    position_ = glm::vec3(0,0,0);
    bb_ = new BoundingBox(centroid,1,1,1);
    calculateBoundingBox();
}

PolyhedraShape::~PolyhedraShape() {
    delete bb_;
}

glm::vec3 PolyhedraShape::collides(CollisionShape* c) {
    return c->collidesWithPoly(this);
}

glm::vec3 doubleCross(const glm::vec3 &a, const glm::vec3 &b)
{
    return glm::cross(glm::cross(a,b),a);
}

bool update(GJK &gjk, glm::vec3 a) { //This monstrosity is GJK
    if (gjk.n == 0) {
        gjk.b = a;
        gjk.v = -a;
        gjk.n = 1;
        return false;
    } else if (gjk.n == 1) {
        gjk.v = doubleCross(gjk.b - a, -1.f*a);
        gjk.c = gjk.b;
        gjk.b = a;
        gjk.n = 2;
        return false;
    } else if (gjk.n == 2) {
        glm::vec3 ao = -1.f*a;
        glm::vec3 ab = gjk.b - a;
        glm::vec3 ac = gjk.c - a;
        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 abp = glm::cross(ab, abc);
        if (glm::dot(abp,ao) > 0) {
            gjk.c = gjk.b;
            gjk.b = a;
            gjk.v = doubleCross(ab, ao);
            return false;
        }
        glm::vec3 acp = glm::cross(abc, ac);
        if (glm::dot(acp, ao) > 0) {
            gjk.b = a;
            gjk.v = doubleCross(ac, ao);
            return false;
        }
        if (glm::dot(abc, ao) > 0) {
            gjk.d = gjk.c;
            gjk.c = gjk.b;
            gjk.b = a;
            gjk.v = abc;
        } else {
            gjk.d = gjk.b;
            gjk.b = a;
            gjk.v = -1.f* abc;
        }
        gjk.n = 3;
        return false;
    } else if (gjk.n == 3) {
        glm::vec3 ao = -a;

        glm::vec3 ab = gjk.b - a;
        glm::vec3 ac = gjk.c - a;
        glm::vec3 ad = gjk.d - a;

        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 acd = glm::cross(ac, ad);
        glm::vec3 adb = glm::cross(ad, ab);

        glm::vec3 tmp;

        int overcount = 0;
        bool overabc = glm::dot(abc, ao) > 0;
        bool overacd = glm::dot(acd, ao) > 0;
        bool overadb = glm::dot(adb, ao) > 0;

        if (overabc)
            overcount++;
        if (overacd)
            overcount++;
        if (overadb)
            overcount++;

        if (overcount == 0) {
            return true;
        }
        if (overcount == 1) {
            if (overacd) {
                gjk.b = gjk.c;
                gjk.c = gjk.d;
                ab = ac;
                ac = ad;
                abc = acd;
            } else if (overadb) {
                gjk.c = gjk.b;
                gjk.b = gjk.d;
                ac = ab;
                ab = ad;
                abc = adb;
            }
            if(glm::dot(glm::cross(abc,ac),ao) > 0 ){
                gjk.b = a;
                gjk.v = doubleCross(ac,ao);
                gjk.n = 2;
                return false;
            }else if(glm::dot(glm::cross(ab,abc),ao) > 0 ){
                gjk.c = gjk.b;
                gjk.b = a;
                gjk.v = doubleCross(ab,ao);
                gjk.n = 2;
                return false;
            } else {
                gjk.d = gjk.c;
                gjk.c = gjk.b;
                gjk.b = a;
                gjk.v = abc;
                gjk.n = 3;
                return false;
            }
        }
        if (overcount == 2) {
            if (overacd && overadb) {
                tmp = gjk.b;
                gjk.b = gjk.c;
                gjk.c = gjk.d;
                gjk.d = tmp;

                tmp = ab;
                ab = ac;
                ac = ad;
                ad = tmp;

                abc = acd;
                acd = adb;
            } else if (overabc && overadb) {
                tmp = gjk.c;
                gjk.c = gjk.b;
                gjk.b = gjk.d;
                gjk.d = tmp;

                tmp = ac;
                ac = ab;
                ab = ad;
                ad = tmp;

                acd = abc;
                abc = adb;
            }
            if(glm::dot(glm::cross(abc,ac),ao) > 0) {
                gjk.b = gjk.c;
                gjk.c = gjk.d;
                ab = ac;
                ac = ad;
                abc = acd;
                if(glm::dot(glm::cross(abc,ac),ao) > 0){
                    gjk.b = a;
                    gjk.v = doubleCross(ac,ao);
                    gjk.n = 2;
                    return false;
                }else if(glm::dot(glm::cross(ab,abc),ao) > 0){
                    gjk.c = gjk.b;
                    gjk.b = a;
                    gjk.v = doubleCross(ab,ao);
                    gjk.n = 2;
                    return false;
                } else {
                    gjk.d = gjk.c;
                    gjk.c = gjk.b;
                    gjk.b = a;
                    gjk.v = abc;
                    gjk.n = 3;
                    return false;
                }
            } else {
                if(glm::dot(glm::cross(ab,abc),ao) > 0){
                    gjk.c = gjk.b;
                    gjk.b = a;
                    gjk.v = doubleCross(ab,ao);
                    gjk.n = 2;
                    return false;
                } else {
                    gjk.d = gjk.c;
                    gjk.c = gjk.b;
                    gjk.b = a;
                    gjk.v = abc;
                    gjk.n = 3;
                    return false;
                }
            }
        }
        if (overcount == 3) {
            std::cout << "Error: this shouldn't happen" << std::endl;
            return true;
        }
    }
    std::cout << "Error: wow this really shouldn't happen" << std::endl;
    return false;
}

glm::vec3 PolyhedraShape::collidesWithPoly(PolyhedraShape* p){
    GJK gjk = {glm::vec3(1.f,0.f,0.f),glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,0),0};
    gjk.v = glm::vec3(1,0,0); //some arbitrary starting vector
    gjk.n = 0;

    int count = 0;
    while (count < 32) {
        glm::vec3 a = supportFunction(gjk.v)-p->supportFunction(-1.f*gjk.v);

        if (glm::dot(a,gjk.v) < 0)
            return glm::vec3(0,0,0);

        if (update(gjk,a))
            return glm::vec3(0,1,0);
        count++;
    }
    return glm::vec3(0,1,0);
}

void PolyhedraShape::setScale(const glm::vec3 &) {
//    for (int i = 0; i < currentvertices_.length(); i++) {
//        glm::vec3 diff = v - currentvertices_.at(i);
//    }
//    calculateBoundingBox();
}

void PolyhedraShape::setRotate(const glm::quat &r) {
    for (int i = 0; i < unrotatedvertices_.length(); i++) {
        currentvertices_.replace(i,position_+(r*(unrotatedvertices_.at(i)-position_)));
    }
    calculateBoundingBox();
}

void PolyhedraShape::setPosition(const glm::vec3 &v) {
    glm::vec3 diff = v - position_;
    position_ = v;
    for (int i = 0; i < currentvertices_.length(); i++) {
        currentvertices_.replace(i, currentvertices_.at(i)+diff);
    }
    for (int i = 0; i < unrotatedvertices_.length(); i++) {
        unrotatedvertices_.replace(i, unrotatedvertices_.at(i)+diff);
    }
    bb_->setPosition(bb_->getPosition()+diff);
}

BoundingBox* PolyhedraShape::getBoundingBox() {
    return bb_;
}

glm::vec3 PolyhedraShape::supportFunction(glm::vec3 axis) {
    GLfloat max = -9999999999999;
    glm::vec3 support = glm::vec3(0,0,0);

    for (int i = 0; i < currentvertices_.length(); i++) {
        GLfloat proj = glm::dot(currentvertices_.at(i),axis);
        if (proj > max) {
            max = proj;
            support = currentvertices_.at(i);
        }
    }
    return support;
}

void PolyhedraShape::calculateBoundingBox() {
    glm::vec3 min = glm::vec3( 9999999999999, 9999999999999, 9999999999999);
    glm::vec3 max = glm::vec3(-9999999999999,-9999999999999,-9999999999999);
    for (int i = 0; i < currentvertices_.length(); i++) {
        min.x = (min.x < currentvertices_.at(i).x) ? min.x : currentvertices_.at(i).x;
        min.y = (min.y < currentvertices_.at(i).y) ? min.y : currentvertices_.at(i).y;
        min.z = (min.z < currentvertices_.at(i).z) ? min.z : currentvertices_.at(i).z;
        max.x = (max.x > currentvertices_.at(i).x) ? max.x : currentvertices_.at(i).x;
        max.y = (max.y > currentvertices_.at(i).y) ? max.y : currentvertices_.at(i).y;
        max.z = (max.z > currentvertices_.at(i).z) ? max.z : currentvertices_.at(i).z;
    }
    glm::vec3 center = .5f*(min+max);
    delete bb_;
    bb_ = new BoundingBox(center,max.x-min.x, max.y-min.y, max.z-min.z);
}
