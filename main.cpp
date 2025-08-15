#include <QApplication>

#include "UI/mainwindow.h"
#include "historymanager.h"
#include "renderer.h"
#include "scenecontroller.h"
#include "selectionmanager.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    HistoryManager* historyManager = new HistoryManager();

    SceneController* scene = new SceneController();
    SelectionManager* selectionManager = new SelectionManager(scene);

    Renderer* renderer = new Renderer(scene, selectionManager);

    MainWindow w(scene, renderer, selectionManager, historyManager);
    w.setMinimumSize(400, 300);
    w.resize(1200, 900);
    w.show();
    return a.exec();
}
