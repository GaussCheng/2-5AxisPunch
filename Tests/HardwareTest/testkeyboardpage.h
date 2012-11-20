#ifndef TESTKEYBOARDPAGE_H
#define TESTKEYBOARDPAGE_H

#include <QMap>
#include "testpagebase.h"

class QToolButton;
class QRunnable;

namespace Ui {
class TestKeyboardPage;
}

enum ICVirtualKeyValue
{
    FB_NULL = 0,
    KS_ManualStatu = 1,
    KS_StopStatu = 4,
    KS_AutoStatu = 6,
    VFB_Run,
    FB_Stop,//8
    FB_Origin,
    FB_Reset,
    FB_Up,//11
    FB_Down,
    VFB_X1Add   = 13,
    VFB_X1Sub   = 14,
    VFB_Y1Sub   = 16,
    VFB_Y1Add   = 24,
    VFB_ZSub    = 25,
    VFB_ZAdd    = 26,
    VFB_Pose_Horizontal  = 17,
    VFB_Pose_Vertical   = 27,
    VFB_SP1 = 19,
    VFB_SP2 = 23,
    VFB_X2Sub   = 37,
    VFB_X2Add   = 28,
    VFB_Y2Sub   = 15,
    VFB_Y2Add   = 21,
    FB_F1       = 29, //29
    FB_F2       = 30,
    FB_F3       = 31,
    FB_F4       = 32,
    FB_F6       = 33,
    FB_F5       = 34
};

class TestKeyboardPage : public TestPageBase
{
    Q_OBJECT
    
public:

    explicit TestKeyboardPage(QWidget *parent = 0);
    ~TestKeyboardPage();

    void KeyPressed(int key);
    void KeyRelease(int key);
    void PullyChanged(int pully);
    bool IsTestPassed() const { return testResult_;}
    QString TestDescription() const { return testDescription_;}
    
private slots:
    void on_f5Button_clicked();

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::TestKeyboardPage *ui;

    void TestPassed_();
    int keyFD_;
    QMap<int, QPair<QToolButton*, int> > keyToButton_;
    QMap<int, QPair<QToolButton*, int> > pullyToButton_;
    QList<QToolButton*> buttons_;
    QRunnable* keyHandler_;
    QRunnable* knobHandler_;
    QRunnable* pullyHandler_;

    QString testDescription_;
    bool testResult_;
};

#endif // TESTKEYBOARDPAGE_H
