#ifndef GRAPHICSOBJECT
#define GRAPHICSOBJECT

#include "util/CommonIncludes.h"
#include <QEvent>
#include <QList>
#include <QMap>
#include <QKeyEvent>

class Camera;
class Entity;
class Graphics;
class Manager;
class OBJ;
class QKeyEvent;
class QInputEvent;
class QMouseEvent;
class QWheelEvent;

/**
 * @brief The GraphicsObject class
 */
class World
{
public:
    World(Graphics* g);
    virtual ~World();

    virtual void initialize(int w, int h);

    virtual void draw();

    virtual void update(GLfloat);

    virtual void mouseMoveEvent(int, int) {}

    virtual void wheelEvent(QWheelEvent* ) {}

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    virtual void resizeGL(int, int);

    int getActionCount() { return actioncount_;}

    Camera* camera_;
protected:
    QMap<int, int> bindings_;
    int* actionactive_;
    int actioncount_;

    Graphics* graphics_;
    Manager* manager_;
    int width_,height_;

    void bindKey(int key, int value);

    void unbindKey(int typevalue);

    virtual void doActions() = 0;

    virtual void loadTextures() = 0;

    virtual void loadShapes() = 0;

    virtual void loadObjects() = 0;

    virtual void bindKeys() = 0;

    virtual void bindActions() = 0;

    virtual void drawUI() {}
};

#endif // GRAPHICSOBJECT

