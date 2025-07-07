#include "glwidget.h"

#include <GL/gl.h>
#include <qopenglwidget.h>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void GLWidget::initializeGL()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void GLWidget::resizeGL(int w, int h) {}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
