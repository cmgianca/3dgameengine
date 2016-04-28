#ifndef BULLET
#define BULLET

#include "game/gameentity.h"

class Bullet : public GameEntity
{
public:
    Bullet(glm::vec3 position, glm::vec3 look);
    ~Bullet();

    void onEnemyCollide(const glm::vec3 & mtv);

    void onObjectCollide(const glm::vec3 &);

    void update(GLfloat seconds);
};

#endif // BULLET
