#ifndef TESTSOUNDPAGE_H
#define TESTSOUNDPAGE_H

#include "testpagebase.h"

namespace Ui {
class TestSoundPage;
}

class TestSoundPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestSoundPage(QWidget *parent = 0);
    ~TestSoundPage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}
    

protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_soundButton_clicked();
    void BeepStop();

private:
    Ui::TestSoundPage *ui;
    bool isTestPassed_;
    QString testDescription_;
    int count_;
    int beepFD_;
};

#endif // TESTSOUNDPAGE_H
