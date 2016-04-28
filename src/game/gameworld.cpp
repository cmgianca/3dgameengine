#include "engine/common/application.h"
#include "engine/common/camera.h"
#include "engine/geometric/geometricmanager.h"
#include "engine/common/collisiongeometry/boundingbox.h"
#include "engine/common/rendergeometry/cylinder.h"
#include "engine/common/rendergeometry/quad.h"
#include "engine/common/rendergeometry/ellipse.h"
#include "engine/common/rendergeometry/skybox.h"
#include "engine/common/rendergeometry/polyhedron.h"
#include "engine/common/collisiongeometry/polyhedrashape.h"
#include "engine/geometric/rendermesh.h"
#include "game/gameworld.h"
#include "game/gameobject.h"
#include "game/player.h"
#include "game/worldblocks.h"
#include <glm/gtx/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <QKeyEvent>


GameWorld::GameWorld(Graphics* g)
    : World(g),
      m_geometricManager(NULL),
      m_player(NULL),
      polycheck_(NULL),
      mouseDistance(glm::vec2()),
      goalVelocity(3.f),
      m_skybox(NULL)
{
    actioncount_ = 8;
    actionfunction_ = new action[actioncount_];
    for (int i = 0; i < actioncount_ ; i++) {
       actionfunction_[i] = NULL;
    }
}

GameWorld::~GameWorld()
{
    delete[] actionfunction_;
}

void GameWorld::bindKeys() {
    bindKey(MoveForward, Qt::Key_W);
    bindKey(MoveBackward, Qt::Key_S);
    bindKey(MoveLeft, Qt::Key_A);
    bindKey(MoveRight, Qt::Key_D);
    bindKey(Hold, Qt::LeftButton);
    bindKey(Sprint, Qt::Key_Shift);
    bindKey(Jump, Qt::Key_Space);
    bindKey(Exit, Qt::Key_Backspace);
}

void GameWorld::bindActions() {
    actionfunction_[MoveForward] = &moveForward;
    actionfunction_[MoveBackward] = &moveBackward;
    actionfunction_[MoveLeft] = &moveLeft;
    actionfunction_[MoveRight] = &moveRight;
    actionfunction_[Sprint] = &sprint;
    actionfunction_[Jump] = &jump;
    actionfunction_[Exit] = &exit;
}

void GameWorld::loadShapes()
{
    graphics_->loadShape("quad", new Quad());
    graphics_->loadShape("cylinder", new Cylinder());
    graphics_->loadShape("ellipse", new typename Ellipse::Ellipse()); //No clue why I had to do this for ellipse and not the others
    m_skybox = new Skybox();
    graphics_->loadShape("skybox", m_skybox);
    glm::vec3 verticesArray[] = {
        glm::vec3( 1.f, 1.f, 1.f),
        glm::vec3( 1.f, 1.f,-1.f),
        glm::vec3(-1.f, 1.f,-1.f),
        glm::vec3(-1.f, 1.f, 1.f),
        glm::vec3( 1.f,-1.f, 1.f),
        glm::vec3( 1.f,-1.f,-1.f),
        glm::vec3(-1.f,-1.f,-1.f),
        glm::vec3(-1.f,-1.f, 1.f)
    };
    glm::vec3 normalsArray[] = {
        glm::vec3(0, 1,0),
        glm::vec3(0,-1,0),
        glm::vec3( 1,0,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,0, 1),
        glm::vec3(0,0,-1)
    };
    glm::vec2 texcoordsArray[] = {
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1)
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        7, 5, 4,
        7, 6, 5,
        4, 1, 0,
        4, 5, 1,
        3, 2, 7,
        2, 6, 7,
        3, 4, 0,
        3, 7, 4,
        1, 5, 2,
        5, 6, 2
    };
    QList<glm::vec3> vertices;
    QList<glm::vec3> normals;
    QList<glm::vec2> texturecoords;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            vertices.append(verticesArray[indices[i*6+j]]);
            normals.append(normalsArray[i]);
            texturecoords.append(texcoordsArray[i]);
        }
    }

    graphics_->loadShape("cube", new Polyhedron(vertices,normals,texturecoords));

    glm::vec3 tetraVerticesArray[] = {
        glm::vec3( 0.f, 1.f, 0.f),
        glm::vec3( 1.f,-1.f, 1.f),
        glm::vec3( 1.f,-1.f,-1.f),
        glm::vec3(-1.f,-1.f, 1.f)
    };
    glm::vec3 tetraNormalsArray[] = {
        glm::vec3(0,-1.f,0),
        glm::cross(tetraVerticesArray[0]-tetraVerticesArray[3], tetraVerticesArray[0]-tetraVerticesArray[1]),
        glm::cross(tetraVerticesArray[0]-tetraVerticesArray[2], tetraVerticesArray[0]-tetraVerticesArray[3]),
        glm::cross(tetraVerticesArray[0]-tetraVerticesArray[1], tetraVerticesArray[0]-tetraVerticesArray[2])
    };
    glm::vec2 tetraTexcoordsArray[] = {
        glm::vec2(1,1),
        glm::vec2(1,1),
        glm::vec2(1,1),
        glm::vec2(1,1)
    };
    GLuint tetraIndices[] = {
        1, 3, 2,
        0, 3, 1,
        0, 2, 3,
        0, 1, 2
    };
    vertices = QList<glm::vec3>();
    normals = QList<glm::vec3>();
    texturecoords = QList<glm::vec2>();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            vertices.append(tetraVerticesArray[tetraIndices[i*3+j]]);
            normals.append(tetraNormalsArray[i]);
            texturecoords.append(tetraTexcoordsArray[i]);
        }
    }
    graphics_->loadShape("tetra", new Polyhedron(vertices,normals,texturecoords));
}

