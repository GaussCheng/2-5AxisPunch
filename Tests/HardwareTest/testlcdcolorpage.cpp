#include "testlcdcolorpage.h"
#include "ui_testlcdcolorpage.h"
#include <QMessageBox>
#include <QPushButton>

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
        QMessageBox info(this);
        info.setWindowTitle(QString::fromUtf8("测试结果"));
        info.setText(QString::fromUtf8("颜色正常吗？"));
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
        if(ret == QMessageBox::No)
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("颜色不正常");
        }
        else if(ret == QMessageBox::Yes)
        {
            isTestPassed_ = true;
            testDescription_ = QString::fromUtf8("颜色正常");
        }
        else
        {
            timer_.start(2000);
            ui->info->setText(QString::fromUtf8("2秒钟后开始LCD 颜色测试，请注意观察"));
            currentColorIndex_ = 0;
            return;
        }
        TestFinished();
    }
}
