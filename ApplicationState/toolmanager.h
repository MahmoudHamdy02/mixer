#pragma once

class ToolManager
{
public:
    enum class Tool { Camera, Select, Move };
    static Tool selectedTool;
};