void GameWorld::loadTextures()
{
    graphics_->loadTexture("terrain.png", "terrain");
    graphics_->loadTexture("skybox.png", "skybox");
}

void GameWorld::loadObjects()
{
    m_geometricManager = new GeometricManager();
    manager_ = m_geometricManager;
}

void GameWorld::draw() {
    graphics_->setShader("default");
    graphics_->draw("skybox","skybox",glm::mat4());
    World::draw();
}

void GameWorld::update(GLfloat seconds)
{
    goal_ = glm::vec3(0.f,0.f,0.f);
    if (!actionactive_[Sprint])
        goalVelocity = 3.f;
    if (actionactive_[Hold])
        polycheck_->setPosition(m_player->getPosition()+camera_->getLook()*5.f);
    if (m_player == NULL) {
        m_player = new Player(this);
        manager_->addEntity(m_player);
        QList<glm::vec3> verts;
        glm::vec3 verticesArray[] = {
            // Positions
            glm::vec3( 1.f, 1.f, 1.f),
            glm::vec3( 1.f, 1.f,-1.f),
            glm::vec3(-1.f, 1.f,-1.f),
            glm::vec3(-1.f, 1.f, 1.f),
            glm::vec3( 1.f,-1.f, 1.f),
            glm::vec3( 1.f,-1.f,-1.f),
            glm::vec3(-1.f,-1.f,-1.f),
            glm::vec3(-1.f,-1.f, 1.f)
        };
        for (int i = 0; i < 8; i++) {
            verts.append(verticesArray[i]);
        }
        polycheck_ = new GameObject("cube",
                                      "",
                                      new PolyhedraShape(verts),
                                      glm::vec3(1.f,1.f,1.f),
                                      m_player->getPosition()+camera_->getLook()*5.f,
                                      glm::vec3(0,1.f,0));
        manager_->addEntity(polycheck_);
        GameObject* g = new GameObject("cube",
                                       "",
                                       new PolyhedraShape(verts),
                                       glm::vec3(1.f,1.f,1.f),
                                       glm::vec3(0,0,10.f),
                                       glm::vec3(0,1.f,0));
        g->rotate(glm::vec3(0, 3.14159f/4.f, 0));
        manager_->addEntity(g);
        verts = QList<glm::vec3>();
        glm::vec3 tetraVerticesArray[] = {
            glm::vec3( 0.f, 1.f, 0.f),
            glm::vec3( 1.f,-1.f, 1.f),
            glm::vec3( 1.f,-1.f,-1.f),
            glm::vec3(-1.f,-1.f, 1.f)
        };
        for (int i = 0; i < 4; i++) {
            verts.append(tetraVerticesArray[i]);
        }
        g = new GameObject("tetra",
                           "",
                           new PolyhedraShape(verts),
                           glm::vec3(1.f,1.f,1.f),
                           glm::vec3(5.f,0,10.f),
                           glm::vec3(0,1.f,0));
        manager_->addEntity(g);
    }
    polycheck_->setColor(glm::vec3(0.f,0.f,1.f));
    camera_->changeYaw(mouseDistance.x*.1f);
    camera_->changePitch(mouseDistance.y*.1f);
    if (m_player->getPosition().y <= 0) {
        m_player->setPosition(m_player->getPosition()*glm::vec3(1,0,1));
        m_player->setVelocity(m_player->getVelocity()*glm::vec3(1,0,1));
        m_player->m_canJump = true;
    }

    World::update(seconds);
    goal_ = 10.f*(glm::vec3(goal_.x,0, goal_.z)-glm::vec3(m_player->getVelocity().x, 0, m_player->getVelocity().z));
    m_player->setAcceleration(goal_ + glm::vec3(0,-9.8,0));
    mouseDistance = glm::vec2();
    camera_->setPosition(m_player->getPosition() + glm::vec3(0,.5f,0));
}

void GameWorld::mouseMoveEvent(int deltaX, int deltaY)
{
    mouseDistance += glm::vec2(deltaX,deltaY);
}

void GameWorld::doActions() {
    for (int i = 0; i < actioncount_; i++) {
        if (actionactive_[i]) {
            if (actionfunction_[i]) {
                (this->*actionfunction_[i])();
            }
        }
    }
}

glm::vec3 GameWorld::getPlayerPosition() {
    return m_player->getPosition();
}

void GameWorld::moveForward() {
    glm::vec3 dir = camera_->getLook();
    dir.y = 0;
    dir = glm::normalize(dir);
    goal_ = goal_ + dir*goalVelocity;
}

void GameWorld::moveBackward() {
    glm::vec3 dir = camera_->getLook();
    dir.y = 0;
    dir = glm::normalize(dir);
    goal_ = goal_ - dir*goalVelocity;
}

void GameWorld::moveLeft() {
    glm::vec3 dir = camera_->getLook();
    dir.y = 0;
    dir = glm::normalize(dir);
    goal_ = goal_ + glm::rotateY(dir,glm::radians(90.f))*goalVelocity;
}

void GameWorld::moveRight() {
    glm::vec3 dir = camera_->getLook();
    dir.y = 0;
    dir = glm::normalize(dir);
    goal_ = goal_ - glm::rotateY(dir,glm::radians(90.f))*goalVelocity;
}

void GameWorld::sprint() {
    goalVelocity = 5.f;
}

void GameWorld::jump() {
    m_player->jump();
}

void GameWorld::hold() {
}

void GameWorld::exit() {
    Application::screenStack.pop_front();
}
