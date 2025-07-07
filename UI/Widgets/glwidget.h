#pragma once

#include <qopenglwidget.h>
#include <qwidget.h>
class GLWidget : public QOpenGLWidget
{
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    GLWidget(QWidget* parent = nullptr);
};
