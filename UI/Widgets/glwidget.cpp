#include "glwidget.h"

#include <qopenglwidget.h>

#include "renderer.h"

GLWidget::GLWidget(Renderer* renderer, QWidget* parent) : QOpenGLWidget(parent), renderer(renderer) {}

void GLWidget::initializeGL()
{
    renderer->initialize();
}

void GLWidget::resizeGL(int w, int h)
{
    renderer->resize(w, h);
}

void GLWidget::paintGL()
{
    renderer->render();
}
