#ifndef TESTBRIGHTNESSPAGE_H
#define TESTBRIGHTNESSPAGE_H

#include "testpagebase.h"

namespace Ui {
class TestBrightnessPage;
}

class TestBrightnessPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestBrightnessPage(QWidget *parent = 0);
    ~TestBrightnessPage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}

    
protected:
    void changeEvent(QEvent *e);
    
private slots:

    void on_minusButton_clicked();

private:
    Ui::TestBrightnessPage *ui;

    int currentBrightness_;
    bool isTestPassed_;
    QString testDescription_;
};

#endif // TESTBRIGHTNESSPAGE_H
