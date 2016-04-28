#ifndef GRAPHICS
#define GRAPHICS

#include "util/CommonIncludes.h"
#include <QMap>
#include <QString>

class Camera;
class Shape;
class View;

class Graphics
{
public:
    Graphics(View* view);
    ~Graphics();

    Camera* m_camera;

    void setShader(QString name);

    GLuint getShader();

    void loadShader(QString name, QString vertexShader, QString fragmentShader);

    void loadTexture(QString filename, QString key);

    void loadShape(QString shapeName, Shape* s);

    void setUniform(QString name, GLfloat* value);

    void draw(const QString &shape, const QString &texture, const glm::mat4 &model, const glm::vec3 &color = glm::vec3(.2f, .5f, .8f));

    void draw(Shape* shape, QString texture, glm::mat4 model);

    void write(QString s, GLfloat w, GLfloat h, int size);
private:
    GLuint shader_;
    View* view_;
    QMap<QString, GLuint> shaders_;
    QMap<QString, GLuint> textures_;
    QMap<QString, Shape*> shapes_;
};

#endif // GRAPHICS

