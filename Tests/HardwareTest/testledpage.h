#ifndef TESTLEDPAGE_H
#define TESTLEDPAGE_H

#include "testpagebase.h"

namespace Ui {
class TestLedPage;
}

class TestLedPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestLedPage(QWidget *parent = 0);
    ~TestLedPage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void Question();
    
private:
    Ui::TestLedPage *ui;

    bool isTestPassed_;
    QString testDescription_;
};

#endif // TESTLEDPAGE_H
