#pragma once

#include <qaction.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class TopToolbar : public QToolBar
{
    Q_OBJECT

public:
    QActionGroup* editModeActionGroup;
    QActionGroup* renderModeActionGroup;

    TopToolbar(QWidget* parent = nullptr);
};
