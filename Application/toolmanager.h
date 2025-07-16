#pragma once

class ToolManager
{
public:
    enum class Tool { Camera, Select, Move };
    static Tool selectedTool;
    enum class EditMode { Object, Vertex };
    static EditMode selectedEditMode;
    enum class RenderMode { Wireframe, Flat, Rendered };
    static RenderMode selectedRenderMode;
};
