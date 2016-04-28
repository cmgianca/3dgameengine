#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "game/gameentity.h"

class GameWorld;

class GameObject : public GameEntity
{
public:
    GameObject(QString shape, QString texture, CollisionShape* s, glm::vec3 sc, glm::vec3 pos, glm::vec3 color = glm::vec3(.5f,.5f,.5f));
    ~GameObject();

    void update(GLfloat seconds);

    void onObjectCollide(const glm::vec3 &mtv);
};

#endif // GAMEOBJECT

