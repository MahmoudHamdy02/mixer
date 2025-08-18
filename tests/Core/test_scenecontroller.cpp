#include <QtTest/qtest.h>
#include <qobject.h>
#include <qtmetamacros.h>

#include <QtTest/QSignalSpy>
#include <memory>
#include <vector>

#include "scenecontroller.h"

class TestSceneController : public QObject
{
    Q_OBJECT

    SceneController* scene = nullptr;

private slots:
    void initTestCase()
    {
        qRegisterMetaType<std::vector<std::shared_ptr<Mesh>>>();
    }

    void init()
    {
        scene = new SceneController();
    }

    void cleanup()
    {
        delete scene;
    }

    void testInitialState()
    {
        QVERIFY(scene->getMeshes().empty());
    }

    void testAddMesh()
    {
        auto mesh = std::make_shared<Mesh>("Mesh1");
        QSignalSpy spy(scene, &SceneController::onMeshListChanged);

        scene->addMesh(mesh);

        QCOMPARE(scene->getMeshes().size(), 1);
        QCOMPARE(scene->getMeshes()[0], mesh);

        QCOMPARE(spy.count(), 1);
        auto args = spy.takeFirst();
        auto meshes = args.at(0).value<std::vector<std::shared_ptr<Mesh>>>();
        QCOMPARE(meshes.size(), 1);
    }

    void testDeleteMesh()
    {
        auto mesh = std::make_shared<Mesh>("Mesh2");
        scene->addMesh(mesh);

        QSignalSpy spy(scene, &SceneController::onMeshListChanged);
        scene->deleteMesh(mesh);

        QVERIFY(scene->getMeshes().empty());
        QCOMPARE(spy.count(), 1);
    }

    void testDeleteNonExistentMesh()
    {
        auto mesh = std::make_shared<Mesh>("Mesh3");
        QSignalSpy spy(scene, &SceneController::onMeshListChanged);

        scene->deleteMesh(mesh);

        QVERIFY(scene->getMeshes().empty());
        QCOMPARE(spy.count(), 0);
    }
};

QTEST_MAIN(TestSceneController);
#include "test_scenecontroller.moc"
