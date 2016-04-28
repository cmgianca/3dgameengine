#include "game/gameentity.h"

GameEntity::GameEntity(QString shape, QString texture, CollisionShape* s, glm::vec3 sc, glm::vec3 pos, glm::vec3 color) :
    Entity(shape, texture, s, color),
    m_type(None)
{
    scale(sc);
    setPosition(pos);
}

GameEntity::~GameEntity() {

}

int GameEntity::getType() {
    return m_type;
}

void GameEntity::setType(Type t) {
    m_type = t;
}


void GameEntity::onCollide(Entity* e, const glm::vec3 &mtv) {
    switch (e->getType()) {
    case None:
        break;
    case t_Player:
        onPlayerCollide(mtv);
        break;
    case t_Bullet:
        onBulletCollide(mtv);
        break;
    case t_Enemy:
        onEnemyCollide(mtv);
        break;
    case t_Object:
        onObjectCollide(mtv);
        break;
    }
}
