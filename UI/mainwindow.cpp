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
#include "toolmanager.h"

MainWindow::MainWindow(Renderer* renderer, SelectionManager* selectionManager, QWidget* parent)
    : QMainWindow(parent), renderer(renderer), selectionManager(selectionManager)
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
    glWidget = new GLWidget(renderer, selectionManager, horizontalLayout);
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

    connect(leftToolbar->actionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        std::string tool = action->text().toStdString();
        if (tool == LeftToolbar::Tools::CAMERA)
            ToolManager::selectedTool = ToolManager::Tool::Camera;
        if (tool == LeftToolbar::Tools::SELECT)
            ToolManager::selectedTool = ToolManager::Tool::Select;
        if (tool == LeftToolbar::Tools::MOVE)
            ToolManager::selectedTool = ToolManager::Tool::Move;
    });
}

void MainWindow::setupTopToolbar()
{
    topToolbar = new TopToolbar(this);

    connect(topToolbar->renderModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        std::string mode = action->text().toStdString();
        if (mode == TopToolbar::RenderMode::FLAT)
            ToolManager::selectedRenderMode = ToolManager::RenderMode::Flat;
        else if (mode == TopToolbar::RenderMode::WIREFRAME)
            ToolManager::selectedRenderMode = ToolManager::RenderMode::Wireframe;
        else if (mode == TopToolbar::RenderMode::RENDERED)
            ToolManager::selectedRenderMode = ToolManager::RenderMode::Rendered;

        glWidget->update();
    });

    connect(topToolbar->editModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        std::string mode = action->text().toStdString();
        if (mode == TopToolbar::EditMode::OBJECT)
            ToolManager::selectedEditMode = ToolManager::EditMode::Object;
        else if (mode == TopToolbar::EditMode::VERTEX)
            ToolManager::selectedEditMode = ToolManager::EditMode::Vertex;

        glWidget->update();
    });
}
