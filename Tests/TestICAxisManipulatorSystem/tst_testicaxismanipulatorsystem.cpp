#include <QtCore/QString>
#include <QtTest/QtTest>

#include "icbackuputility.h"
#include "icdataformatchecker.h"
#include "icprogramformatchecker.h"
#include "icconfigformatchecker.h"


#define TEST_BACKUP_SRC "test_backup_src"
#define TEST_BACKUP_DEST "test_backup_dest"
#define TEST_BACKUP_DEST1 "test_backup_dest1"


class TestICAxisManipulatorSystem : public QObject
{
    Q_OBJECT
    
public:
    TestICAxisManipulatorSystem();
    
private Q_SLOTS:
    void initTestCase();
    void testCase1();
    void testICBackupUtility_Init();
    void testICBackupUtility_SetBackupSrcDir();
    void testICBackupUtility_SetBackupDestDir();
    void testICBackupUtility_BackupDir();
    void testICBackupUtility_RestoreDir();

    void testICDataFormatChecker_Init();
    void testICDataFormatChecker_SetContent();
    void testICDataFormatChecker_CheckRowCount();
    void testICDataFormatChecker_CheckColumCount();
    void testICDataFormatChecker_CheckDataVal();
    void testICProgramFormatChecker_Check();
    void testICConfigFormatChecker_Check();
    void cleanupTestCase();
};

TestICAxisManipulatorSystem::TestICAxisManipulatorSystem()
{
}

void TestICAxisManipulatorSystem::initTestCase()
{
    ::system("touch test_backup_src/test1");
    ::system("touch test_backup_src/test2.act");
    ::system("touch test_backup_src/test2.fnc");
    ICBackupUtility backupUtility(TEST_BACKUP_SRC, TEST_BACKUP_DEST);
    QStringList ret;
       ret<<"test1"<<"test2.act"<<"test2.fnc";
    QCOMPARE(backupUtility.SrcDirFiles(), ret);
    QCOMPARE(backupUtility.DestDirFiles(), QStringList());
}

void TestICAxisManipulatorSystem::cleanupTestCase()
{
    ::system("rm test_backup_src/*");
    ::system("rm test_backup_dest/*");
    ::system("rm -R test_backup_dest1");
//    ::system("rm test_backup_dest/.CopyEnd");
}

void TestICAxisManipulatorSystem::testCase1()
{
    QVERIFY2(true, "Failure");
}

void TestICAxisManipulatorSystem::testICBackupUtility_Init()
{
    ICBackupUtility backupUtility(TEST_BACKUP_SRC, TEST_BACKUP_DEST);
    QCOMPARE(backupUtility.SrcDir(), QString(TEST_BACKUP_SRC));
    QCOMPARE(backupUtility.DestDir(), QString(TEST_BACKUP_DEST));
}

void TestICAxisManipulatorSystem::testICBackupUtility_SetBackupSrcDir()
{
    ICBackupUtility backupUtility;
    backupUtility.SetBackupSrcDir(TEST_BACKUP_SRC);
    QCOMPARE(backupUtility.SrcDir(), QString(TEST_BACKUP_SRC));
}

void TestICAxisManipulatorSystem::testICBackupUtility_SetBackupDestDir()
{
    ICBackupUtility backupUtility;
    backupUtility.SetBackupDestDir(TEST_BACKUP_DEST);
    QCOMPARE(backupUtility.DestDir(),QString(TEST_BACKUP_DEST));
}

void TestICAxisManipulatorSystem::testICBackupUtility_BackupDir()
{
    ICBackupUtility backupUtility;
    QCOMPARE(backupUtility.BackupDir(), false);
    QCOMPARE(backupUtility.BackupDir(TEST_BACKUP_SRC, TEST_BACKUP_DEST1), true);
    QCOMPARE(backupUtility.BackupDir(TEST_BACKUP_SRC, TEST_BACKUP_DEST, QStringList()<<"*.act"<<"*.fnc"), true);
    QCOMPARE(backupUtility.DestDirFiles().contains("test2.act"), QBool(true));
    QCOMPARE(backupUtility.DestDirFiles().contains("test2.fnc"), QBool(true));
    QCOMPARE(backupUtility.DestDirFiles().contains("test1"), QBool(false));

    QCOMPARE(backupUtility.BackupDir(TEST_BACKUP_SRC, TEST_BACKUP_DEST),
             true);
    QCOMPARE(backupUtility.SrcDirFiles().size(), 3);
    QCOMPARE(backupUtility.SrcDirFiles(), backupUtility.DestDirFiles());
    QCOMPARE(backupUtility.DestDirFiles(true).contains(".CopyEnd") == true, true);

    QCOMPARE(backupUtility.BackupDir(), true);
    QCOMPARE(backupUtility.SrcDirFiles().size(), 3);
    QCOMPARE(backupUtility.SrcDirFiles(), backupUtility.DestDirFiles());
    QCOMPARE(backupUtility.DestDirFiles(true).contains(".CopyEnd") == true, true);

    QCOMPARE(backupUtility.BackupDir(QStringList()<<"*.act"<<"*.fnc"), true);

}

