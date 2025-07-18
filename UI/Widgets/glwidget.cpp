#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <algorithm>
#include <iostream>

#include "renderer.h"
#include "toolmanager.h"

GLWidget::GLWidget(Renderer* renderer, SelectionManager* selectionManager, QWidget* parent)
    : QOpenGLWidget(parent), renderer(renderer), selectionManager(selectionManager)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    QSurfaceFormat format;
    format.setDepthBufferSize(32);   // Request a 24-bit depth buffer
    format.setStencilBufferSize(8);  // Stencil support
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(0);
    setFormat(format);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
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
    } else if (ToolManager::selectedTool == ToolManager::Tool::Select) {
        isDrawingSelectionRectangle = true;
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
    makeCurrent();
    // TODO: Create InputHandler class
    if (ToolManager::selectedTool == ToolManager::Tool::Select) {
        float newMousePosX = event->position().x();
        float newMousePosY = event->position().y();

        float minX = std::min(mousePosX, newMousePosX);
        float maxY = height() - std::min(mousePosY, newMousePosY);
        float maxX = std::max(mousePosX, newMousePosX);
        float minY = height() - std::max(mousePosY, newMousePosY);

        pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, minY));
        pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, maxY));
        if (ToolManager::selectedEditMode == ToolManager::EditMode::Vertex) {
            if (isDrawingSelectionRectangle) {
                selectionManager->selectVerticesInRectangle(min, max);
            } else {
                pmp::vec2 ndcClickPos = screenSpaceToNDC(pmp::vec2(newMousePosX, height() - newMousePosY));
                GLfloat depth;
                glReadPixels((int)newMousePosX, (int)height() - newMousePosY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,
                             &depth);
                std::cout << "Selecting vertex, depth: " << depth << std::endl;
                selectionManager->selectVertex(ndcClickPos[0], ndcClickPos[1], depth);
            }
        } else {
            if (isDrawingSelectionRectangle) {
                selectionManager->selectObjectsInRectangle(min, max);
            } else {
                Ray ray = renderer->mouseToWorldRay(newMousePosX, newMousePosY);
                selectionManager->selectObject(ray);
            }
        }
        renderer->setSelectionRectangleVertices(pmp::vec2(0, 0), pmp::vec2(0, 0));
        update();
    }
    isDrawingSelectionRectangle = false;
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
