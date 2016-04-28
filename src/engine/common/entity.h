#ifndef ENTITY
#define ENTITY

#include "util/CommonIncludes.h"
#include <QString>

class CollisionShape;
class Graphics;

class Entity
{
public:
    Entity(QString shape, QString texture, CollisionShape* s, glm::vec3 color = glm::vec3(.5f, .5f, .5f));
    virtual ~Entity();

    virtual void draw(Graphics* g);

    virtual void update(GLfloat seconds);

    CollisionShape* getCollisionShape();

    virtual int getType() {return 0;}

    virtual void onCollide(Entity*, const glm::vec3 &) {}

    glm::vec3 getPosition() {return m_position;}
    virtual void setPosition(const glm::vec3 &v);

    glm::vec3 getVelocity() {return m_velocity;}
    void setVelocity(const glm::vec3 &v) {m_velocity = v;}

    glm::vec3 getAcceleration() {return m_acceleration;}
    void setAcceleration(const glm::vec3 &v) {m_acceleration = v;}


    glm::vec3 getScale() {return m_scale;}
    void scale(glm::vec3 scale);

    void rotate(glm::vec3 rotation);

    virtual void translate(const glm::vec3 &translation);

    bool isStatic() {return m_static;}

    void setStatic(bool s) {m_static = s;}

    bool isDead() {return m_dead;}

    void setColor(const glm::vec3 &c);

    bool m_collidable;
    bool m_canJump;
protected:
    virtual void die() {m_dead = true;}
    bool m_static;
    bool m_dead;
    glm::vec3 color_;

    QString m_shape;
    QString m_texture;
    CollisionShape* m_collisionShape;

    glm::vec3 m_scale;
    glm::quat m_rotation;
    glm::vec3 m_position;

    glm::mat4 m_model;

    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    void rebuildModel();
};

#endif // ENTITY

