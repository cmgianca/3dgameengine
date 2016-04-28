#ifndef STARTWORLD
#define STARTWORLD

#include "engine/common/menuworld.h"

class StartWorld : public MenuWorld
{
public:
    StartWorld(Graphics* g);
    ~StartWorld();

private:
    typedef void (StartWorld::*action)();
    action* actionfunction_;

    enum ActionType{
        Start
    };

    void doActions();

    void loadShapes();

    void loadTextures();

    void loadObjects();

    void bindKeys();

    void bindActions();

    void start();
};

#endif // STARTWORLD

