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
    toolManager->onMousePress(event->position());
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // TODO: Invert y axis
    toolManager->onMouseMove(event->position());
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO: Create InputHandler class
    toolManager->onMouseRelease(event->position());
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
    toolManager->onKeyDown((Qt::Key)event->key());
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    toolManager->onKeyUp((Qt::Key)event->key());

    switch (event->key()) {
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
