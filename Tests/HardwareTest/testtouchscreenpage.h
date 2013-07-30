#ifndef TESTTOUCHSCREENPAGE_H
#define TESTTOUCHSCREENPAGE_H

#include "testpagebase.h"

class QPushButton;

namespace Ui {
class TestTouchScreenPage;
}

class TestTouchScreenPage : public TestPageBase
{
    Q_OBJECT
    
public:
    explicit TestTouchScreenPage(QWidget *parent = 0);
    ~TestTouchScreenPage();
    
    bool IsTestPassed() const { return isTestPassed_;}
    QString TestDescription() const { return testDescription_;}

protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *e);
    
private slots:
    void on_clearButton_clicked();

    void on_passButton_clicked();

    void on_rejectButton_clicked();

    void OnTestButtonClicked();

private:
    Ui::TestTouchScreenPage *ui;

    bool isTestPassed_;
    QString testDescription_;
    QList<QPushButton*> testButtons_;
};

#endif // TESTTOUCHSCREENPAGE_H
