#ifndef CAMERA
#define CAMERA

#include "util/CommonIncludes.h"

class Graphics;

/**
 * @brief The Camera class
 */
class Camera
{
public:
    Camera(Graphics* g, GLfloat aspectRatio);
    ~Camera();

    void setAspectRatio(float aspectRatio) {aspect_ = aspectRatio; buildProjection();}

    void setHeightAngle(float h) {heightAngle_ = h; buildProjection();}

    glm::vec3 getPosition() {return eye_;}

    void setPosition(const glm::vec3 &v);

    glm::vec3 getLook() {return look_;}

    glm::vec3 getUp() {return up_;}

    float getAspectRatio() {return aspect_;}

    float getHeightAngle() {return heightAngle_;}

    glm::mat4 getView();

    glm::mat4 getProj();

    void translate(const glm::vec3 &v);

    void buildLook();

    void changeYaw(float degrees);

    void changePitch(float degrees);

    void setClip(float nearPlane, float farPlane);

    void toggleThirdPerson();

    void checkAABB(int size, int count, int* position, bool* returnValue);

private:
    Graphics* graphics_;
    GLfloat aspect_;
    GLfloat heightAngle_;
    GLfloat near_;
    GLfloat far_;

    glm::vec3 eye_;
    glm::vec3 look_;
    glm::vec3 up_;

    GLfloat yaw_;
    GLfloat pitch_;

    glm::mat4 proj_;
    glm::mat4 view_;

    bool thirdPerson_;

    void buildView();

    void buildProjection();
};

#endif // CAMERA

