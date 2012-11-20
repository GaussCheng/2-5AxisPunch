#include "testkeyboardpage.h"
#include "ui_testkeyboardpage.h"

#include <unistd.h>
#include <fcntl.h>

#include <QRunnable>
#include <QThreadPool>
#include <QMouseEvent>
#include <QDebug>

class KeyHandler: public QRunnable
{
public:
    KeyHandler(TestKeyboardPage* testPage):testPage_(testPage){ lastPressedKey_ = 0;}
    ~KeyHandler()
    {
        close(keyFD_);
    }
    void run()
    {
        if(testPage_ == NULL)
        {
            return;
        }
        keyFD_ = open("/dev/szhc_keyboard", O_RDONLY);
        if(keyFD_ < 0)
        {
            return;
        }
        char keyValue[5];
        int key;
        while(1)
        {
            memset(keyValue, 0, sizeof(keyValue));
            read(keyFD_, keyValue, 5);
            key = QString(keyValue).toInt();
            if(key != FB_NULL)
            {
                lastPressedKey_ = key;
                testPage_->KeyPressed(lastPressedKey_);
            }
            else
            {
                testPage_->KeyRelease(lastPressedKey_);
            }
        }
    }

private:
    TestKeyboardPage* testPage_;
    int lastPressedKey_;
    int keyFD_;
};

class KnobHandler: public QRunnable
{
public:
    KnobHandler(TestKeyboardPage* testPage):testPage_(testPage){ lastKey_ = -1;}
    ~KnobHandler()
    {
        close(swFD_);
    }
    void run()
    {
        if(testPage_ == NULL)
        {
            return;
        }
        swFD_ = open("/dev/szhc_knob", O_RDONLY);
        if(swFD_ < 0)
        {
            return;
        }
        char keyValue[5];
        int key;
        while(1)
        {
            memset(keyValue, 0, sizeof(keyValue));
            read(swFD_, keyValue, 5);
            qDebug()<<keyValue;
            key = QString(keyValue).toInt();
            testPage_->KeyPressed(key);
            if(lastKey_ != -1)
            {
                testPage_->KeyRelease(lastKey_);
            }
            lastKey_ = key;
        }
    }
private:
    TestKeyboardPage* testPage_;
    int swFD_;
    int lastKey_;
};

class PullyHandler: public QRunnable
{
public:
    PullyHandler(TestKeyboardPage* testPage):testPage_(testPage){}
    ~PullyHandler()
    {
        close(pullyFD_);
    }
    void run()
    {
        if(testPage_ == NULL)
        {
            return;
        }
        pullyFD_ = open("/dev/szhc_pulley", O_RDONLY);
        if(pullyFD_ < 0)
        {
            return;
        }
        char keyValue[2];
        int key;
        while(1)
        {
            memset(keyValue, 0, sizeof(keyValue));
            read(pullyFD_, keyValue, 2);
            key = int(keyValue[1]);
            testPage_->PullyChanged(key);
        }
    }
private:
    TestKeyboardPage* testPage_;
    int pullyFD_;
};

static bool IsAllWidgetHidden(const QList<QToolButton*> widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        if(!widgets.at(i)->isHidden())
        {
            return false;
        }
    }
    return true;
}

