#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <memory>
#include <vector>

#include "Commands/deletemeshcommand.h"
#include "Commands/deletemeshescommand.h"
#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "toolmanager.h"
#include "toolmodes.h"

GLWidget::GLWidget(SceneController* scene, Renderer* renderer, ToolManager* toolManager,
                   SelectionManager* selectionManager, HistoryManager* historyManager, QWidget* parent)
    : QOpenGLWidget(parent),
      scene(scene),
      renderer(renderer),
      toolManager(toolManager),
      selectionManager(selectionManager),
      historyManager(historyManager)
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
    toolManager->onMousePress(event->position());
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    float newMousePosX = event->position().x();
    float newMousePosY = event->position().y();

    float offsetX = newMousePosX - mousePosX;
    float offsetY = newMousePosY - mousePosY;

    if (toolManager->getActiveTool() == ToolMode::Camera) {
        if (isCtrlHeld) {
            renderer->panCamera(offsetX, offsetY);
        } else {
            renderer->moveCamera(offsetX, offsetY);
        }
        mousePosX = newMousePosX;
        mousePosY = newMousePosY;
    } else if (toolManager->getActiveTool() == ToolMode::Select) {
        toolManager->onMouseMove(event->position());
    }
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    makeCurrent();
    // TODO: Create InputHandler class
    if (toolManager->getActiveTool() == ToolMode::Select) {
        toolManager->onMouseRelease(event->position());
    }
    update();
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

    switch (event->key()) {
        case Qt::Key_Control: {
            isCtrlHeld = false;
            break;
        }
        case Qt::Key_Delete: {
            if (ToolManager::selectedEditMode == EditMode::Object) {
                const std::unordered_set<std::weak_ptr<Mesh>>& meshes = selectionManager->getSelectedMeshes();
                // TODO: Vertex delete command
                if (meshes.size() == 1) {
                    if (auto mesh = meshes.begin()->lock()) {
                        historyManager->executeCommand(
                            std::make_unique<DeleteMeshCommand>(this, scene, renderer, mesh));
                    }
                } else if (meshes.size() > 1) {
                    std::vector<std::shared_ptr<Mesh>> meshesToBeDeleted;
                    for (const std::weak_ptr<Mesh>& w : meshes) {
                        if (auto mesh = w.lock()) {
                            meshesToBeDeleted.push_back(mesh);
                        }
                    }
                    historyManager->executeCommand(
                        std::make_unique<DeleteMeshesCommand>(this, scene, renderer, meshesToBeDeleted));
                }
                selectionManager->resetSelectedObjects();

                update();
            } else {
                const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();
                for (std::shared_ptr<Mesh> mesh : meshes) {
                    mesh->deleteSelectedVertices();
                }
                renderer->updateMeshes();
                update();
            }
        }
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
