#pragma once

#include <qaction.h>
#include <qmainwindow.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class Toolbar : public QToolBar
{
    Q_OBJECT

    QActionGroup* actionGroup;

public:
    Toolbar(QWidget* parent = nullptr);
};
