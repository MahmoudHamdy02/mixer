#include "historymanager.h"

#include <memory>
#include <utility>

HistoryManager::HistoryManager() {}

void HistoryManager::executeCommand(std::unique_ptr<Command>&& command)
{
    command->execute();
    undoStack.push(std::move(command));

    // Empty redo stack
    while (!redoStack.empty()) redoStack.pop();

    emit(onUndoStackSizeChanged(undoStack.size()));
    emit(onRedoStackSizeChanged(redoStack.size()));
}

void HistoryManager::undo()
{
    if (undoStack.empty())
        return;

    std::unique_ptr<Command> command = std::move(undoStack.top());
    undoStack.pop();

    command->undo();
    redoStack.push(std::move(command));

    emit(onUndoStackSizeChanged(undoStack.size()));
    emit(onRedoStackSizeChanged(redoStack.size()));
}

void HistoryManager::redo()
{
    if (redoStack.empty())
        return;

    std::unique_ptr<Command> command = std::move(redoStack.top());
    redoStack.pop();

    command->execute();
    undoStack.push(std::move(command));

    emit(onUndoStackSizeChanged(undoStack.size()));
    emit(onRedoStackSizeChanged(redoStack.size()));
}
