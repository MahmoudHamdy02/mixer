#include <QtTest/qtest.h>
#include <qobject.h>
#include <qsignalspy.h>
#include <qtestcase.h>
#include <qtmetamacros.h>

#include <memory>

#include "Commands/command.h"
#include "historymanager.h"

int value = 0;

class TestHistoryManager : public QObject
{
    Q_OBJECT

    HistoryManager* historyManager = nullptr;

    class Add5Command : public Command
    {
    public:
        void execute() override
        {
            value += 5;
        }
        void undo() override
        {
            value -= 5;
        }
    };

private slots:
    void init()
    {
        historyManager = new HistoryManager();
        value = 0;
    }

    void cleanup()
    {
        delete historyManager;
    }

    void testExecuteCommand()
    {
        QSignalSpy undoSpy(historyManager, &HistoryManager::onUndoStackSizeChanged);
        QSignalSpy redoSpy(historyManager, &HistoryManager::onRedoStackSizeChanged);

        historyManager->executeCommand(std::make_unique<Add5Command>());

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        int undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        int redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);
    }

    void testExecuteAndUndoCommand()
    {
        QSignalSpy undoSpy(historyManager, &HistoryManager::onUndoStackSizeChanged);
        QSignalSpy redoSpy(historyManager, &HistoryManager::onRedoStackSizeChanged);

        historyManager->executeCommand(std::make_unique<Add5Command>());

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        int undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        int redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);

        historyManager->undo();

        QCOMPARE(value, 0);

        QCOMPARE(undoSpy.count(), 1);
        undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 0);

        QCOMPARE(redoSpy.count(), 1);
        redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 1);
    }

    void testExecuteUndoRedoCommand()
    {
        QSignalSpy undoSpy(historyManager, &HistoryManager::onUndoStackSizeChanged);
        QSignalSpy redoSpy(historyManager, &HistoryManager::onRedoStackSizeChanged);

        historyManager->executeCommand(std::make_unique<Add5Command>());

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        int undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        int redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);

        historyManager->undo();

        QCOMPARE(value, 0);

        QCOMPARE(undoSpy.count(), 1);
        undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 0);

        QCOMPARE(redoSpy.count(), 1);
        redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 1);

        historyManager->redo();

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);
    }

    void testExecuteResetsRedoStack()
    {
        QSignalSpy undoSpy(historyManager, &HistoryManager::onUndoStackSizeChanged);
        QSignalSpy redoSpy(historyManager, &HistoryManager::onRedoStackSizeChanged);

        historyManager->executeCommand(std::make_unique<Add5Command>());

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        int undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        int redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);

        historyManager->undo();

        QCOMPARE(value, 0);

        QCOMPARE(undoSpy.count(), 1);
        undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 0);

        QCOMPARE(redoSpy.count(), 1);
        redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 1);

        historyManager->executeCommand(std::make_unique<Add5Command>());

        QCOMPARE(value, 5);

        QCOMPARE(undoSpy.count(), 1);
        undoStackSize = undoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(undoStackSize, 1);

        QCOMPARE(redoSpy.count(), 1);
        redoStackSize = redoSpy.takeFirst().at(0).value<int>();
        QCOMPARE(redoStackSize, 0);
    }
};

QTEST_MAIN(TestHistoryManager);
#include "test_historymanager.moc"
