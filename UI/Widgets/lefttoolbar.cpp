#include "lefttoolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qnamespace.h>
#include <qtoolbar.h>

#include "toolmodes.h"

LeftToolbar::LeftToolbar(QWidget* parent) : QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);
    setOrientation(Qt::Orientation::Vertical);

    actionGroup = new QActionGroup(this);

    QAction* cameraAction = new QAction(ToolModeString::CAMERA, actionGroup);
    cameraAction->setCheckable(true);
    cameraAction->setChecked(true);

    QAction* selectAction = new QAction(ToolModeString::SELECT, actionGroup);
    selectAction->setCheckable(true);

    QAction* moveAction = new QAction(ToolModeString::MOVE, actionGroup);
    moveAction->setCheckable(true);

    addAction(cameraAction);
    addAction(selectAction);
    addAction(moveAction);
}
