#include "lefttoolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qnamespace.h>
#include <qtoolbar.h>

LeftToolbar::LeftToolbar(QWidget* parent) : QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);
    setOrientation(Qt::Orientation::Vertical);

    actionGroup = new QActionGroup(this);

    QAction* cameraAction = new QAction("Camera", actionGroup);
    cameraAction->setCheckable(true);
    cameraAction->setChecked(true);

    QAction* selectAction = new QAction("Select", actionGroup);
    selectAction->setCheckable(true);

    QAction* moveAction = new QAction("Move", actionGroup);
    moveAction->setCheckable(true);

    addAction(cameraAction);
    addAction(selectAction);
    addAction(moveAction);
}