void TestICAxisManipulatorSystem::testICBackupUtility_RestoreDir()
{
    ::system("rm test_backup_src/*");
    ICBackupUtility backupUtility;
    QCOMPARE(backupUtility.RestoreDir(), false);
    QCOMPARE(backupUtility.RestoreDir(TEST_BACKUP_DEST, TEST_BACKUP_SRC), true);
    QCOMPARE(backupUtility.SrcDirFiles(), backupUtility.DestDirFiles());
    QCOMPARE(backupUtility.DestDirFiles(true).contains(".CopyEnd") == false, true);
    QCOMPARE(backupUtility.RestoreDir(QStringList()<<"*.act"<<"*.fnc"), true);
    QCOMPARE(backupUtility.RestoreDir(TEST_BACKUP_DEST, TEST_BACKUP_SRC, QStringList()<<"*.act"<<"*.fnc"), true);

    ::system("rm test_backup_dest/.CopyEnd");
    QCOMPARE(backupUtility.RestoreDir(TEST_BACKUP_DEST, TEST_BACKUP_SRC), false);

}

void TestICAxisManipulatorSystem::testICDataFormatChecker_Init()
{
    ICDataFormatChecker dataFormatChecker;
    QCOMPARE(dataFormatChecker.Content(), QString());
    QCOMPARE(dataFormatChecker.Check(QString()), false);
}

void TestICAxisManipulatorSystem::testICDataFormatChecker_SetContent()
{
    ICDataFormatChecker dataFormatChecker;
    dataFormatChecker.SetContent(QString("123"));
    QCOMPARE(dataFormatChecker.Content(), QString("123"));
}

