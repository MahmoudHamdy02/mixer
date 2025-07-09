#pragma once

#include <qaction.h>
#include <qmainwindow.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class LeftToolbar : public QToolBar
{
    Q_OBJECT

    QActionGroup* actionGroup;

public:
    LeftToolbar(QWidget* parent = nullptr);
};
