#include "game/normalentity.h"

NormalEntity::NormalEntity(QString shape, QString texture, CollisionShape* s) :
    Entity(shape, texture, s)
{
}

NormalEntity::~NormalEntity() {

}

void NormalEntity::onCollide(Entity*, const glm::vec3 &) {
}
