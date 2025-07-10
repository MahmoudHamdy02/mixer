#include "toptoolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qsizepolicy.h>
#include <qtoolbar.h>
#include <qwidget.h>

TopToolbar::TopToolbar(QWidget* parent) : QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);
    setOrientation(Qt::Orientation::Horizontal);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addWidget(spacer);

    actionGroup = new QActionGroup(this);

    QAction* wireframeAction = new QAction(Actions::WIREFRAME, actionGroup);
    wireframeAction->setCheckable(true);

    QAction* flatAction = new QAction(Actions::FLAT, actionGroup);
    flatAction->setCheckable(true);
    flatAction->setChecked(true);

    QAction* renderAction = new QAction(Actions::RENDERED, actionGroup);
    renderAction->setCheckable(true);

    addAction(wireframeAction);
    addAction(flatAction);
    addAction(renderAction);
}
