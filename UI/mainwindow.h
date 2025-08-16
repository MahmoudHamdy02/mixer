#pragma once

#include <qboxlayout.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "Widgets/lefttoolbar.h"
#include "Widgets/menubar.h"
#include "Widgets/rightsidebar.h"
#include "Widgets/toptoolbar.h"
#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "selectionmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SceneController *scene;
    Renderer *renderer;
    SelectionManager *selectionManager;
    HistoryManager *historyManager;

    MenuBar *menubar;
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
               HistoryManager *historyManager, QWidget *parent = nullptr);
    ~MainWindow();
};
