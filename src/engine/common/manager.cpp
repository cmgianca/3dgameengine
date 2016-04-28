#include "engine/common/manager.h"
#include "engine/common/entity.h"
#include "engine/common/collisiongeometry/collisionshape.h"

Manager::Manager() {

}

Manager::~Manager() {
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i){
       delete (*i);
    }
}

void Manager::update(GLfloat seconds) {
    addAll();
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i)
       (*i)->update(seconds);
    continuousDetection(seconds);
    defaultDetection();
    clean();
}

void Manager::draw(Graphics* g, Camera*) {
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i)
       (*i)->draw(g);
}

void Manager::continuousDetection(GLfloat) {
}

void Manager::defaultDetection() {
    QList<Entity*>::iterator i;
    QList<Entity*>::iterator j;
    for (i = entities_.begin(); i != entities_.end(); ++i){
        if ((*i)->m_collidable) {
            for (j = entities_.begin(); j != entities_.end(); ++j) {
                if ((*i) != (*j) && (*j)->m_collidable) {
                    glm::vec3 mtv = (*i)->getCollisionShape()->collides((*j)->getCollisionShape());
                    if (glm::length2(mtv) != 0) {
                        (*i)->onCollide((*j), mtv);
                        (*j)->onCollide((*i), -1.f*mtv);
                        if (!(*i)->isStatic() && !(*j)->isStatic()) {
                            (*i)->translate(mtv*.5f);
                            (*i)->setVelocity((*i)->getVelocity() - glm::normalize(mtv)*glm::dot((*i)->getVelocity(),(glm::normalize(mtv))));
                            (*j)->translate(mtv*-.5f);
                            (*j)->setVelocity((*j)->getVelocity() - glm::normalize(-1.f*mtv)*glm::dot((*j)->getVelocity(),(glm::normalize(-1.f*mtv))));
                        } else if (!(*i)->isStatic()) {
                            (*i)->translate(mtv*1.f);
                            (*i)->setVelocity((*i)->getVelocity() - glm::normalize(mtv)*glm::dot((*i)->getVelocity(),(glm::normalize(mtv))));
                        } else if (!(*j)->isStatic()) {
                            (*j)->translate(mtv*-1.f);
                            (*j)->setVelocity((*j)->getVelocity() - glm::normalize(-1.f*mtv)*glm::dot((*j)->getVelocity(),(glm::normalize(-1.f*mtv))));
                        }
                    }
                }
            }
        }
    }
}

void Manager::addEntity(Entity* e) {
    toAdd_.append(e);
}

void Manager::removeEntity(Entity* e) {
    toRemove_.append(e);
}

void Manager::addAll() {
    QList<Entity*>::iterator i;
    for (i = toAdd_.begin(); i != toAdd_.end(); ++i)
        entities_.append(*i);
    toAdd_ = QList<Entity*>();
}

void Manager::clean() {
    QList<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i){
        if ((*i)->isDead())
            toRemove_.append(*i);
    }
    for (i = toRemove_.begin(); i != toRemove_.end(); ++i) {
        entities_.removeAll(*i);
        delete (*i);
    }
    toRemove_ = QList<Entity*>();
}
