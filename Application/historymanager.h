#pragma once

#include <memory>
#include <stack>

#include "History/command.h"
class HistoryManager
{
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;

public:
    HistoryManager();

    void executeCommand(std::unique_ptr<Command>&& command);

    void undo();
    void redo();
};
