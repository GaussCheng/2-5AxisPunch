#ifndef TESTBACKLIGHTPAGE_H
#define TESTBACKLIGHTPAGE_H

#include "testpagebase.h"

namespace Ui {
class TestBacklightPage;
}

class TestBacklightPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestBacklightPage(QWidget *parent = 0);
    ~TestBacklightPage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void OnTimeOut();
    
private:
    Ui::TestBacklightPage *ui;

    bool isTestPassed_;
    QString testDescription_;
    int count_;
    bool isOn_;
};

#endif // TESTBACKLIGHTPAGE_H
