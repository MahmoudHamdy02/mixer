#include <QtTest/qtest.h>
#include <qobject.h>
#include <qtmetamacros.h>

#include "scenecontroller.h"

class TestSceneController : public QObject
{
    Q_OBJECT

    SceneController* controller = nullptr;

private slots:
    void initTestCase()
    {
        controller = new SceneController();
    }

    void cleanupTestCase()
    {
        delete controller;
    }

    void test_initialState()
    {
        QVERIFY(controller->getMeshes().empty());
    }
};

QTEST_MAIN(TestSceneController);
#include "test_scenecontroller.moc"
