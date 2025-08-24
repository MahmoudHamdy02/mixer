#include <QApplication>

#include "Geometry/primitives.h"
#include "Tools/cameratool.h"
#include "Tools/selecttool.h"
#include "UI/mainwindow.h"
#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "selectionmanager.h"
#include "toolmanager.h"
#include "toolmodes.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    HistoryManager* historyManager = new HistoryManager();
    ToolManager* toolManager = new ToolManager();

    SceneController* scene = new SceneController();

    // Add some default objects to the scene
    scene->addMesh(std::make_shared<Mesh>(Primitives::Type::Sphere, "Sphere1"));
    scene->getMeshes()[0]->translate(pmp::vec3(3, 0, 0));
    scene->addMesh(std::make_shared<Mesh>(Primitives::Type::Sphere, "Sphere2"));
    scene->getMeshes()[1]->translate(pmp::vec3(0, 0, -3));
    scene->addMesh(std::make_shared<Mesh>(Primitives::Type::Sphere, "Sphere3"));
    scene->getMeshes()[2]->translate(pmp::vec3(0, 0, -9));

    SelectionManager* selectionManager = new SelectionManager(scene);

    Renderer* renderer = new Renderer(scene, selectionManager);

    MainWindow w(scene, renderer, toolManager, selectionManager, historyManager);

    toolManager->addTool(ToolMode::Select, new SelectTool(w.getGLWidget(), renderer, selectionManager));
    toolManager->addTool(ToolMode::Camera, new CameraTool(renderer));
    toolManager->setActiveTool(ToolMode::Camera);

    w.setMinimumSize(400, 300);
    w.resize(1200, 900);
    w.show();
    return a.exec();
}
