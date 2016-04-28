#ifndef APPLICATION
#define APPLICATION

#include "engine/common/graphics.h"
#include <QList>

class World;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

/**
 * @brief The Application class
 */
class Application
{
public:
    Application(View* v);

    ~Application();

    virtual void initialize() = 0;

    void static popScreen();

    void static pushScreen(World* s);

    void paintGL();

    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(int deltaX, int deltaY);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void update(float seconds);

    static QList<World*> screenStack;

protected:
    Graphics graphics_;
};

#endif // APPLICATION

