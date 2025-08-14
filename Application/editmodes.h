#pragma once

#include <qstring.h>

enum class EditMode { Object, Vertex };

struct EditModeString {
    inline static const QString OBJECT = "Object";
    inline static const QString VERTEX = "Vertex";
};
