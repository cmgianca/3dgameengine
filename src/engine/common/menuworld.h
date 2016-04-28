#ifndef MENUWORLD
#define MENUWORLD

#include "engine/common/world.h"

class Entity;

class MenuWorld : public World
{
public:
    MenuWorld(Graphics* g, QString background);
    ~MenuWorld();

    void drawUI();
private:
    Entity* background_;
};


#endif // MENUWORLD
