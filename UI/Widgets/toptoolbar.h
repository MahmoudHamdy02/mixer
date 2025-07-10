#pragma once

#include <qaction.h>
#include <qkeysequence.h>
#include <qmainwindow.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qtoolbar.h>
#include <qwidget.h>

class TopToolbar : public QToolBar
{
    Q_OBJECT

public:
    QActionGroup* actionGroup;

    TopToolbar(QWidget* parent = nullptr);

    struct Actions {
        inline static const QString WIREFRAME = "Wireframe";
        inline static const QString FLAT = "Flat";
        inline static const QString RENDERED = "Rendered";
    };
};
