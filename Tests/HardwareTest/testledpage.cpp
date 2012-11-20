#include "testledpage.h"
#include "ui_testledpage.h"

#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"

#include <QTimer>
#include <QMessageBox>

TestLedPage::TestLedPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestLedPage)
{
    ui->setupUi(this);
}

TestLedPage::~TestLedPage()
{
    delete ui;
}

void TestLedPage::showEvent(QShowEvent *e)
{
    QTimer::singleShot(2000, this, SLOT(Question()));
    TestPageBase::showEvent(e);
}

void TestLedPage::changeEvent(QEvent *e)
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

static bool IsQuestionYes(const QString& question)
{
    return QMessageBox::question(NULL,
                                 QString::fromUtf8("LED 测试"),
                                 question,
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void TestLedPage::Question()
{
    int ledFD_ = open("/dev/szhc_leds", O_WRONLY);

    ioctl(ledFD_, 0, 8);
    int ret = 0;
    if(IsQuestionYes(QString::fromUtf8("开模完亮吗?")))
    {
        ret |= 8;
    }
    ioctl(ledFD_, 0, 12);
    if(IsQuestionYes(QString::fromUtf8("安全门亮吗?")))
    {
        ret |= 4;
    }
    ioctl(ledFD_, 0, 14);
    if(IsQuestionYes(QString::fromUtf8("可关模亮吗?")))
    {
        ret |= 2;
    }
    ioctl(ledFD_, 0, 15);
    if(IsQuestionYes(QString::fromUtf8("可顶针亮吗?")))
    {
        ret |= 1;
    }
    if(ret != 0xF)
    {
        isTestPassed_ = false;
        testDescription_  = QString::fromUtf8("测试不通过");
        emit TestFinished();
        return;
    }

    ret = 0;
    ioctl(ledFD_, 0, 7);
    if(IsQuestionYes(QString::fromUtf8("开模完灭吗?")))
    {
        ret |= 8;
    }
    ioctl(ledFD_, 0, 3);
    if(IsQuestionYes(QString::fromUtf8("安全门灭吗?")))
    {
        ret |= 4;
    }
    ioctl(ledFD_, 0, 1);
    if(IsQuestionYes(QString::fromUtf8("可关模灭吗?")))
    {
        ret |= 2;
    }
    ioctl(ledFD_, 0, 0);
    if(IsQuestionYes(QString::fromUtf8("可顶针灭吗?")))
    {
        ret |= 1;
    }
    if(ret != 0xF)
    {
        isTestPassed_ = false;
        testDescription_  = QString::fromUtf8("测试不通过");
        emit TestFinished();
        return;
    }
    isTestPassed_ = true;
    testDescription_  = QString::fromUtf8("测试通过");
    emit TestFinished();

}
