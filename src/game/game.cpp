#include "game/game.h"
#include "game/startworld.h"

Game::Game(View* v) : Application(v)
{
}

Game::~Game()
{
}

void Game::initialize()
{
    graphics_.loadShader("default", ":/shaders/shader.vert", ":/shaders/shader.frag");
    graphics_.loadShader("ui", ":/shaders/shader.vert", ":/shaders/shader.frag");
    graphics_.setShader("ui");
    glUseProgram(graphics_.getShader());
    glUniformMatrix4fv(glGetUniformLocation(graphics_.getShader(),"p"),1,GL_FALSE,glm::value_ptr(glm::mat4()));
    glUniformMatrix4fv(glGetUniformLocation(graphics_.getShader(),"v"),1,GL_FALSE,glm::value_ptr(glm::mat4()));
    glUseProgram(0);
    graphics_.setShader("default");
    World* startWorld = new StartWorld(&graphics_);
    startWorld->initialize(0,0);
    pushScreen((World *) startWorld);
}
