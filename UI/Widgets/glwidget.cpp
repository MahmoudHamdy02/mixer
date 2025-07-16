#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <algorithm>

#include "renderer.h"
#include "toolmanager.h"

GLWidget::GLWidget(Renderer* renderer, SelectionManager* selectionManager, QWidget* parent)
    : QOpenGLWidget(parent), renderer(renderer), selectionManager(selectionManager)
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

    if (ToolManager::selectedTool == ToolManager::Tool::Camera) {
        if (isCtrlHeld) {
            renderer->panCamera(offsetX, offsetY);
        } else {
            renderer->moveCamera(offsetX, offsetY);
        }
        mousePosX = newMousePosX;
        mousePosY = newMousePosY;
    } else if (renderer->getEditMode() == Renderer::EditMode::Vertex &&
               ToolManager::selectedTool == ToolManager::Tool::Select) {
        float minX = std::min(mousePosX, newMousePosX);
        float minY = height() - std::min(mousePosY, newMousePosY);
        float maxX = std::max(mousePosX, newMousePosX);
        float maxY = height() - std::max(mousePosY, newMousePosY);

        pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, minY));
        pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, maxY));
        renderer->setSelectionRectangleVertices(min, max);
    }
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO: Create InputHandler class
    if (event->button() == Qt::LeftButton) {
        isMouseButtonDown = false;
    }
    if (renderer->getEditMode() == Renderer::EditMode::Vertex &&
        ToolManager::selectedTool == ToolManager::Tool::Select) {
        float newMousePosX = event->position().x();
        float newMousePosY = event->position().y();

        float minX = std::min(mousePosX, newMousePosX);
        float maxY = height() - std::min(mousePosY, newMousePosY);
        float maxX = std::max(mousePosX, newMousePosX);
        float minY = height() - std::max(mousePosY, newMousePosY);

        pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, minY));
        pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, maxY));
        renderer->setSelectionRectangleVertices(pmp::vec2(0, 0), pmp::vec2(0, 0));
        selectionManager->selectRectangle(min, max);
        update();
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

pmp::vec2 GLWidget::screenSpaceToNDC(pmp::vec2 point)
{
    pmp::vec2 ndc;
    // Map 0 -> width to -1 -> 1
    ndc[0] = 2.0f * point[0] / width() - 1.0f;
    ndc[1] = 2.0f * point[1] / height() - 1.0f;
    return ndc;
}
