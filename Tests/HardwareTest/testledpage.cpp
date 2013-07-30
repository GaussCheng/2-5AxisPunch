#include "testledpage.h"
#include "ui_testledpage.h"

#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"

#include <QTimer>
#include <QMessageBox>
#include <QPushButton>

#include "conf.h"

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
    QMessageBox info(NULL);
    info.setWindowTitle(QString::fromUtf8("LED 测试"));
    info.setText(question);
    QPushButton* yes = info.addButton(QMessageBox::Yes);
    QPushButton* no  = info.addButton(QMessageBox::No);
    yes->setText(QString::fromUtf8("是"));
    no->setText(QString::fromUtf8("否"));
    yes->setFixedHeight(64);
    no->setFixedHeight(64);

    int ret = info.exec();
    return ret == QMessageBox::Yes;
}

void TestLedPage::Question()
{
    int ledFD_ = open("/dev/szhc_leds", O_WRONLY);

#ifdef HC_3AXIS
    int cmd = 2;
#else
    int cmd = 0;
#endif
    ioctl(ledFD_, cmd, 8);
    int ret = 0;
    if(IsQuestionYes(QString::fromUtf8("开模完亮吗?")))
    {
        ret |= 8;
    }
    ioctl(ledFD_, cmd, 0);
    if(IsQuestionYes(QString::fromUtf8("开模完灭吗？")))
    {
        ret &= 0x7;
    }
    ioctl(ledFD_, cmd, 4);
    if(IsQuestionYes(QString::fromUtf8("安全门亮吗?")))
    {
        ret |= 4;
    }
    ioctl(ledFD_, cmd, 0);
    if(IsQuestionYes(QString::fromUtf8("安全门灭吗？")))
    {
        ret &= 0xB;
    }
    ioctl(ledFD_, cmd, 2);
    if(IsQuestionYes(QString::fromUtf8("可关模亮吗?")))
    {
        ret |= 2;
    }
    ioctl(ledFD_, cmd, 0);
    if(IsQuestionYes(QString::fromUtf8("可关模灭吗？")))
    {
        ret &= 0xD;
    }
    ioctl(ledFD_, cmd, 1);
    if(IsQuestionYes(QString::fromUtf8("可顶针亮吗?")))
    {
        ret |= 1;
    }
    ioctl(ledFD_, cmd, 0);
    if(IsQuestionYes(QString::fromUtf8("可顶针灭吗？")))
    {
        ret &= 0xE;
    }
    if(ret != 0)
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
