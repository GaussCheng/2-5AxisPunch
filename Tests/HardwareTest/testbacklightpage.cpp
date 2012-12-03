#include "testbacklightpage.h"
#include "ui_testbacklightpage.h"
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>

#include "conf.h"

TestBacklightPage::TestBacklightPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestBacklightPage)
{
    ui->setupUi(this);
    count_ = 0;
    isOn_ = true;
}

TestBacklightPage::~TestBacklightPage()
{
    delete ui;
}

void TestBacklightPage::changeEvent(QEvent *e)
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

void TestBacklightPage::showEvent(QShowEvent *e)
{
    QTimer::singleShot(2000, this, SLOT(OnTimeOut()));
    TestPageBase::showEvent(e);
}

void TestBacklightPage::OnTimeOut()
{
    if(isOn_)
    {
        ::system("BackLight off");
        ++count_;
        isOn_ = false;
    }
    else
    {
#ifdef HC_3AXIS
        ::system("BackLight on");
#else
        ::system("BackLight on 0");
#endif
        isOn_ = true;
    }
    if(count_ >= 3 && isOn_)
    {
        QMessageBox info(this);
        info.setWindowTitle(QString::fromUtf8("背光测试"));
        info.setText(QString::fromUtf8("背光正常吗？"));
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
            testDescription_ = QString::fromUtf8("背光正常");
            emit TestFinished();
            return;
        }
        else if(ret == QMessageBox::No)
        {
            isTestPassed_ = false;
            testDescription_ = QString::fromUtf8("背光不正常");
            emit TestFinished();
            return;
        }
        else
        {
            count_ = 0;
        }
    }
    QTimer::singleShot(1000, this, SLOT(OnTimeOut()));
}
