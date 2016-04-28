#include "engine/common/camera.h"
#include "engine/common/rendergeometry/shape.h"
#include "engine/common/graphics.h"
#include "util/ResourceLoader.h"
#include "view.h"
#include <string.h>
#include <iostream>

Graphics::Graphics(View* view) : view_(view){
}

Graphics::~Graphics() {
    QMap<QString, GLuint>::iterator i;
    for (i = textures_.begin(); i != textures_.end(); ++i)
        glDeleteTextures(1,&i.value());

    QMap<QString, GLuint>::iterator j;
    for (j = shaders_.begin(); j != shaders_.end(); ++j)
        glDeleteShader(j.value());

    QMap<QString, Shape*>::iterator k;
    for (k = shapes_.begin(); k != shapes_.end(); ++k)
        delete k.value();
}

void Graphics::setShader(QString name) {
    shader_ = shaders_.value(name);
}

GLuint Graphics::getShader() {
    return shader_;
}

void Graphics::loadShader(QString name, QString vertexShader, QString fragmentShader) {
    if (!shaders_.contains(name)) {
        QByteArray vert = vertexShader.toLatin1();
        QByteArray frag = fragmentShader.toLatin1();
        GLuint newShader = ResourceLoader::loadShaders(vert.data(),frag.data());
        shaders_.insert(name, newShader);
    }
}

void Graphics::loadTexture(QString filename, QString key)
{
    if (!textures_.contains(key)) {
        QImage img(":/images/"+filename);
        if (img.isNull()) {
            std::cout << "Failed to load texture: " << filename.toLocal8Bit().constData() << std::endl;
            return;
        }
        img = QGLWidget::convertToGLFormat(img);

        GLuint texture;
        glGenTextures(1,&texture);
        textures_.insert(key, texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Graphics::loadShape(QString shapeName, Shape* s){
    if (!shapes_.contains(shapeName)) {
        shapes_.insert(shapeName, s);
    }
}

void Graphics::write(QString s, GLfloat w, GLfloat h, int size) {
    view_->write(s,w,h,size);
}

void Graphics::draw(const QString &shape, const QString &texture, const glm::mat4 &model, const glm::vec3 &color)
{
    if (!texture.isEmpty() && !textures_.contains(texture)) {
        std::cout << ("ERROR: texture not loaded: " + texture).toStdString() << std::endl;
        return;
    }
    if (!shapes_.contains(shape)) {
        std::cout << ("ERROR: shape not loaded: " + shape).toStdString() << std::endl;
        return;
    }
    if (!texture.isEmpty()){
        glUseProgram(shader_);
        glUniformMatrix4fv(glGetUniformLocation(shader_,"m"),1,GL_FALSE,glm::value_ptr(model));

        GLuint texId = textures_.value(texture);

        glUniform1i(glGetUniformLocation(shader_,"useTexture"),1);

        glUniform1i(glGetUniformLocation(shader_, "tex"), GL_TEXTURE0);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texId);

        (shapes_.value(shape))->draw();

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }else {
        glUseProgram(shader_);
        glUniformMatrix4fv(glGetUniformLocation(shader_,"m"),1,GL_FALSE,glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shader_,"useTexture"),0);
        glUniform3fv(glGetUniformLocation(shader_,"color"),1,glm::value_ptr(color));

        (shapes_.value(shape))->draw();
        glUseProgram(0);
    }
}

void Graphics::draw(Shape* shape, QString texture, glm::mat4 model)
{
    if (!texture.isEmpty() && !textures_.contains(texture)) {
        std::cout << ("ERROR: texture not loaded 2: " + texture).toStdString() << std::endl;
        return;
    }
    if (!texture.isEmpty()){
        glUseProgram(shader_);
        glUniformMatrix4fv(glGetUniformLocation(shader_,"m"),1,GL_FALSE,glm::value_ptr(model));

        GLuint texId = textures_.value(texture);

        glUniform1i(glGetUniformLocation(shader_,"useTexture"),1);

        glUniform1i(glGetUniformLocation(shader_, "tex"), GL_TEXTURE0);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texId);

        shape->draw();

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }
}
