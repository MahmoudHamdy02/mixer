#pragma once

#include <qobject.h>

#include <memory>
#include <stack>

#include "Commands/command.h"

class HistoryManager : public QObject
{
    Q_OBJECT

    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;

public:
    HistoryManager();

    void executeCommand(std::unique_ptr<Command>&& command);

    void undo();
    void redo();

signals:
    void onUndoStackSizeChanged(int size);
    void onRedoStackSizeChanged(int size);
};
