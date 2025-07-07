#include "mainwindow.h"

#include <qmenubar.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupMenubar();
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
