#pragma once

#include <qopenglwidget.h>
#include <qwidget.h>

#include "renderer.h"

class GLWidget : public QOpenGLWidget, QOpenGLExtraFunctions
{
    Renderer* renderer;

    // Camera state
    bool isMouseButtonDown = false;
    float mousePosX = 0;
    float mousePosY = 0;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

public:
    GLWidget(Renderer* renderer, QWidget* parent = nullptr);
};
