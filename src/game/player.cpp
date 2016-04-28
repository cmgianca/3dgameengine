#include "engine/common/application.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include "game/gameworld.h"
#include "game/player.h"

Player::Player(GameWorld* w) :
    GameEntity("ellipse",
           "",
           new BoundingBox(glm::vec3(0.f,2.f,0.f),.5f,1.0f,.5f),
           glm::vec3(.5f,1.0f,.5f),
           glm::vec3(0.f,2.f,0.f)),
    m_world(w)
{
    m_collidable = true;
}

Player::~Player() {
}

void Player::update(GLfloat seconds) {
    Entity::update(seconds);
}

void Player::jump() {
    if (m_canJump) {
        setVelocity(m_velocity + glm::vec3(0,7,0));
        m_canJump = false;
    }
}

bool Player::intersects(int x, int y, int z) {
    glm::vec3 pos = m_collisionShape->getPosition();
    BoundingBox* box = (BoundingBox *) m_collisionShape;
    if (pos.x+box->getWidth()/2 < x) return false;
    if (pos.x-box->getWidth()/2 > x+1) return false;
    if (pos.y+box->getHeight()/2 < y) return false;
    if (pos.y-box->getHeight()/2 > y+1) return false;
    if (pos.z+box->getDepth()/2 < z) return false;
    if (pos.z-box->getDepth()/2 > z+1) return false;
    return true;
}

void  Player::onEnemyCollide(const glm::vec3 &) {
}
