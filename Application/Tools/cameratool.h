#pragma once

#include <qnamespace.h>
#include <qpoint.h>

#include "Tools/tool.h"
#include "renderer.h"
class CameraTool : public Tool
{
    Renderer* renderer;

    QPointF mousePos;
    bool isCtrlHeld = false;

public:
    CameraTool(Renderer* renderer);

    void onMousePress(QPointF pos) override;
    void onMouseMove(QPointF pos) override;
    void onKeyDown(Qt::Key key) override;
    void onKeyUp(Qt::Key key) override;
};
