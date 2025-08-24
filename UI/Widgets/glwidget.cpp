#include "glwidget.h"

#include <qnamespace.h>
#include <qopenglwidget.h>

#include <QMouseEvent>
#include <memory>
#include <vector>

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

float GLWidget::getPixelDepth(int screenX, int screenY)
{
    makeCurrent();
    GLfloat depth;
    glReadPixels(screenX, screenY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    doneCurrent();
    return depth;
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    toolManager->onMousePress(QPointF(event->position().x(), height() - event->position().y()));
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    toolManager->onMouseMove(QPointF(event->position().x(), height() - event->position().y()));
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO: Create InputHandler class
    toolManager->onMouseRelease(QPointF(event->position().x(), height() - event->position().y()));
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
                std::vector<std::shared_ptr<Mesh>> meshesToBeDeleted;
                for (const std::weak_ptr<Mesh>& w : meshes) {
                    if (auto mesh = w.lock()) {
                        meshesToBeDeleted.push_back(mesh);
                    }
                }
                historyManager->executeCommand(
                    std::make_unique<DeleteMeshesCommand>(this, scene, renderer, meshesToBeDeleted));
                selectionManager->resetSelectedObjects();

                update();
            } else {
                const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();
                for (std::shared_ptr<Mesh> mesh : meshes) {
                    mesh->deleteSelectedVertices();
                }
                renderer->queueUpdateMeshes();
                update();
            }
        }
    }
    QOpenGLWidget::keyReleaseEvent(event);
}
