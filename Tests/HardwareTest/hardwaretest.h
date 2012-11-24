#ifndef HARDWARETEST_H
#define HARDWARETEST_H

#include <QWidget>
#include "testreportpage.h"
class TestPageBase;

namespace Ui {
class HardwareTest;
}

class HardwareTest : public QWidget
{
    Q_OBJECT
    
public:
    explicit HardwareTest(QWidget *parent = 0);
    ~HardwareTest();
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void OnSingleTestFinish();

private:

    void AddTestPageHelper_(TestPageBase* page);
    Ui::HardwareTest *ui;

    TestPageBase* keyboardWidget_;
    TestPageBase* colorWidget_;
    TestPageBase* brightnessWidget_;
    TestPageBase* backlightWidget_;
    TestPageBase* soundWidget_;
    TestPageBase* touchScreenWidget_;
    TestPageBase* testLedWidget_;
    TestPageBase* communicateWidget_;
    TestReportPage* reportPage_;
    int currentTest_;
};

#endif // HARDWARETEST_H
