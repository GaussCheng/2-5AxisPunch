#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QDebug>
#include "../../ictimerpool.h"

class TestICUtility : public QObject
{
    Q_OBJECT

public:
    TestICUtility();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testStart();
//    void testStart_data();

};


TestICUtility::TestICUtility()
{
}

void TestICUtility::initTestCase()
{
}

void TestICUtility::cleanupTestCase()
{
}

void TestICUtility::testStart()
{
    ICTimerPool* timerPool = ICTimerPool::Instance();
    int timerID;
    for(int i = 0; i != 48; ++i)
    {
        timerID = timerPool->Start(10, NULL, NULL);
    }
    QVERIFY(timerID == -1);
    timerID = timerPool->Start(10, this, SLOT(initTestCase()));
    timerID = timerPool->Start(10, this, SLOT(initTestCase()));
    timerPool->Stop(timerID);
    timerID = timerPool->Start(10, this, SLOT(initTestCase()));
    QVERIFY(timerID == 1);
//    QTimer timer;
//    timer.start(10);

//    QFETCH(QString, data);
//    QVERIFY2(true, "Failure");
}

//void TestICUtility::testStart_data()
//{
//    QTest::addColumn<QString>("data");
//    QTest::newRow("0") << QString();
//}

QTEST_APPLESS_MAIN(TestICUtility);

#include "tst_testicutility.moc"
