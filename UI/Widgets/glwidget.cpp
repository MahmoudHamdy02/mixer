#include "glwidget.h"

#include <qopenglwidget.h>

#include <QMouseEvent>

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

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isMouseButtonDown = true;
    }
    mousePosX = event->position().x();
    mousePosY = event->position().y();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    float newMousePosX = event->position().x();
    float newMousePosY = event->position().y();

    float offsetX = newMousePosX - mousePosX;
    float offsetY = newMousePosY - mousePosY;

    renderer->moveCamera(offsetX, offsetY);
    mousePosX = newMousePosX;
    mousePosY = newMousePosY;
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isMouseButtonDown = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent* event) {}
