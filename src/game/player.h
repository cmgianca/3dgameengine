#ifndef PLAYER_H
#define PLAYER_H

#include "game/gameentity.h"

class GameWorld;

class Player : public GameEntity
{
public:
    Player(GameWorld* w);
    ~Player();

    void jump();

    void setJump(int j);

    bool intersects(int x, int y, int z);

    void onEnemyCollide(const glm::vec3 &);

    void update(GLfloat seconds);

private:
    GameWorld* m_world;
};

#endif // PLAYER_H
