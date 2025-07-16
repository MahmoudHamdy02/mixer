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
#include "selectionmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Renderer *renderer;
    SelectionManager* selectionManager;

    QMenuBar *menubar;
    LeftToolbar *leftToolbar;
    TopToolbar *topToolbar;
    RightSidebar *rightSidebar;
    QOpenGLWidget *glWidget;

    void setupMenubar();
    void setupLeftToolbar();
    void setupTopToolbar();

public:
    MainWindow(Renderer *renderer, SelectionManager *selectionManager, QWidget *parent = nullptr);
    ~MainWindow();
};
