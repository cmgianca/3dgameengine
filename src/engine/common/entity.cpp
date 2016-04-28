#include "engine/common/collisiongeometry/collisionshape.h"
#include "engine/common/entity.h"
#include "engine/common/graphics.h"

Entity::Entity(QString shape, QString texture, CollisionShape* s, glm::vec3 color):
    m_collidable(false),
    m_canJump(true),
    m_static(false),
    m_dead(false),
    color_(color),
    m_shape(shape),
    m_texture(texture),
    m_collisionShape(s),
    m_scale(glm::vec3(1,1,1)),
    m_rotation(glm::quat(glm::vec3(0,0,0))),
    m_position(glm::vec3(0,0,0)),
    m_velocity(0),
    m_acceleration(0)
{
    glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(), m_position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
    m_model = translationMatrix*rotationMatrix*scaleMatrix;
}

Entity::~Entity() {
}

CollisionShape* Entity::getCollisionShape() {
    return m_collisionShape;
}

void Entity::setColor(const glm::vec3 &c) {
    color_ = c;
}

void Entity::draw(Graphics* g) {
    g->draw(m_shape, m_texture, m_model, color_);
}

void Entity::update(GLfloat seconds) {
    if (!m_static) {
        m_velocity = m_velocity + m_acceleration*seconds;
    }
}

void Entity::scale(glm::vec3 scale) {
    m_scale = scale*m_scale;
    rebuildModel();
    m_collisionShape->setScale(m_scale);
}

void Entity::rotate(glm::vec3 rotation) {
    glm::quat r = glm::quat(rotation);
    m_rotation = r*m_rotation;
    rebuildModel();
    m_collisionShape->setRotate(r);
}

void Entity::translate(const glm::vec3 &translation){
    m_position = m_position + translation;
    m_collisionShape->setPosition(m_position);
    rebuildModel();
}

void Entity::setPosition(const glm::vec3 &v) {
    m_position = v;
    m_collisionShape->setPosition(v);
    rebuildModel();
}

void Entity::rebuildModel() {
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(), m_scale);
    glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(), m_position);
    m_model = translationMatrix*rotationMatrix*scaleMatrix;
}

