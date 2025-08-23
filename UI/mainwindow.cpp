#include "mainwindow.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qwidget.h>

#include <memory>

#include "Widgets/glwidget.h"
#include "Widgets/lefttoolbar.h"
#include "Widgets/menubar.h"
#include "Widgets/rightsidebar.h"
#include "Widgets/toptoolbar.h"
#include "editmodes.h"
#include "historymanager.h"
#include "renderer.h"
#include "rendermodes.h"
#include "toolmanager.h"
#include "toolmodes.h"

MainWindow::MainWindow(SceneController* scene, Renderer* renderer, ToolManager* toolManager,
                       SelectionManager* selectionManager, HistoryManager* historyManager, QWidget* parent)
    : QMainWindow(parent),
      scene(scene),
      renderer(renderer),
      toolManager(toolManager),
      selectionManager(selectionManager),
      historyManager(historyManager)
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
    glWidget = new GLWidget(scene, renderer, toolManager, selectionManager, historyManager, horizontalLayout);
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

GLWidget* MainWindow::getGLWidget()
{
    return glWidget;
}

void MainWindow::setupMenubar()
{
    menubar = new MenuBar(this);

    connect(historyManager, &HistoryManager::onUndoStackSizeChanged, menubar, &MenuBar::toggleUndoButtonStatus);
    connect(historyManager, &HistoryManager::onRedoStackSizeChanged, menubar, &MenuBar::toggleRedoButtonStatus);

    connect(menubar->exitAction, &QAction::triggered, this, &MainWindow::close);
    connect(menubar->undoAction, &QAction::triggered, this, [&]() { historyManager->undo(); });
    connect(menubar->redoAction, &QAction::triggered, this, [&]() { historyManager->redo(); });
    connect(menubar->addSphereAction, &QAction::triggered, this, [&]() {
        // TODO: Create AddMeshCommand
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Sphere");
        scene->addMesh(mesh);
        renderer->queueCreateMeshGL(mesh);
        glWidget->update();
    });

    setMenuBar(menubar);
}

void MainWindow::setupLeftToolbar()
{
    leftToolbar = new LeftToolbar(this);

    connect(leftToolbar->actionGroup, &QActionGroup::triggered, this, [&](QAction* action) {
        QString tool = action->text();
        if (tool == ToolModeString::CAMERA)
            toolManager->setActiveTool(ToolMode::Camera);
        if (tool == ToolModeString::SELECT)
            toolManager->setActiveTool(ToolMode::Select);
        if (tool == ToolModeString::MOVE)
            toolManager->setActiveTool(ToolMode::Move);
    });
}

void MainWindow::setupTopToolbar()
{
    topToolbar = new TopToolbar(this);

    connect(topToolbar->renderModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString mode = action->text();
        if (mode == RenderModeString::FLAT)
            ToolManager::selectedRenderMode = RenderMode::Flat;
        else if (mode == RenderModeString::WIREFRAME)
            ToolManager::selectedRenderMode = RenderMode::Wireframe;
        else if (mode == RenderModeString::RENDERED)
            ToolManager::selectedRenderMode = RenderMode::Rendered;

        glWidget->update();
    });

    connect(topToolbar->editModeActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString mode = action->text();
        if (mode == EditModeString::OBJECT)
            ToolManager::selectedEditMode = EditMode::Object;
        else if (mode == EditModeString::VERTEX)
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
