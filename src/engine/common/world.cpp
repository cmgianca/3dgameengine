#include "engine/common/camera.h"
#include "engine/common/entity.h"
#include "engine/common/manager.h"
#include "engine/common/world.h"
#include "engine/common/collisiongeometry/collisionshape.h"
#include "engine/common/graphics.h"
#include <QKeyEvent>

World::World(Graphics* g) :
    actionactive_(NULL),
    actioncount_(0),
    graphics_(g),
    manager_(NULL),
    width_(0),
    height_(0)
{
}

World::~World(){
    if (camera_ != NULL) {
        delete camera_;
    }
    delete[] actionactive_;
    if (manager_ != NULL) {
        delete manager_;
    }
}

void World::initialize(int w, int h){
    loadTextures();
    loadShapes();
    loadObjects();
    width_ = w;
    height_ = h;
    camera_ = new Camera(graphics_, 1.f*w/h);
    actionactive_ = new int[actioncount_];
    for (int i = 0; i < actioncount_; i++) {
        actionactive_[i] = false;
    }
    bindKeys();
    bindActions();
}

void World::resizeGL(int w, int h)
{
    width_ = w;
    height_ = h;
    if (camera_ != NULL) {
        camera_->setAspectRatio(1.f*w/h);
    }
}


void World::draw() {
    graphics_->setShader("ui");
    drawUI();
    graphics_->setShader("default");
    if (manager_ != NULL) {
        manager_->draw(graphics_,camera_);
    }
}

void World::update(GLfloat seconds) {
    doActions();
    if (manager_ != NULL) {
        manager_->update(seconds);
    }
}

void World::keyPressEvent(QKeyEvent *event) {
    int check = event->key();
    QList<int> results = bindings_.values(check);
    for (int i = 0; i < results.size(); i++) {
        if (results.at(i) > actioncount_) {
            std::cout << "ERROR: Invalid Action" << std::endl;
        }  else {
            actionactive_[results.at(i)] = true;
        }
    }
}

void World::keyReleaseEvent(QKeyEvent *event) {
    int check = event->key();
    QList<int> results = bindings_.values(check);
    for (int i = 0; i < results.size(); i++) {
        if (results.at(i) > actioncount_) {
            std::cout << "ERROR: Invalid Action" << std::endl;
        }  else {
            actionactive_[results.at(i)] = false;
        }
    }
}

void World::mousePressEvent(QMouseEvent *event) {
    int check = event->button();
    QList<int> results = bindings_.values(check);
    for (int i = 0; i < results.size(); i++) {
        if (results.at(i) > actioncount_) {
            std::cout << "ERROR: Invalid Action" << std::endl;
        }  else {
            actionactive_[results.at(i)] = true;
        }
    }
}

void World::mouseReleaseEvent(QMouseEvent *event) {
    int check = event->button();
    QList<int> results = bindings_.values(check);
    for (int i = 0; i < results.size(); i++) {
        if (results.at(i) > actioncount_) {
            std::cout << "ERROR: Invalid Action" << std::endl;
        }  else {
            actionactive_[results.at(i)] = false;
        }
    }
}

void World::bindKey(int key, int typevalue) {
    bindings_.insert(typevalue,key);
}

void World::unbindKey(int typevalue){
    bindings_.remove(typevalue);
}
