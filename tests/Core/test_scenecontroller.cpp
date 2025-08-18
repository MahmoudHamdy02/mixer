#include <QtTest/qtest.h>
#include <qobject.h>
#include <qtmetamacros.h>

class TestSceneController : public QObject
{
    Q_OBJECT

private slots:
    void testTest()
    {
        QCOMPARE(2, 2);
    }
};

QTEST_APPLESS_MAIN(TestSceneController);
#include "test_scenecontroller.moc"
