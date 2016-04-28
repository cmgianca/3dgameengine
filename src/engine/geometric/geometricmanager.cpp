#include "engine/common/camera.h"
#include "engine/common/graphics.h"
#include "engine/common/world.h"
#include "engine/common/entity.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include "engine/common/rendergeometry/obj.h"
#include "engine/geometric/geometricmanager.h"
#include "engine/geometric/navgraph.h"


GeometricManager::GeometricManager() :
    terrainmesh_(NULL),
    navmesh_(NULL),
    navgraph_(NULL)
{
}

GeometricManager::~GeometricManager() {
    delete terrainmesh_;
    delete navmesh_;
    delete navgraph_;
}

void GeometricManager::addTerrain(OBJ* o, QString s) {
    terrainmesh_ = o;
    terrainrender_ = s;
}
void GeometricManager::addNav(OBJ* o, QString s) {
    navmesh_ = o;
    navrender_ = s;
    navgraph_ = new NavGraph(o);

}

void GeometricManager::initialize() {
}

QList<glm::vec3> GeometricManager::findpath(glm::vec3 start, glm::vec3 end) {
    if (navgraph_ != NULL) {
        return navgraph_->findpath(start,end);
    } else {
        return QList<glm::vec3>();
    }
}

void GeometricManager::drawTerrain(Graphics* g) {
    if (terrainmesh_ != NULL) {
        g->draw(terrainrender_,"",glm::mat4());
    }
}

void GeometricManager::drawNav(Graphics* g) {
    if (navrender_ != NULL) {
        g->draw(navrender_,"",glm::translate(glm::mat4(),glm::vec3(0,.1f,0)), glm::vec3(1.f, 8.f, 0.f));
    }
}

void GeometricManager::continuousDetection(GLfloat seconds) {
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i) {
        bool jump = false;
        if (terrainmesh_ != NULL && (*i)->m_collidable){
            for (int j = 0; j < 4; j++) {  // 4 sweep iterations
                if (glm::length2((*i)->getVelocity()) != 0) {
                    glm::vec3 position = (*i)->getPosition();
                    glm::vec3 velocity = (*i)->getVelocity()*seconds;
                    glm::vec3 translation = -(*i)->getPosition();
                    glm::vec3 scale = glm::vec3(2.f/(*i)->getScale().x, 2.f/(*i)->getScale().y, 2.f/(*i)->getScale().z);
                    glm::vec3 cdir = glm::normalize(velocity);
                    GLfloat max = glm::length(position+velocity - position);
                    cdir = glm::vec3(cdir.x*scale.x, cdir.y*scale.y, cdir.z*scale.z);

                    GLfloat minimum = 99999999;
                    glm::vec3 normal = glm::vec3(0,1,0);

                    QList<OBJ::Triangle*>::iterator tri;
                    for (tri = terrainmesh_->triangles.begin(); tri != terrainmesh_->triangles.end(); ++tri) {
                        glm::vec3 newV0 = (*tri)->vertices[0] + translation;
                        glm::vec3 newV1 = (*tri)->vertices[1] + translation;
                        glm::vec3 newV2 = (*tri)->vertices[2] + translation;
                        newV0 = glm::vec3(newV0.x*scale.x,newV0.y*scale.y,newV0.z*scale.z);
                        newV1 = glm::vec3(newV1.x*scale.x,newV1.y*scale.y,newV1.z*scale.z);
                        newV2 = glm::vec3(newV2.x*scale.x,newV2.y*scale.y,newV2.z*scale.z);
                        Triangle modified = {{newV0, newV1, newV2}, glm::normalize(glm::cross(newV1 - newV0, newV2 - newV0))};
                        Collision t = sphereTriangleCollision(&modified, cdir, max);
                        if (t.t < minimum) {
                            minimum = t.t;
                            normal = glm::normalize(glm::vec3(t.normal.x*scale.x, t.normal.y*scale.y, t.normal.z*scale.z));
                        }
                    }

                    if (minimum < max) {
                        (*i)->setPosition(position+minimum*glm::normalize(position+velocity - position) + normal*.0001f);
                        glm::vec3 normalVelocity = glm::dot((*i)->getVelocity(),normal)*normal;
                        if (glm::length2(normalVelocity) != 0) {
                            (*i)->setVelocity((*i)->getVelocity()-1.01f*normalVelocity);
                        }
                        seconds -= seconds*(minimum/max);
                        if (normal.y > .5) {
                            jump = true;
                        }
                    } else {
                        (*i)->setPosition(position+velocity);
                        break;
                    }
                }
            }
            (*i)->m_canJump = jump;
        } else {
            (*i)->setPosition((*i)->getPosition()+(*i)->getVelocity()*seconds);
        }
    }
}

