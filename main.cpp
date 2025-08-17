#include <QApplication>

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
    SelectionManager* selectionManager = new SelectionManager(scene);

    Renderer* renderer = new Renderer(scene, selectionManager);

    MainWindow w(scene, renderer, toolManager, selectionManager, historyManager);

    toolManager->addTool(ToolMode::Select, new SelectTool(w.getGLWidget(), renderer, selectionManager));

    w.setMinimumSize(400, 300);
    w.resize(1200, 900);
    w.show();
    return a.exec();
}
