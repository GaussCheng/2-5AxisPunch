#include "testtouchscreenpage.h"
#include "ui_testtouchscreenpage.h"

#include <QResizeEvent>

TestTouchScreenPage::TestTouchScreenPage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestTouchScreenPage)
{
    ui->setupUi(this);
    QPushButton* tmp;
    for(int i = 0; i != 30; ++i)
    {
        tmp = new QPushButton(QString::number(i), this);
        testButtons_.append(tmp);
        tmp->hide();
        tmp->resize(32, 32);
        connect(tmp,
                SIGNAL(clicked()),
                SLOT(OnTestButtonClicked()));
    }
}

TestTouchScreenPage::~TestTouchScreenPage()
{
    delete ui;
}

void TestTouchScreenPage::changeEvent(QEvent *e)
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

void TestTouchScreenPage::resizeEvent(QResizeEvent *e)
{
    const int splitWidth = e->size().width() / 6;
    const int splitHeight = e->size().height() / 5;
    const int marginTop = splitHeight >> 1;
    const int marginLeft = splitHeight >> 2;
    for(int i = 0; i != 5; ++i)
    {
        for(int j = 0; j != 6; ++j)
        {
            testButtons_.at(i * 6 + j)->move(j * splitWidth + marginLeft, i * splitHeight + marginTop);
        }
    }
    QWidget::resizeEvent(e);
}

void TestTouchScreenPage::on_clearButton_clicked()
{
    ui->touchTestWidget->ClearDraw();
}

void TestTouchScreenPage::on_passButton_clicked()
{
    ui->infoLabel->setText(QString::fromUtf8("请点击屏幕上的按钮"));
    ui->clearButton->hide();
    ui->passButton->hide();
    ui->rejectButton->hide();
    for(int i = 0; i != testButtons_.size(); ++i)
    {
        testButtons_[i]->show();
    }
//    emit TestFinished();
}

void TestTouchScreenPage::on_rejectButton_clicked()
{
    isTestPassed_ = false;
    testDescription_ = QString::fromUtf8("测试不通过");
//    emit TestFinished();
}

void TestTouchScreenPage::OnTestButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    button->hide();
    for(int i = 0; i != testButtons_.size(); ++i)
    {
        if(!testButtons_.at(i)->isHidden())
        {
            return;
        }
    }
    isTestPassed_ = true;
    testDescription_ = QString::fromUtf8("测试通过");
    emit TestFinished();
}
