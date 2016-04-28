#include "engine/common/camera.h"
#include "engine/common/graphics.h"
#include <qgl.h>

Camera::Camera(Graphics* g, GLfloat aspectRatio) :
    graphics_(g),
    aspect_(aspectRatio),
    heightAngle_(glm::radians(60.f)),
    near_(.1f),
    far_(20000.f),
    eye_(glm::vec3(0,.5,0)),
    look_(glm::vec3(1,0,0)),
    up_(glm::vec3(0,1,0)),
    yaw_(0.f),
    pitch_(0.f),
    thirdPerson_(false)
{
    buildView();
    buildProjection();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getView() {
    return glm::lookAt(eye_, eye_+look_, up_);
}

glm::mat4 Camera::getProj() {
    return glm::perspective(heightAngle_,aspect_,near_, far_);
}


void Camera::buildProjection()
{
    GLuint shader = graphics_->getShader();
    glUseProgram(shader);
    proj_ = glm::perspective(heightAngle_,aspect_,near_, far_);
    glUniformMatrix4fv(glGetUniformLocation(shader,"p"),1,GL_FALSE, glm::value_ptr(proj_));

    glUseProgram(0);
}

void Camera::buildView()
{
    GLuint shader = graphics_->getShader();
    glUseProgram(shader);
    if (thirdPerson_) {
        view_ = glm::lookAt(eye_-look_*2.f, eye_+look_, up_);
        glUniformMatrix4fv(glGetUniformLocation(shader,"v"),1,GL_FALSE,glm::value_ptr(view_));
    } else {
        view_ = glm::lookAt(eye_, eye_+look_, up_);
        glUniformMatrix4fv(glGetUniformLocation(shader,"v"),1,GL_FALSE,glm::value_ptr(view_));
    }
    glUseProgram(0);
}

void Camera::translate(const glm::vec3 &v)
{
    eye_ = eye_ + v;
    buildView();
}

void Camera::setPosition(const glm::vec3 &v)
{
    eye_ = v;
    buildView();
}

void Camera::changePitch(GLfloat degrees) {
    if (!thirdPerson_) {
        GLfloat pitch =  pitch_-degrees;
        if (pitch > 80.0f)
            pitch = 80.0;
        if (pitch < -89.0f)
            pitch = -89.0f;
        pitch_ = pitch;
        buildLook();
    }
}

void Camera::changeYaw(GLfloat degrees) {
    yaw_ += degrees;
    buildLook();
}

void Camera::buildLook()
{
    glm::vec3 look;
    look.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    look.y = sin(glm::radians(pitch_));
    look.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    look_ = glm::normalize(look);
    buildView();
}

void Camera::setClip(float nearPlane, float farPlane)
{
    assert(farPlane > nearPlane);
    near_ = nearPlane;
    far_ = farPlane;
    buildProjection();
}

void Camera::toggleThirdPerson() {
    thirdPerson_ = !thirdPerson_;
    pitch_ = 0.f;
    buildLook();
}

void Camera::checkAABB(int size, int count, int* position, bool* returnValue) {
    glm::vec4 points[] = {
      glm::vec4(0,0,0,0),
      glm::vec4(0,0,1,0),
      glm::vec4(0,1,0,0),
      glm::vec4(0,1,1,0),
      glm::vec4(1,0,0,0),
      glm::vec4(1,0,1,0),
      glm::vec4(1,1,0,0),
      glm::vec4(1,1,1,0)};

    glm::mat4 frust = glm::transpose(proj_*view_);

    glm::vec4 planes[] {
        frust[3]-frust[0],
        frust[3]-frust[1],
        frust[3]-frust[2],
        frust[3]+frust[0],
        frust[3]+frust[1],
        frust[3]+frust[2]};

    for (int i = 0; i < count; i++) {
        glm::vec4 pos = glm::vec4(glm::vec3(position[3*i],position[3*i+1],position[3*i+2]),1);
        int in = 0;
        int out = 0;

        returnValue[i] = true;

        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 8; k++) {
                if (glm::dot(pos+1.f*size*points[k],planes[j]) < 0) {
                    out++;
                } else {
                    in++;
                    break;
                }
            }
            if (out == 8) {
                returnValue[i] = false;
                break;
            }
            out = 0;
        }
    }
}

