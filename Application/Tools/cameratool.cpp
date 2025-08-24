#include "cameratool.h"

#include <qpoint.h>

#include "renderer.h"
#include "toolmodes.h"

CameraTool::CameraTool(Renderer* renderer) : Tool(ToolMode::Camera), renderer(renderer) {}

void CameraTool::onMousePress(QPointF pos)
{
    mousePos = pos;
}

void CameraTool::onMouseMove(QPointF pos)
{
    QPointF newMousePos = pos;
    QPointF offset = newMousePos - mousePos;

    if (isCtrlHeld) {
        renderer->panCamera(offset.x(), -offset.y());
    } else {
        renderer->moveCamera(offset.x(), -offset.y());
    }
    mousePos = pos;
}

void CameraTool::onKeyDown(Qt::Key key)
{
    if (key == Qt::Key_Control) {
        isCtrlHeld = true;
    }
}

void CameraTool::onKeyUp(Qt::Key key)
{
    if (key == Qt::Key_Control) {
        isCtrlHeld = false;
    }
}
