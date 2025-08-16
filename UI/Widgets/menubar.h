#pragma once

#include <qaction.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qobject.h>
#include <qwidget.h>

class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    QMenu* file;
    QAction* exitAction;

    QMenu* edit;
    QAction* undoAction;
    QAction* redoAction;

    MenuBar(QWidget* parent = nullptr);

public slots:
    void toggleUndoButtonStatus(int stackSize);
    void toggleRedoButtonStatus(int stackSize);
};
