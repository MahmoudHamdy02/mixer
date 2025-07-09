#pragma once

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "Widgets/lefttoolbar.h"
#include "Widgets/toptoolbar.h"
#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    LeftToolbar *leftToolbar;
    TopToolbar *topToolbar;
    QHBoxLayout *layout;
    QOpenGLWidget *glWidget;

    void setupMenubar();
    void setupLeftToolbar();
    void setupTopToolbar();

public:
    MainWindow(Renderer *renderer, QWidget *parent = nullptr);
    ~MainWindow();
};
