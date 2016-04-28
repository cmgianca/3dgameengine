#include "engine/common/application.h"
#include "engine/common/collisiongeometry/nullshape.h"
#include "engine/common/rendergeometry/background.h"
#include "game/gameworld.h"
#include "game/normalentity.h"
#include "game/startworld.h"
#include <QKeyEvent>

StartWorld::StartWorld(Graphics *g) : MenuWorld(g, "start")
{
    actioncount_ = 1;
    actionfunction_ = new action[actioncount_];
    for (int i = 0; i < actioncount_ ; i++) {
       actionfunction_[i] = NULL;
    }
}

StartWorld::~StartWorld()
{
    delete[] actionfunction_;
}

void StartWorld::start() {
    GameWorld* gameScreen = new GameWorld(graphics_);
    gameScreen->initialize(width_,height_);
    Application::screenStack.push_front(gameScreen);
    actionactive_[Start] = false;
}

void StartWorld::bindKeys() {
    bindKey(Start, Qt::Key_Space);
}

void StartWorld::bindActions() {
    actionfunction_[Start] = &start;
}

void StartWorld::doActions() {
    for (int i = 0; i < actioncount_; i++) {
        if (actionactive_[i]) {
            if (actionfunction_[i]) {
                (this->*actionfunction_[i])();
            }
        }
    }
}

void StartWorld::loadTextures()
{
    graphics_->loadTexture("start.png", "start");
}

void StartWorld::loadShapes()
{
    graphics_->loadShape("background", new Background());
}

void StartWorld::loadObjects()
{
}
