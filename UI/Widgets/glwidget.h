#pragma once

#include <qopenglwidget.h>
#include <qwidget.h>

#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "selectionmanager.h"
#include "toolmanager.h"

class GLWidget : public QOpenGLWidget, QOpenGLExtraFunctions
{
    SceneController* scene;
    Renderer* renderer;
    ToolManager* toolManager;
    SelectionManager* selectionManager;
    HistoryManager* historyManager;

    bool isDrawingSelectionRectangle = false;
    bool isCtrlHeld = false;
    float mousePosX = 0;
    float mousePosY = 0;

    pmp::vec2 screenSpaceToNDC(pmp::vec2);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public:
    GLWidget(SceneController* scene, Renderer* renderer, ToolManager* toolManager, SelectionManager* selectionManager,
             HistoryManager* historyManager, QWidget* parent = nullptr);
};
