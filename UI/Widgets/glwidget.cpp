#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <iostream>

#include "renderer.h"

GLWidget::GLWidget(Renderer* renderer, QWidget* parent) : QOpenGLWidget(parent), renderer(renderer)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

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

    if (isCtrlHeld) {
        renderer->panCamera(offsetX, offsetY);
    } else {
        renderer->moveCamera(offsetX, offsetY);
    }
    mousePosX = newMousePosX;
    mousePosY = newMousePosY;
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO: Create InputHandler class
    if (event->button() == Qt::LeftButton) {
        isMouseButtonDown = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        renderer->zoomCamera(numPixels.y());
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        renderer->zoomCamera(numSteps.y());
    }

    event->accept();
    update();
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        isCtrlHeld = true;
    }
    QOpenGLWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        isCtrlHeld = false;
    }
    QOpenGLWidget::keyReleaseEvent(event);
}
