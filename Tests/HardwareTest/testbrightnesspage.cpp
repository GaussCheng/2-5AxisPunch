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
        if(QMessageBox::information(this,
                                 QString::fromUtf8("亮度测试"),
                                 QString::fromUtf8("亮度正常吗？"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            isTestPassed_ = true;
            testDescription_ = QString::fromUtf8("测试通过");
        }
        else
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("测试没通过");
        }
        emit TestFinished();
        currentBrightness_ = 9;
    }
    cmd += QString::number(9 - currentBrightness_);
    ::system(cmd.toAscii());

}
