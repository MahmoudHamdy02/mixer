#pragma once

#include <qaction.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class LeftToolbar : public QToolBar
{
    Q_OBJECT

public:
    QActionGroup* actionGroup;

    LeftToolbar(QWidget* parent = nullptr);
};
