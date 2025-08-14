#include "toptoolbar.h"

#include <qaction.h>
#include <qactiongroup.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qsizepolicy.h>
#include <qtoolbar.h>
#include <qwidget.h>

#include "editmodes.h"
#include "rendermodes.h"

TopToolbar::TopToolbar(QWidget* parent) : QToolBar(parent)
{
    setFloatable(false);
    setMovable(false);
    setOrientation(Qt::Orientation::Horizontal);

    editModeActionGroup = new QActionGroup(this);

    QAction* objectEditAction = new QAction(EditModeString::OBJECT, editModeActionGroup);
    objectEditAction->setCheckable(true);
    objectEditAction->setChecked(true);

    QAction* vertexEditAction = new QAction(EditModeString::VERTEX, editModeActionGroup);
    vertexEditAction->setCheckable(true);

    addAction(objectEditAction);
    addAction(vertexEditAction);

    // Horizontal spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addWidget(spacer);

    renderModeActionGroup = new QActionGroup(this);

    QAction* wireframeAction = new QAction(RenderModeString::WIREFRAME, renderModeActionGroup);
    wireframeAction->setCheckable(true);

    QAction* flatAction = new QAction(RenderModeString::FLAT, renderModeActionGroup);
    flatAction->setCheckable(true);
    flatAction->setChecked(true);

    QAction* renderAction = new QAction(RenderModeString::RENDERED, renderModeActionGroup);
    renderAction->setCheckable(true);

    addAction(wireframeAction);
    addAction(flatAction);
    addAction(renderAction);
}