void TestICAxisManipulatorSystem::testICDataFormatChecker_CheckRowCount()
{
    ICDataFormatChecker dataFormatChecker;
    QString text;
    text += "A\n";
    text += "B\n";
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 0, ICDataFormatChecker::kCompareLargger), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 3, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 2, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 1, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 3, ICDataFormatChecker::kCompareSmaller), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 2, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 1, ICDataFormatChecker::kCompareEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 2, ICDataFormatChecker::kCompareEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 3, ICDataFormatChecker::kCompareEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 1, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 2, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 3, ICDataFormatChecker::kCompareLarggerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 1, ICDataFormatChecker::kCompareSmallerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 2, ICDataFormatChecker::kCompareSmallerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(text, 3, ICDataFormatChecker::kCompareSmallerOrEqual), true);

    dataFormatChecker.SetContent(text);
    QCOMPARE(dataFormatChecker.CheckRowCount(0, ICDataFormatChecker::kCompareLargger), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(3, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(2, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(1, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(3, ICDataFormatChecker::kCompareSmaller), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(2, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(1, ICDataFormatChecker::kCompareEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(2, ICDataFormatChecker::kCompareEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(3, ICDataFormatChecker::kCompareEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(1, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(2, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(3, ICDataFormatChecker::kCompareLarggerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(1, ICDataFormatChecker::kCompareSmallerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckRowCount(2, ICDataFormatChecker::kCompareSmallerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckRowCount(3, ICDataFormatChecker::kCompareSmallerOrEqual), true);

}

void TestICAxisManipulatorSystem::testICDataFormatChecker_CheckColumCount()
{
    ICDataFormatChecker dataFormatChecker;
    QString text;
    text += "A B \n";
    text += "A;B;C\n";
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 0, " ", 0, ICDataFormatChecker::kCompareLargger), true);
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 0, " ", 2, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 0, " ", 2, ICDataFormatChecker::kCompareLargger), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 1, ";", 0, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 1, ";", 2, ICDataFormatChecker::kCompareSmaller), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(text, 1, ";", 3, ICDataFormatChecker::kCompareSmaller), false);

    dataFormatChecker.SetContent(text);
    QCOMPARE(dataFormatChecker.CheckColumCount(0, " ", 0, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckColumCount(0, " ", 2, ICDataFormatChecker::kCompareLarggerOrEqual), true);
    QCOMPARE(dataFormatChecker.CheckColumCount(0, " ", 3, ICDataFormatChecker::kCompareLarggerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(1, ";", 0, ICDataFormatChecker::kCompareSmallerOrEqual), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(1, ";", 2, ICDataFormatChecker::kCompareEqual), false);
    QCOMPARE(dataFormatChecker.CheckColumCount(1, ";", 3, ICDataFormatChecker::kCompareSmallerOrEqual), true);

    text.clear();
    text += "A B \n";
    text += "A B \n";
    text += "A B \n";
    dataFormatChecker.SetContent(text);
    QCOMPARE(dataFormatChecker.CheckColumCount(-1, " ", 2, ICDataFormatChecker::kCompareEqual), true);
    text += "A B \n";
    text += "A B \n";
    text += "A B C\n";
    dataFormatChecker.SetContent(text);
    QCOMPARE(dataFormatChecker.CheckColumCount(-1, " ", 2, ICDataFormatChecker::kCompareEqual), false);

    text.clear();
    QCOMPARE(dataFormatChecker.CheckColumCount(text, -1, " ", 1, ICDataFormatChecker::kCompareEqual), false);
}

void TestICAxisManipulatorSystem::testICDataFormatChecker_CheckDataVal()
{
    ICDataFormatChecker dataFormatChecker;
    QString text;
    text += "1 2 0 3\n";
    text += "A;b;C;D\n";
    QCOMPARE(dataFormatChecker.CheckDataVal(text, 0, 1, " ", "2"), true);
    QCOMPARE(dataFormatChecker.CheckDataVal(text, 1, 2, ";", "C"), true);
    QCOMPARE(dataFormatChecker.CheckDataVal(text, 3, 2, ";", "C"), false);
    QCOMPARE(dataFormatChecker.CheckDataVal(text, 1, 5, ";", "C"), false);
    QCOMPARE(dataFormatChecker.CheckDataVal(text, -1, 1, ";", "b"), true);
    QCOMPARE(dataFormatChecker.CheckDataVal(text, -1, -1, ";", "D"), true);

    dataFormatChecker.SetContent(text);
    QCOMPARE(dataFormatChecker.CheckDataVal(0, 1, " ", "2"), true);
    QCOMPARE(dataFormatChecker.CheckDataVal(1, 2, ";", "C"), true);
}

void TestICAxisManipulatorSystem::testICProgramFormatChecker_Check()
{
    ICProgramFormatChecker checker;
    QString text;
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "1 2 \n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0,1,2,3,4,5,6,7,8,9\n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0 1 2 3 4 5 6 7 8 9\n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0 1 2 32 4 5 6 7 8 A\n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0 1\n";
    text += "0 1 2 32 4 5 6 7 8 9\n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0 1 2 32 4 5 %% 7 8 9\n";
    QCOMPARE(checker.Check(text), false);

    text.clear();
    text += "0 1 2 32 4 5 6 7 8 9\n";
    QCOMPARE(checker.Check(text), true);


    text.clear();
    text += "1 2 3 4 5 6 7 8 9 0 \n";
    text += "0 1 2 32 4 5 6 7 8 9\n";
    QCOMPARE(checker.Check(text), true);
}

void TestICAxisManipulatorSystem::testICConfigFormatChecker_Check()
{
    ICConfigFormatChecker configFormatChecker;
    QString text;
    QCOMPARE(configFormatChecker.Check(text), false);

    text += "1 2\n";
    QCOMPARE(configFormatChecker.Check(text), false);

    text.clear();
    text += "1;2\n";
    QCOMPARE(configFormatChecker.Check(text), false);

    text.clear();
    text += "A\n";
    QCOMPARE(configFormatChecker.Check(text), false);

    text.clear();
    text += "1\n";
    QCOMPARE(configFormatChecker.Check(text), true);

    text.clear();
    text += "2\n";
    text += "1\n";
    QCOMPARE(configFormatChecker.Check(text), true);

    text.clear();
    text += "2\n";
    text += "A\n";
    QCOMPARE(configFormatChecker.Check(text), false);

}

QTEST_APPLESS_MAIN(TestICAxisManipulatorSystem)

#include "tst_testicaxismanipulatorsystem.moc"
