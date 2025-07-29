#pragma once

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "Widgets/lefttoolbar.h"
#include "Widgets/rightsidebar.h"
#include "Widgets/toptoolbar.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "selectionmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SceneController *scene;
    Renderer *renderer;
    SelectionManager *selectionManager;

    QMenuBar *menubar;
    LeftToolbar *leftToolbar;
    TopToolbar *topToolbar;
    RightSidebar *rightSidebar;
    QOpenGLWidget *glWidget;

    void setupMenubar();
    void setupLeftToolbar();
    void setupTopToolbar();
    void setupRightSidebar();

public:
    MainWindow(SceneController *scene, Renderer *renderer, SelectionManager *selectionManager,
               QWidget *parent = nullptr);
    ~MainWindow();
};
