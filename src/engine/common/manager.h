#ifndef MANAGER
#define MANAGER

#include "engine/common/raycollision.h"
#include <util/CommonIncludes.h>
#include <QList>

class Camera;
class Entity;
class Graphics;

class Manager {
public:
    Manager();
    virtual ~Manager();

    virtual void initialize() {}

    virtual void draw(Graphics* g, Camera* c);

    virtual void update(GLfloat seconds);

    void addEntity(Entity* e);

    void removeEntity(Entity* e);

protected:
    QList<Entity*> entities_;
    QList<Entity*> toAdd_;
    QList<Entity*> toRemove_;

    virtual void continuousDetection(GLfloat);

    void defaultDetection();

    virtual void addAll();

    virtual void clean();
};

#endif // MANAGER

