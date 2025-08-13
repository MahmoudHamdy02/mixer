#include "mainwindow.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qwidget.h>

#include "Widgets/glwidget.h"
#include "Widgets/lefttoolbar.h"
#include "Widgets/rightsidebar.h"
#include "Widgets/toptoolbar.h"
#include "renderer.h"
#include "toolmanager.h"
#include "toolmodes.h"

MainWindow::MainWindow(SceneController* scene, Renderer* renderer, SelectionManager* selectionManager, QWidget* parent)
    : QMainWindow(parent), scene(scene), renderer(renderer), selectionManager(selectionManager)
{
    setupMenubar();
    setupLeftToolbar();
    setupTopToolbar();
    setupRightSidebar();

    addToolBar(Qt::LeftToolBarArea, leftToolbar);
    addToolBar(Qt::TopToolBarArea, topToolbar);

    // Resizable horizontal layout
    QSplitter* horizontalLayout = new QSplitter(this);
    horizontalLayout->setChildrenCollapsible(false);

    // OpenGL Viewport
    glWidget = new GLWidget(scene, renderer, selectionManager, horizontalLayout);
    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    glWidget->setMinimumWidth(300);
    horizontalLayout->addWidget(glWidget);

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

    connect(leftToolbar->actionGroup, &QActionGroup::triggered, this, [](QAction* action) {
        QString tool = action->text();
        if (tool == LeftToolbar::Tools::CAMERA)
            ToolManager::selectedTool = ToolMode::Camera;
        if (tool == LeftToolbar::Tools::SELECT)
            ToolManager::selectedTool = ToolMode::Select;
        if (tool == LeftToolbar::Tools::MOVE)
            ToolManager::selectedTool = ToolMode::Move;
    });
}

void MainWindow::setupTopToolbar()
{
    topToolbar = new TopToolbar(this);

    connect(topToolbar->renderModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString mode = action->text();
        if (mode == TopToolbar::RenderMode::FLAT)
            ToolManager::selectedRenderMode = RenderMode::Flat;
        else if (mode == TopToolbar::RenderMode::WIREFRAME)
            ToolManager::selectedRenderMode = RenderMode::Wireframe;
        else if (mode == TopToolbar::RenderMode::RENDERED)
            ToolManager::selectedRenderMode = RenderMode::Rendered;

        glWidget->update();
    });

    connect(topToolbar->editModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString mode = action->text();
        if (mode == TopToolbar::EditMode::OBJECT)
            ToolManager::selectedEditMode = EditMode::Object;
        else if (mode == TopToolbar::EditMode::VERTEX)
            ToolManager::selectedEditMode = EditMode::Vertex;

        glWidget->update();
    });
}

void MainWindow::setupRightSidebar()
{
    rightSidebar = new RightSidebar(this);

    connect(scene, &SceneController::onMeshListChanged, rightSidebar, &RightSidebar::setMeshList);

    rightSidebar->setMeshList(scene->getMeshes());
}
