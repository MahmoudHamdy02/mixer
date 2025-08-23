#include "menubar.h"

#include <qkeysequence.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qwidget.h>

MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent)
{
    // File menu
    file = new QMenu("File", this);
    addMenu(file);

    // File -> Exit
    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    file->addAction(exitAction);

    // Edit menu
    edit = new QMenu("Edit", this);
    addMenu(edit);

    // Edit -> Undo
    undoAction = new QAction("Undo", this);
    undoAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    undoAction->setDisabled(true);
    edit->addAction(undoAction);

    // Edit -> Redo
    redoAction = new QAction("Redo", this);
    redoAction->setShortcut(Qt::CTRL | Qt::Key_Y);
    redoAction->setDisabled(true);
    edit->addAction(redoAction);

    // Add menu
    add = new QMenu("Add", this);
    addMenu(add);

    // Add -> Mesh
    addMesh = new QMenu("Mesh", add);
    add->addMenu(addMesh);

    // Add -> Mesh -> Cube
    addCubeAction = new QAction("Cube", addMesh);
    addMesh->addAction(addCubeAction);

    // Add -> Mesh -> Sphere
    addSphereAction = new QAction("Sphere", addMesh);
    addMesh->addAction(addSphereAction);
}

void MenuBar::toggleUndoButtonStatus(int stackSize)
{
    if (stackSize <= 0)
        undoAction->setDisabled(true);
    else
        undoAction->setDisabled(false);
}

void MenuBar::toggleRedoButtonStatus(int stackSize)
{
    if (stackSize <= 0)
        redoAction->setDisabled(true);
    else
        redoAction->setDisabled(false);
}
