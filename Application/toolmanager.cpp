#include "toolmanager.h"

// TODO: Connect with UI state
ToolManager::Tool ToolManager::selectedTool = ToolManager::Tool::Camera;
ToolManager::EditMode ToolManager::selectedEditMode = ToolManager::EditMode::Object;
ToolManager::RenderMode ToolManager::selectedRenderMode = ToolManager::RenderMode::Flat;
