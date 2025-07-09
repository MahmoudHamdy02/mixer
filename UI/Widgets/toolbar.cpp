#include "toolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qnamespace.h>
#include <qtoolbar.h>

Toolbar::Toolbar(QWidget* parent) : QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);
    setOrientation(Qt::Orientation::Vertical);

    actionGroup = new QActionGroup(this);

    QAction* selectAction = new QAction("Select", actionGroup);
    selectAction->setCheckable(true);
    selectAction->setChecked(true);

    QAction* moveAction = new QAction("Move", actionGroup);
    moveAction->setCheckable(true);

    addAction(selectAction);
    addAction(moveAction);
}
