#ifndef GAMEWORLD
#define GAMEWORLD

#include "engine/common/world.h"
#include "game/overworld.h"

class Camera;
class GeometricManager;
class Player;
class GameEntity;
class Skybox;

class GameWorld : public World
{
public:
    GameWorld(Graphics* g);
    ~GameWorld();

    void draw();

    void update(GLfloat seconds);

    void mouseMoveEvent(int deltaX, int deltaY);

    glm::vec3 getPlayerPosition();

    void makeWorld(int setting);
private:
    enum ActionType {
        MoveForward,
        MoveBackward,
        MoveLeft,
        MoveRight,
        Hold,
        Sprint,
        Jump,
        Exit
    };
    typedef void (GameWorld::*action)();
    action* actionfunction_;

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void sprint();
    void jump();
    void hold();
    void exit();

    GeometricManager* m_geometricManager;

    Player* m_player;
    GameEntity* polycheck_;

    glm::vec3 goal_;
    glm::vec2 mouseDistance;

    GLfloat goalVelocity;
    glm::vec3 currentGoal;

    Skybox* m_skybox;

    void loadShapes();

    void loadTextures();

    void loadObjects();

    void bindKeys();

    void bindActions();

    void doActions();
};

#endif // GAMEWORLD

