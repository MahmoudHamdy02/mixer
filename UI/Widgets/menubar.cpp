#include "menubar.h"

#include <qkeysequence.h>
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