TestKeyboardPage::TestKeyboardPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestKeyboardPage)
{
    ui->setupUi(this);
    showFullScreen();

    keyToButton_.insert(KS_ManualStatu, qMakePair(ui->manualSW, 0));
    keyToButton_.insert(KS_StopStatu, qMakePair(ui->stopSW, 0));
    keyToButton_.insert(KS_AutoStatu, qMakePair(ui->autoSW, 0));
    keyToButton_.insert(VFB_Run, qMakePair(ui->startButton, 0));
    keyToButton_.insert(FB_Stop, qMakePair(ui->stopButton, 0));
    keyToButton_.insert(FB_Origin, qMakePair(ui->originButton, 0));
    keyToButton_.insert(FB_Reset, qMakePair(ui->returnButton, 0));
    keyToButton_.insert(FB_Up, qMakePair(ui->upButton, 0));
    keyToButton_.insert(FB_Down, qMakePair(ui->downButton, 0));
    keyToButton_.insert(FB_F1, qMakePair(ui->f1Button, 0));
    keyToButton_.insert(FB_F2, qMakePair(ui->f2Button, 0));
    keyToButton_.insert(FB_F3, qMakePair(ui->f3Button, 0));
    keyToButton_.insert(FB_F4, qMakePair(ui->f4Button, 0));
    keyToButton_.insert(FB_F5, qMakePair(ui->f5Button, 0));
    keyToButton_.insert(VFB_Y1Sub, qMakePair(ui->y1MinusButton, 0));
    keyToButton_.insert(VFB_X1Sub, qMakePair(ui->x1MinusButton, 0));
    keyToButton_.insert(VFB_ZSub, qMakePair(ui->zMinusButton, 0));
    keyToButton_.insert(VFB_X2Sub, qMakePair(ui->x2MinusButton, 0));
    keyToButton_.insert(VFB_Y2Sub, qMakePair(ui->y2MinusButton, 0));
    keyToButton_.insert(VFB_Pose_Vertical, qMakePair(ui->cPlusButton, 0));
    keyToButton_.insert(VFB_Y1Add, qMakePair(ui->y1PlusButton, 0));
    keyToButton_.insert(VFB_X1Add, qMakePair(ui->x1PlusButton, 0));
    keyToButton_.insert(VFB_ZAdd, qMakePair(ui->zPlusButton, 0));
    keyToButton_.insert(VFB_X2Add, qMakePair(ui->x2PlusButton, 0));
    keyToButton_.insert(VFB_Y2Add, qMakePair(ui->y2PlusButton, 0));
    keyToButton_.insert(VFB_Pose_Horizontal, qMakePair(ui->cMinusButton, 0));


    pullyToButton_.insert(1, qMakePair(ui->scrollUp, 0));
    pullyToButton_.insert(2, qMakePair(ui->scrollDown, 0));

    buttons_ = this->findChildren<QToolButton*>();
    testResult_ = false;

    keyHandler_ = new KeyHandler(this);
    knobHandler_ = new KnobHandler(this);
    pullyHandler_ = new PullyHandler(this);

    QThreadPool::globalInstance()->start(keyHandler_);
    QThreadPool::globalInstance()->start(knobHandler_);
    QThreadPool::globalInstance()->start(pullyHandler_);
}

TestKeyboardPage::~TestKeyboardPage()
{
    delete keyHandler_;
    delete knobHandler_;
    delete pullyHandler_;
    delete ui;
}

void TestKeyboardPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TestKeyboardPage::KeyPressed(int key)
{
    if(!keyToButton_.contains(key))
    {
        return;
    }
    QPair<QToolButton*, int> buttonAndCount = keyToButton_.value(key);
    QEvent* event = new QMouseEvent(QEvent::MouseButtonPress,
                                    QPoint(0, 0),
                                    Qt::LeftButton,
                                    Qt::NoButton,
                                    Qt::NoModifier);
    qApp->postEvent(buttonAndCount.first, event);
}

void TestKeyboardPage::KeyRelease(int key)
{
    if(!keyToButton_.contains(key))
    {
        return;
    }

    QPair<QToolButton*, int> buttonAndCount = keyToButton_.value(key);
    ++buttonAndCount.second;
    if(buttonAndCount.second >= 3)
    {
        buttonAndCount.first->hide();
    }
    QEvent* event = new QMouseEvent(QEvent::MouseButtonRelease,
                                    QPoint(0, 0),
                                    Qt::LeftButton,
                                    Qt::NoButton,
                                    Qt::NoModifier);
    qApp->postEvent(buttonAndCount.first, event);
    keyToButton_.insert(key, buttonAndCount);
    if(IsAllWidgetHidden(buttons_))
    {
        TestPassed_();
    }
}

void TestKeyboardPage::PullyChanged(int pully)
{
    if(!pullyToButton_.contains(pully))
    {
        return;
    }

    QPair<QToolButton*, int> buttonAndCount = pullyToButton_.value(pully);
    ++buttonAndCount.second;
    if(buttonAndCount.second >=20)
    {
        buttonAndCount.first->hide();
    }
    QEvent* event = new QMouseEvent(QEvent::MouseButtonPress,
                                    QPoint(0, 0),
                                    Qt::LeftButton,
                                    Qt::NoButton,
                                    Qt::NoModifier);
    qApp->postEvent(buttonAndCount.first, event);
    qApp->processEvents();
    QEvent* event1 = new QMouseEvent(QEvent::MouseButtonRelease,
                                    QPoint(0, 0),
                                    Qt::LeftButton,
                                    Qt::NoButton,
                                    Qt::NoModifier);
    qApp->postEvent(buttonAndCount.first, event1);
    pullyToButton_.insert(pully, buttonAndCount);
    if(IsAllWidgetHidden(buttons_))
    {
        TestPassed_();
    }
}

void TestKeyboardPage::TestPassed_()
{
    testResult_ = true;
    testDescription_ = QString::fromUtf8("测试通过");
    ui->label->setText(TestDescription());
    emit TestFinished();
    this->blockSignals(true);
}

void TestKeyboardPage::on_f5Button_clicked()
{
#ifdef Q_WS_X11
    emit TestFinished();
#endif
}

