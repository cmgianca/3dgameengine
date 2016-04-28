#ifndef ENEMY
#define ENEMY

#include "game/gameentity.h"

class GameWorld;

class Enemy : public GameEntity
{
public:
    Enemy(glm::vec3 pos, GameWorld* w);
    ~Enemy();

    void onBulletCollide(const glm::vec3 &);

    void onObjectCollide(const glm::vec3 &);

    void update(GLfloat seconds);

    void jump();
private:
    int jumps;
    GameWorld* m_world;
};

#endif // ENEMY
