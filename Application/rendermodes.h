#pragma once

#include <qstring.h>

enum class RenderMode { Flat, Wireframe, Rendered };

struct RenderModeString {
    inline static const QString WIREFRAME = "Wireframe";
    inline static const QString FLAT = "Flat";
    inline static const QString RENDERED = "Rendered";
};
