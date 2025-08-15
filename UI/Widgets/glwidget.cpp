#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <algorithm>
#include <memory>
#include <optional>
#include <vector>

#include "History/deletemeshcommand.h"
#include "History/deletemeshescommand.h"
#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "toolmanager.h"
#include "toolmodes.h"

GLWidget::GLWidget(SceneController* scene, Renderer* renderer, SelectionManager* selectionManager,
                   HistoryManager* historyManager, QWidget* parent)
    : QOpenGLWidget(parent),
      scene(scene),
      renderer(renderer),
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
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    float newMousePosX = event->position().x();
    float newMousePosY = event->position().y();

    float offsetX = newMousePosX - mousePosX;
    float offsetY = newMousePosY - mousePosY;

    if (ToolManager::selectedTool == ToolMode::Camera) {
        if (isCtrlHeld) {
            renderer->panCamera(offsetX, offsetY);
        } else {
            renderer->moveCamera(offsetX, offsetY);
        }
        mousePosX = newMousePosX;
        mousePosY = newMousePosY;
    } else if (ToolManager::selectedTool == ToolMode::Select) {
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
    if (ToolManager::selectedTool == ToolMode::Select) {
        float newMousePosX = event->position().x();
        float newMousePosY = event->position().y();

        float minX = std::min(mousePosX, newMousePosX);
        float maxY = height() - std::min(mousePosY, newMousePosY);
        float maxX = std::max(mousePosX, newMousePosX);
        float minY = height() - std::max(mousePosY, newMousePosY);

        pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, minY));
        pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, maxY));
        if (ToolManager::selectedEditMode == EditMode::Vertex) {
            if (isDrawingSelectionRectangle) {
                selectionManager->selectVerticesInRectangle(min, max, renderer->getMVPMatrix(),
                                                            renderer->getCamera().front);
                // TODO: Only update buffers of changed meshes
                renderer->updateMeshes();
            } else {
                pmp::vec2 ndcClickPos = screenSpaceToNDC(pmp::vec2(newMousePosX, height() - newMousePosY));
                GLfloat depth;
                glReadPixels((int)newMousePosX, (int)height() - newMousePosY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,
                             &depth);
                selectionManager->selectVertex(ndcClickPos[0], ndcClickPos[1], depth, renderer->getMVPMatrix(),
                                               renderer->getCamera().front);
                renderer->updateMeshes();
            }
        } else {
            if (isDrawingSelectionRectangle) {
                selectionManager->selectObjectsInRectangle(min, max, renderer->getViewMatrix(),
                                                           renderer->getProjectionMatrix());
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
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z) {
        historyManager->undo();
        event->accept();
        return;
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Y) {
        historyManager->redo();
        event->accept();
        return;
    }

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
                // TODO: Maybe move this inside the command?
                // TODO: Vertex delete command
                // TODO: Undo/redo UI
                if (meshes.size() == 1) {
                    if (auto mesh = meshes.begin()->lock()) {
                        std::optional<const std::shared_ptr<MeshGL>> meshGL = renderer->getMeshGLFromMesh(mesh);
                        if (meshGL.has_value())
                            historyManager->executeCommand(
                                std::make_unique<DeleteMeshCommand>(this, scene, renderer, mesh, meshGL.value()));
                    }
                } else if (meshes.size() > 1) {
                    std::vector<std::shared_ptr<Mesh>> meshesToBeDeleted;
                    std::vector<std::shared_ptr<MeshGL>> meshGLsToBeDeleted;
                    for (const std::weak_ptr<Mesh>& w : meshes) {
                        if (auto mesh = w.lock()) {
                            meshesToBeDeleted.push_back(mesh);
                            std::optional<const std::shared_ptr<MeshGL>> meshGL = renderer->getMeshGLFromMesh(mesh);
                            if (meshGL.has_value())
                                meshGLsToBeDeleted.push_back(meshGL.value());
                        }
                    }
                    historyManager->executeCommand(std::make_unique<DeleteMeshesCommand>(
                        this, scene, renderer, meshesToBeDeleted, meshGLsToBeDeleted));
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
