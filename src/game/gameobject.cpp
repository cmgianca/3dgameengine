#include "game/gameobject.h"
#include "engine/common/collisiongeometry/polyhedrashape.h"

GameObject::GameObject(QString shape, QString texture, CollisionShape* s, glm::vec3 sc, glm::vec3 pos, glm::vec3 color) :
    GameEntity(shape,texture,s,sc,pos,color)
{
    m_type = t_Object;
    m_collidable = true;
    m_static = true;
}

GameObject::~GameObject() {

}

void GameObject::update(GLfloat seconds) {
    GameEntity::update(seconds);
}

void GameObject::onObjectCollide(const glm::vec3 &) {
    color_ = glm::vec3(1,0,0);
}
