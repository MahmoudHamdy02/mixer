#pragma once

#include "Tools/tool.h"
#include "Widgets/glwidget.h"
#include "pmp/mat_vec.h"
#include "renderer.h"
#include "selectionmanager.h"

class SelectTool : public Tool
{
    GLWidget* glWidget;
    Renderer* renderer;
    SelectionManager* selectionManager;

    QPointF mousePos;
    bool isDrawingSelectionRectangle = false;

public:
    // TODO: Use interfaces here?
    SelectTool(GLWidget* glWidget, Renderer* renderer, SelectionManager* selectionManager);

    void onMousePress(QPointF pos) override;
    void onMouseMove(QPointF pos) override;
    void onMouseRelease(QPointF pos) override;

    pmp::vec2 screenSpaceToNDC(pmp::vec2 point);
};
