#pragma once

#include <qaction.h>
#include <qmainwindow.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class TopToolbar : public QToolBar
{
    Q_OBJECT

    QActionGroup* actionGroup;

public:
    TopToolbar(QWidget* parent = nullptr);
};
