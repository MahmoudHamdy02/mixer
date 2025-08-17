#include "selecttool.h"

#include <qpoint.h>

#include "Tools/tool.h"
#include "Widgets/glwidget.h"
#include "editmodes.h"
#include "toolmanager.h"
#include "toolmodes.h"

SelectTool::SelectTool(GLWidget* glWidget, Renderer* renderer, SelectionManager* selectionManager)
    : Tool(ToolMode::Select), glWidget(glWidget), renderer(renderer), selectionManager(selectionManager)
{
}

void SelectTool::onMousePress(QPointF pos)
{
    mousePos = pos;
}

void SelectTool::onMouseMove(QPointF pos)
{
    QPointF newMousePos = pos;

    isDrawingSelectionRectangle = true;
    renderer->setDrawSelectionRectangle(true);
    float minX = std::min(mousePos.x(), newMousePos.x());
    float minY = glWidget->height() - std::min(mousePos.y(), newMousePos.y());
    float maxX = std::max(mousePos.x(), newMousePos.x());
    float maxY = glWidget->height() - std::max(mousePos.y(), newMousePos.y());

    pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, minY));
    pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, maxY));
    renderer->setSelectionRectangleVertices(min, max);
}

void SelectTool::onMouseRelease(QPointF pos)
{
    QPointF newMousePos = pos;

    float minX = std::min(mousePos.x(), newMousePos.x());
    float minY = glWidget->height() - std::min(mousePos.y(), newMousePos.y());
    float maxX = std::max(mousePos.x(), newMousePos.x());
    float maxY = glWidget->height() - std::max(mousePos.y(), newMousePos.y());

    pmp::vec2 min = screenSpaceToNDC(pmp::vec2(minX, maxY));
    pmp::vec2 max = screenSpaceToNDC(pmp::vec2(maxX, minY));
    if (ToolManager::selectedEditMode == EditMode::Vertex) {
        if (isDrawingSelectionRectangle) {
            selectionManager->selectVerticesInRectangle(min, max, renderer->getMVPMatrix(),
                                                        renderer->getCamera().front);
            // TODO: Only update buffers of changed meshes
            renderer->updateMeshes();
        } else {
            pmp::vec2 ndcClickPos = screenSpaceToNDC(pmp::vec2(newMousePos.x(), glWidget->height() - newMousePos.y()));
            float depth = renderer->getPixelDepth((int)newMousePos.x(), (int)glWidget->height() - newMousePos.y());
            selectionManager->selectVertex(ndcClickPos[0], ndcClickPos[1], depth, renderer->getMVPMatrix(),
                                           renderer->getCamera().front);
            renderer->updateMeshes();
        }
    } else {
        if (isDrawingSelectionRectangle) {
            selectionManager->selectObjectsInRectangle(min, max, renderer->getViewMatrix(),
                                                       renderer->getProjectionMatrix());
        } else {
            Ray ray = renderer->mouseToWorldRay(newMousePos.x(), newMousePos.y());
            selectionManager->selectObject(ray);
        }
    }
    renderer->setSelectionRectangleVertices(pmp::vec2(0, 0), pmp::vec2(0, 0));
    isDrawingSelectionRectangle = false;
    renderer->setDrawSelectionRectangle(false);
}

pmp::vec2 SelectTool::screenSpaceToNDC(pmp::vec2 point)
{
    pmp::vec2 ndc;
    // Map 0 -> width to -1 -> 1
    ndc[0] = 2.0f * point[0] / glWidget->width() - 1.0f;
    ndc[1] = 2.0f * point[1] / glWidget->height() - 1.0f;
    return ndc;
}
