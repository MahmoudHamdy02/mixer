#include "toptoolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
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

    QAction* wireframeAction = new QAction("Wireframe", actionGroup);
    wireframeAction->setCheckable(true);
    wireframeAction->setChecked(true);

    QAction* flatAction = new QAction("Flat", actionGroup);
    flatAction->setCheckable(true);

    QAction* renderAction = new QAction("Render", actionGroup);
    renderAction->setCheckable(true);

    addAction(wireframeAction);
    addAction(flatAction);
    addAction(renderAction);
}
