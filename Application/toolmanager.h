#pragma once

#include <qnamespace.h>
#include <qpoint.h>

#include <map>

#include "Tools/tool.h"
#include "editmodes.h"
#include "rendermodes.h"
#include "toolmodes.h"

class ToolManager
{
    Tool *currentTool;
    std::map<ToolMode, Tool *> tools;

public:
    // TODO: Stores these in a State class?
    static EditMode selectedEditMode;
    static RenderMode selectedRenderMode;

    void addTool(ToolMode toolMode, Tool *tool);

    ToolMode getActiveTool();
    void setActiveTool(ToolMode toolMode);

    void onMousePress(QPointF pos);
    void onMouseMove(QPointF pos);
    void onMouseRelease(QPointF pos);
    void onKeyDown(Qt::Key key);
    void onKeyUp(Qt::Key key);
};