GLfloat GeometricManager::shootRay(const glm::vec3 &position, const glm::vec3 &direction, GLfloat length, bool navmesh) {
    GLfloat minimum = 99999999;
    OBJ* mesh = navmesh ? navmesh_ : terrainmesh_;
    QList<OBJ::Triangle*>::iterator tri;
    for (tri = mesh->triangles.begin(); tri != mesh->triangles.end(); ++tri) {
        Triangle triangle = {{(*tri)->vertices[0], (*tri)->vertices[1], (*tri)->vertices[2]}, glm::normalize(glm::cross((*tri)->vertices[1] - (*tri)->vertices[0], (*tri)->vertices[2] - (*tri)->vertices[0]))};
        GLfloat t = shootRayTriangle(&triangle, position, direction);
        if (t != 0 && t < minimum) {
            minimum = t;
        }
    }

    if (minimum < length) {
        return minimum;
    } else {
        return 0;
    }
}

GLfloat GeometricManager::solve(GLfloat a, GLfloat b, GLfloat c) {
    GLfloat determinant = b*b - 4*a*c;
    if (determinant == 0) {
        return (-b/(2*a));
    } else if (determinant > 0) {
        GLfloat val1 = (-b+sqrt(determinant))/(2*a);
        GLfloat val2 = (-b-sqrt(determinant))/(2*a);
        return (val1 < val2) ? val1 : val2;
    }
    return 0;
}

GLfloat GeometricManager::shootRaySphere(glm::vec3 pos, glm::vec3 dir) {
    GLfloat a = dir.x*dir.x + dir.y*dir.y + dir.z*dir.z;
    GLfloat b = 2*pos.x*dir.x + 2*pos.y*dir.y + 2*pos.z*dir.z;
    GLfloat c = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z - 1;
    return solve(a,b,c);
}

GLfloat GeometricManager::shootRayTriangle(Triangle* tri, glm::vec3 pos, glm::vec3 dir) {
    if (glm::dot(tri->normal,dir) == 0) {
        return 0;
    } else {
        GLfloat t = -(glm::dot(tri->normal,pos-tri->vertices[0])/(glm::dot(tri->normal,dir)));
        if (t <= 0) {
            return 0;
        } else {
            glm::vec3 collisionPoint = pos + t*dir;
            glm::vec3 normal1 = glm::cross(tri->vertices[0]-collisionPoint, tri->vertices[1]-collisionPoint);
            glm::vec3 normal2 = glm::cross(tri->vertices[1]-collisionPoint, tri->vertices[2]-collisionPoint);
            glm::vec3 normal3 = glm::cross(tri->vertices[2]-collisionPoint, tri->vertices[0]-collisionPoint);
            if (glm::dot(normal1,normal2) > 0 && glm::dot(normal2,normal3) > 0 && glm::dot(normal3,normal1) > 0) {
                return t;
            } else {
                return 0;
            }
        }
    }
}

GeometricManager::Collision GeometricManager::sphereTriangleCollision(Triangle* tri, glm::vec3 cdir, GLfloat max) {
    // Interior collision
    glm::vec3 endPoint = cdir;
    GLfloat t = shootRayTriangle(tri,-tri->normal,cdir);
    if (t > 0 && t < max && glm::dot(tri->normal,cdir) < 0) {
        return {t, tri->normal};
    }
    // Edge collision
    Collision toReturn;
    toReturn.t = max;
    toReturn.normal = glm::vec3(0,1,0);
    for (int i = 0; i < 3; i++) {
        int c = i;
        int d = (i+1) % 3;
        GLfloat aQuad = glm::length2(glm::cross(endPoint,tri->vertices[d]-tri->vertices[c]));
        GLfloat bQuad = 2.f*glm::dot(glm::cross(endPoint,tri->vertices[d]-tri->vertices[c]),
                               glm::cross(-1.f*tri->vertices[c],tri->vertices[d]-tri->vertices[c]));
        GLfloat cQuad = glm::length2(glm::cross(-1.f*tri->vertices[c],tri->vertices[d]-tri->vertices[c])) -
                glm::length2(tri->vertices[d]-tri->vertices[c]);
        GLfloat result = solve(aQuad,bQuad,cQuad);
        if (result > 0 && result < max) {
            glm::vec3 point = cdir*result;
            GLfloat check = glm::dot(point-tri->vertices[c],tri->vertices[d]-tri->vertices[c]);
            if (0 < check && check < glm::length2(tri->vertices[d]-tri->vertices[c])) {
                if (result < toReturn.t) {
                    toReturn.t = result;
                    toReturn.normal = -1.f*glm::normalize(glm::cross(glm::cross(tri->vertices[d]-tri->vertices[c],tri->vertices[d]-point),tri->vertices[d]-tri->vertices[c]));
                }
            }
        }
    }

   // Vertex collision
    for (int i = 0; i < 3; i++) {
        GLfloat t = shootRaySphere(tri->vertices[i], -1.f*cdir);
        if (t > 0 && t < toReturn.t) {
            toReturn.t = t;
            toReturn.normal = cdir*t;
        }
    }
    return toReturn;
}
