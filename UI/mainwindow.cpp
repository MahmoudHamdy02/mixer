#include "mainwindow.h"

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qwidget.h>

#include "Widgets/glwidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupMenubar();

    QWidget* central = new QWidget(this);
    layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(central);

    glWidget = new GLWidget(this);
    layout->addWidget(glWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenubar()
{
    menubar = new QMenuBar(this);

    // File menu
    QMenu* file = new QMenu("File", this);
    menubar->addMenu(file);

    // File -> Exit
    QAction* exitAction = new QAction("Exit", this);
    file->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    setMenuBar(menubar);
}
