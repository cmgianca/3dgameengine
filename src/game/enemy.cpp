#include "engine/common/collisiongeometry/cylindershape.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include "game/enemy.h"
#include "game/gameworld.h"

Enemy::Enemy(glm::vec3 pos, GameWorld* w) :
    GameEntity("cylinder",
           "",
           new BoundingBox(pos, .5f, 1.8f, .5f),
           glm::vec3(.5f,1.8f,.5f),
           pos),
    jumps(1),
    m_world(w)
{
    m_type = t_Enemy;
    setAcceleration(glm::vec3(0,-9.8,0));
}

Enemy::~Enemy() {
}

void Enemy::update(GLfloat seconds) {
    glm::vec3 direction = glm::normalize(m_world->getPlayerPosition()-m_position);
    glm::vec3 goal = 10.f*(glm::vec3(7.f*direction.x, 0, 7.f*direction.z)-glm::vec3(m_velocity.x, 0, m_velocity.z));
    setAcceleration(goal+glm::vec3(0,-9.8,0));
    if (m_velocity.y == 0) {
        jumps = 1;
    } else {
        jumps = 0;
    }
    if (glm::length2(m_velocity) < .25f) {
        jump();
    }
    Entity::update(seconds);
    if (m_position.y < 0) {
        die();
    }
}

void Enemy::onBulletCollide(const glm::vec3 &) {
    die();
}

void Enemy::onObjectCollide(const glm::vec3 &mtv) {
    if (mtv.y > .1f) {
        jumps = 1;
    }
}

void Enemy::jump() {
    if (jumps > 0) {
        m_velocity = glm::vec3(m_velocity.x, 6.f, m_velocity.z);
        jumps = jumps - 1;
    }
}

