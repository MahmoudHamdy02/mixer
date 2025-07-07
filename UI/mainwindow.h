#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qmenubar.h>

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;

    void setupMenubar();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif  // MAINWINDOW_H
