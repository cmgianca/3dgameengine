#include "engine/common/collisiongeometry/boundingbox.h"
#include "game/bullet.h"

Bullet::Bullet(glm::vec3 position, glm::vec3 look) :
    GameEntity("cylinder",
           "",
           new BoundingBox(position,1.f,1.f,1.f),
           glm::vec3(.08f,.02f,.08f),
           position)
{
    translate(look*2.f);
    setVelocity(look*15.f);
    setAcceleration(glm::vec3(0,-9.8f,0));
    m_type = t_Bullet;
}

Bullet::~Bullet() {

}

void Bullet::onEnemyCollide(const glm::vec3 &) {
    die();
}

void Bullet::onObjectCollide(const glm::vec3 &) {
    die();
}

void Bullet::update(GLfloat seconds) {
    Entity::update(seconds);
    if (glm::length2(m_velocity) < 25) {
        die();
    }
}

