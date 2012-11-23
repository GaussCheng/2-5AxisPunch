#include "testbrightnesspage.h"
#include "ui_testbrightnesspage.h"
#include <QMessageBox>

TestBrightnessPage::TestBrightnessPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestBrightnessPage)
{
    ui->setupUi(this);
    QString cmd("BackLight on ");
    currentBrightness_ = 9;
    cmd += QString::number(9 - currentBrightness_);
}

TestBrightnessPage::~TestBrightnessPage()
{
    delete ui;
}

void TestBrightnessPage::changeEvent(QEvent *e)
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

void TestBrightnessPage::on_minusButton_clicked()
{
    QString cmd("BackLight on ");
    --currentBrightness_;
    if(currentBrightness_ < 0)
    {
        currentBrightness_ = 9;
        cmd += QString::number(9 - currentBrightness_);
        ::system(cmd.toAscii());
        QMessageBox info(this);
        info.setWindowTitle(QString::fromUtf8("亮度测试"));
        info.setText(QString::fromUtf8("亮度正常吗？"));
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
            testDescription_ = QString::fromUtf8("测试通过");
            emit TestFinished();
        }
        else if(ret == QMessageBox::No)
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("测试没通过");
            emit TestFinished();
        }
        return;
    }
    cmd += QString::number(9 - currentBrightness_);
    ::system(cmd.toAscii());

}
