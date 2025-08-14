#pragma once

#include <qstring.h>

enum class ToolMode { Camera, Select, Move };

struct ToolModeString {
    inline static const QString CAMERA = "Camera";
    inline static const QString SELECT = "Select";
    inline static const QString MOVE = "Move";
};
