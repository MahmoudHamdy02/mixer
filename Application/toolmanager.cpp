#include "toolmanager.h"

EditMode ToolManager::selectedEditMode = EditMode::Object;
RenderMode ToolManager::selectedRenderMode = RenderMode::Flat;

void ToolManager::addTool(ToolMode toolMode, Tool *tool)
{
    tools[toolMode] = tool;
}

ToolMode ToolManager::getActiveTool()
{
    return currentTool->toolMode;
}

void ToolManager::setActiveTool(ToolMode toolMode)
{
    currentTool = tools[toolMode];
}

void ToolManager::onMousePress(QPointF pos)
{
    if (currentTool)
        currentTool->onMousePress(pos);
}

void ToolManager::onMouseMove(QPointF pos)
{
    if (currentTool)
        currentTool->onMouseMove(pos);
}

void ToolManager::onMouseRelease(QPointF pos)
{
    if (currentTool)
        currentTool->onMouseRelease(pos);
}

void ToolManager::onKeyDown(Qt::Key key)
{
    if (currentTool)
        currentTool->onKeyDown(key);
}

void ToolManager::onKeyUp(Qt::Key key)
{
    if (currentTool)
        currentTool->onKeyUp(key);
}
