#pragma once

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qopenglwidget.h>

#include <QHBoxLayout>
#include <QMainWindow>

#include "Widgets/toolbar.h"
#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    Toolbar *toolbar;
    QHBoxLayout *layout;
    QOpenGLWidget *glWidget;

    void setupMenubar();
    void setupToolbar();

public:
    MainWindow(Renderer *renderer, QWidget *parent = nullptr);
    ~MainWindow();
};
