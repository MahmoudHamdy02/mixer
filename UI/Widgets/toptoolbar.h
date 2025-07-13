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

    struct EditMode {
        inline static const QString OBJECT = "Object";
        inline static const QString VERTEX = "Vertex";
    };

    struct RenderMode {
        inline static const QString WIREFRAME = "Wireframe";
        inline static const QString FLAT = "Flat";
        inline static const QString RENDERED = "Rendered";
    };
};
