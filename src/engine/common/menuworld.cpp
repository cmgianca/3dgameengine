#include "engine/common/menuworld.h"
#include "engine/common/entity.h"
#include "engine/common/collisiongeometry/nullshape.h"

MenuWorld::MenuWorld(Graphics* g, QString background) :
    World(g),
    background_(NULL)
{
    camera_ = NULL;
    background_ = new Entity("background", background, new NullShape());
}

MenuWorld::~MenuWorld() {
    delete background_;
}

void MenuWorld::drawUI() {
    background_->draw(graphics_);
}
