#include "testlcdcolorpage.h"
#include "ui_testlcdcolorpage.h"
#include "QMessageBox"

TestLCDColorPage::TestLCDColorPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestLCDColorPage),
    isTestPassed_(false)
{
    ui->setupUi(this);
    colors_<<"black"<<"red"<<"green"<<"blue"<<"white";
    currentColorIndex_ = 0;
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(ChangeColor()));
}

TestLCDColorPage::~TestLCDColorPage()
{
    delete ui;
}

void TestLCDColorPage::changeEvent(QEvent *e)
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

void TestLCDColorPage::showEvent(QShowEvent *e)
{
    timer_.start(2000);
    TestPageBase::showEvent(e);
}

void TestLCDColorPage::hideEvent(QHideEvent *e)
{
    timer_.stop();
    TestPageBase::hideEvent(e);
}


void TestLCDColorPage::ChangeColor()
{
    ui->info->setText("");
    QString style = QString("background-color:%1;").arg(colors_.at(currentColorIndex_++));
    this->setStyleSheet(style);
    if(currentColorIndex_ == colors_.size())
    {
        timer_.stop();
        if(QMessageBox::information(this,
                                 QString::fromUtf8("测试结果"),
                                 QString::fromUtf8("颜色正常吗？"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("颜色不正常");
        }
        else
        {
            isTestPassed_ = true;
            testDescription_ = QString::fromUtf8("颜色正常");
        }
        TestFinished();
    }
}
