#include <QApplication>

#include "UI/mainwindow.h"
#include "renderer.h"
#include "scenecontroller.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SceneController* scene = new SceneController();
    Renderer* renderer = new Renderer(scene);

    MainWindow w(renderer);
    w.setMinimumSize(400, 300);
    w.resize(1200, 900);
    w.show();
    return a.exec();
}
