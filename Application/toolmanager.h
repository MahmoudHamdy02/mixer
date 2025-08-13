#pragma once

#include "editmodes.h"
#include "rendermodes.h"
#include "toolmodes.h"

class ToolManager
{
public:
    static ToolMode selectedTool;

    static EditMode selectedEditMode;

    static RenderMode selectedRenderMode;
};
