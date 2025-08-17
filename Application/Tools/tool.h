#pragma once

#include <qnamespace.h>
#include <qpoint.h>

#include "toolmodes.h"

class Tool
{
public:
    const ToolMode toolMode;

    Tool(ToolMode toolMode) : toolMode(toolMode) {};
    virtual ~Tool() = default;

    virtual void onMousePress(QPointF){};
    virtual void onMouseMove(QPointF){};
    virtual void onMouseRelease(QPointF){};
    virtual void onKeyDown(Qt::Key){};
    virtual void onKeyUp(Qt::Key){};
};
