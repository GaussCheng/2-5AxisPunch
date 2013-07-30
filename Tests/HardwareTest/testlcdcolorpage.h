#ifndef TESTLCDCOLORPAGE_H
#define TESTLCDCOLORPAGE_H

#include <QTimer>
#include "testpagebase.h"

namespace Ui {
class TestLCDColorPage;
}

class TestLCDColorPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestLCDColorPage(QWidget *parent = 0);
    ~TestLCDColorPage();

    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

signals:
    void TestFinished();
private slots:
    void ChangeColor();
    
private:
    Ui::TestLCDColorPage *ui;

    QStringList colors_;
    int currentColorIndex_;
    QTimer timer_;
    bool isTestPassed_;
    QString testDescription_;
};

#endif // TESTLCDCOLORPAGE_H
