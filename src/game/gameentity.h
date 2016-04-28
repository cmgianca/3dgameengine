#ifndef GAMEENTITY
#define GAMEENTITY

#include "engine/common/entity.h"

enum Type {None = 0, t_Player, t_Bullet, t_Enemy, t_Object, t_Dew};

class GameEntity : public Entity
{
public:
    GameEntity(QString shape, QString texture, CollisionShape* s, glm::vec3 sc, glm::vec3 pos, glm::vec3 color = glm::vec3(.5f,.5f,.5f));
    virtual ~GameEntity();

    void onCollide(Entity* e, const glm::vec3 &mtv);

    void setType(Type t);

    int getType();

    virtual void onPlayerCollide(const glm::vec3 &) {}

    virtual void onBulletCollide(const glm::vec3 &) {}

    virtual void onEnemyCollide(const glm::vec3 &) {}

    virtual void onObjectCollide(const glm::vec3 &) {}
protected:
    Type m_type;
};


#endif // GAMEENTITY

