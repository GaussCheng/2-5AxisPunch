#include "testsoundpage.h"
#include "ui_testsoundpage.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include <QMessageBox>
#include <QTimer>

TestSoundPage::TestSoundPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestSoundPage),
    isTestPassed_(false),
    count_(0)
{
    ui->setupUi(this);
//    beepFD_ = open("/dev/szhc_beep/", O_WRONLY);
//    ioctl(beepFD_, 0,  20);
    ::system("szhc-any-char /dev/szhc_beep 0 20");
}

TestSoundPage::~TestSoundPage()
{
//    ::close(beepFD_);
    delete ui;
}

void TestSoundPage::changeEvent(QEvent *e)
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

void TestSoundPage::on_soundButton_clicked()
{
    ++count_;
    if(count_ >= 3)
    {
        if(QMessageBox::information(this,
                                 QString::fromUtf8("声音测试"),
                                 QString::fromUtf8("声音正常吗?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            isTestPassed_ = true;
            testDescription_ = QString::fromUtf8("测试正常");
        }
        else
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("测试不正常");
        }
        emit TestFinished();
    }
//    ioctl(beepFD_, 0, NULL);
//    QTimer::singleShot(200, this, SLOT(BeepStop()));

}

void TestSoundPage::BeepStop()
{
    ioctl(beepFD_, 1, NULL);
}
