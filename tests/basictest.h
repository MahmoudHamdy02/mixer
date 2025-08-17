#include <qt6/QtCore/qobject.h>
#include <qt6/QtCore/qtmetamacros.h>

#include <QtTest/QtTest>

class BasicTest : public QObject
{
    Q_OBJECT

private slots:
    void basicTest();
};
