#pragma once

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "Widgets/lefttoolbar.h"
#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    LeftToolbar *leftToolbar;
    QHBoxLayout *layout;
    QOpenGLWidget *glWidget;

    void setupMenubar();
    void setupLeftToolbar();

public:
    MainWindow(Renderer *renderer, QWidget *parent = nullptr);
    ~MainWindow();
};
