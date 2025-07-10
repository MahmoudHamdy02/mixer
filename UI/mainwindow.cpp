#include "mainwindow.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qwidget.h>

#include "Widgets/glwidget.h"
#include "Widgets/lefttoolbar.h"
#include "Widgets/rightsidebar.h"
#include "Widgets/toptoolbar.h"
#include "renderer.h"

MainWindow::MainWindow(Renderer* renderer, QWidget* parent) : QMainWindow(parent), renderer(renderer)
{
    setupMenubar();
    setupLeftToolbar();
    setupTopToolbar();

    addToolBar(Qt::LeftToolBarArea, leftToolbar);
    addToolBar(Qt::TopToolBarArea, topToolbar);

    // Resizable horizontal layout
    QSplitter* horizontalLayout = new QSplitter(this);
    horizontalLayout->setChildrenCollapsible(false);

    // OpenGL Viewport
    glWidget = new GLWidget(renderer, horizontalLayout);
    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    glWidget->setMinimumWidth(300);
    horizontalLayout->addWidget(glWidget);

    // Right sidebar
    rightSidebar = new RightSidebar(this);
    rightSidebar->setMinimumWidth(200);
    horizontalLayout->addWidget(rightSidebar);

    horizontalLayout->setStretchFactor(0, 3);
    horizontalLayout->setStretchFactor(1, 1);

    setCentralWidget(horizontalLayout);
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

void MainWindow::setupTopToolbar()
{
    topToolbar = new TopToolbar(this);

    connect(topToolbar->actionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        std::string mode = action->text().toStdString();
        if (mode == TopToolbar::Actions::FLAT)
            renderer->setRenderMode(Renderer::RenderMode::Flat);
        if (mode == TopToolbar::Actions::WIREFRAME)
            renderer->setRenderMode(Renderer::RenderMode::Wireframe);
        if (mode == TopToolbar::Actions::RENDERED)
            renderer->setRenderMode(Renderer::RenderMode::Rendered);

        glWidget->update();
    });
}
