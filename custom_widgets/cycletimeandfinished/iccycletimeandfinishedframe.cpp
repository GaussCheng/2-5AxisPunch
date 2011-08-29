#include "iccycletimeandfinishedframe.h"
#include "ui_iccycletimeandfinishedframe.h"

//ICCycleTimeAndFinishedFrame * ICCycleTimeAndFinishedFrame::instance_ = NULL;

ICCycleTimeAndFinishedFrame::ICCycleTimeAndFinishedFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICCycleTimeAndFinishedFrame)
{
    ui->setupUi(this);

    InitInterface();
}

ICCycleTimeAndFinishedFrame::~ICCycleTimeAndFinishedFrame()
{
    delete ui;
}

void ICCycleTimeAndFinishedFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICCycleTimeAndFinishedFrame::SetCycleTime(const QString & cycleTime)
{
    ui->loopTimeLabel->setText(cycleTime + " s");
}

void ICCycleTimeAndFinishedFrame::SetFinished(int finishedCount)
{
    ui->finishedProductsLabel->setText(QString::number(finishedCount));
}

void ICCycleTimeAndFinishedFrame::SetAlarmInfo(const QString & alarmInfo)
{
    ui->alarmLabel->setText(alarmInfo);
}

void ICCycleTimeAndFinishedFrame::InitInterface()
{
    ui->alarmLabel->setStyleSheet("background-color: rgb(192,192,192); border-radius: 6px; border: 3px solid gray; font-size: 15px; color: red;");
}
