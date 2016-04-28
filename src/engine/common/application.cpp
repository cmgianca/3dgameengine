#include "engine/common/application.h"
#include "engine/common/manager.h"
#include "engine/common/world.h"
#include <qgl.h>

Application::Application(View* v) : graphics_(Graphics(v))
{
}

Application::~Application()
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        delete val;
    }
}

void Application::popScreen()
{
    if (screenStack.isEmpty()){
        qFatal("ERROR: Screenstack already empty.");
    } else {
        screenStack.pop_front();
    }
}

void Application::pushScreen(World *s)
{
    screenStack.push_front(s);
}

void Application::paintGL()
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->draw();
        break;
    }
}

void Application::resizeGL(int w, int h)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->resizeGL(w,h);
    }
}

void Application::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->mousePressEvent(event);
        break;
    }
}

void Application::mouseMoveEvent(int deltaX, int deltaY)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->mouseMoveEvent(deltaX, deltaY);
        break;
    }
}

void Application::mouseReleaseEvent(QMouseEvent *event)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->mouseReleaseEvent(event);
        break;
    }
}

void Application::wheelEvent(QWheelEvent *event)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->wheelEvent(event);
        break;
    }
}

void Application::keyPressEvent(QKeyEvent *event)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->keyPressEvent(event);
        break;
    }
}

void Application::keyReleaseEvent(QKeyEvent *event)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->keyReleaseEvent(event);
        break;
    }
}

void Application::update(float seconds)
{
    for (int i = 0; i < screenStack.count(); i++){
        World* val = screenStack.value(i);
        val->update(seconds);
        break;
    }
}
