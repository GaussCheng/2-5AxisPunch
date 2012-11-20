#ifndef TESTCOMMUNICATEPAGE_H
#define TESTCOMMUNICATEPAGE_H

#include "testpagebase.h"
#include <QTimer>
#include <termios.h>

namespace Ui {
class TestCommunicatePage;
}

class TestCommunicatePage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestCommunicatePage(QWidget *parent = 0);
    ~TestCommunicatePage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private slots:
    void OnTimerOut();
    
private:
    Ui::TestCommunicatePage *ui;

    bool isTestPassed_;
    QString testDescription_;
    int serialFD_;
    QTimer timer_;
    const static int kSentSize  = 24;
    char sentBuf[kSentSize];
    int sentCount_;
    int succeesfulCount_;
    termios oldTios_;

};

#endif // TESTCOMMUNICATEPAGE_H
