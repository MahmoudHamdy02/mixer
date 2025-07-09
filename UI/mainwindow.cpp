#include "mainwindow.h"

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qwidget.h>

#include "Widgets/glwidget.h"
#include "Widgets/lefttoolbar.h"
#include "renderer.h"

MainWindow::MainWindow(Renderer* renderer, QWidget* parent) : QMainWindow(parent)
{
    setupMenubar();
    setupLeftToolbar();

    QWidget* central = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    addToolBar(Qt::LeftToolBarArea, leftToolbar);

    glWidget = new GLWidget(renderer, central);
    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(glWidget);

    central->setLayout(layout);
    setCentralWidget(central);
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

void MainWindow::setupLeftToolbar()
{
    leftToolbar = new LeftToolbar(this);
}
