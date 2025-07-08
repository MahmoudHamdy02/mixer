#pragma once

#include <qopenglwidget.h>
#include <qwidget.h>

#include "renderer.h"

class GLWidget : public QOpenGLWidget, QOpenGLExtraFunctions
{
    Renderer* renderer;
    GLuint VAO, VBO;
    GLuint shaderProgram;

    GLuint compileShader(const char* source, GLenum type);
    GLuint createShaderProgram();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    GLWidget(Renderer* renderer, QWidget* parent = nullptr);
};
