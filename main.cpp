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

    w.resize(800, 600);
    w.show();
    return a.exec();
}
