#include "testsoundpage.h"
#include "ui_testsoundpage.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include <QMessageBox>
#include <QTimer>
#include "conf.h"

TestSoundPage::TestSoundPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestSoundPage),
    isTestPassed_(false),
    count_(0)
{
    ui->setupUi(this);
#ifdef HC_3AXIS
    beepFD_ = open("/dev/szhc_beep", O_WRONLY);
#endif
#ifndef HC_3AXIS
    ::system("szhc-any-char /dev/szhc_beep 0 20");
#endif
}

TestSoundPage::~TestSoundPage()
{
#ifdef HC_3AXIS
    ::close(beepFD_);
#endif
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
#ifdef HC_3AXIS
    ioctl(beepFD_, 0, NULL);
    QTimer::singleShot(200, this, SLOT(BeepStop()));
#endif
    if(count_ >= 3)
    {
        QMessageBox info(this);
        info.setWindowTitle(QString::fromUtf8("声音测试"));
        info.setText(QString::fromUtf8("声音正常吗？"));
        QPushButton* yes = info.addButton(QMessageBox::Yes);
        QPushButton* no  = info.addButton(QMessageBox::No);
        QPushButton* retry = info.addButton(QMessageBox::Retry);
        yes->setText(QString::fromUtf8("正常"));
        no->setText(QString::fromUtf8("不正常"));
        retry->setText(QString::fromUtf8("重测"));
        yes->setFixedHeight(64);
        no->setFixedHeight(64);
        retry->setFixedHeight(64);

        int ret = info.exec();
        if(ret == QMessageBox::Yes)
        {
            isTestPassed_ = true;
            testDescription_ = QString::fromUtf8("测试正常");
        }
        else if(ret == QMessageBox::No)
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("测试不正常");
        }
        else
        {
            ++count_ = 0;
            return;
        }
        emit TestFinished();
    }

}

void TestSoundPage::BeepStop()
{
    ioctl(beepFD_, 1, NULL);
}
