#pragma once

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    QHBoxLayout *layout;
    QOpenGLWidget *glWidget;

    void setupMenubar();

public:
    MainWindow(Renderer *renderer, QWidget *parent = nullptr);
    ~MainWindow();
};
