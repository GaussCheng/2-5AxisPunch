#include "iccycletimeandfinishedframe.h"
#include "ui_iccycletimeandfinishedframe.h"
#include <QDateTime>
#include "icalarmdetailsdialog.h"

//ICCycleTimeAndFinishedFrame * ICCycleTimeAndFinishedFrame::instance_ = NULL;

ICCycleTimeAndFinishedFrame::ICCycleTimeAndFinishedFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICCycleTimeAndFinishedFrame)
{
    ui->setupUi(this);
    InitInterface();
    alarmDetailsDialog_ = new ICAlarmDetailsDialog(this);

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
    ui->alarmLabel->setStyleSheet("background-color: rgb(192,192,192); border-radius: 6px; border: 3px solid gray; font-size: 15px; color: red;");
    ui->alarmLabel->setText(alarmInfo);
    alarmInfo_ = alarmInfo;
}

void ICCycleTimeAndFinishedFrame::SetHintInfo(const QString &hintInfo)
{
    ui->alarmLabel->setStyleSheet("background-color: rgb(192,192,192); border-radius: 6px; border: 3px solid gray; font-size: 15px; color: green;");
    ui->alarmLabel->setText(hintInfo);
}

void ICCycleTimeAndFinishedFrame::InitInterface()
{
    ui->alarmLabel->setStyleSheet("background-color: rgb(192,192,192); border-radius: 6px; border: 3px solid gray; font-size: 15px;");
}


void ICCycleTimeAndFinishedFrame::on_toolButton_clicked()
{
    if(alarmInfo_.isEmpty()) return;
    QString err = alarmInfo_.split(":")[0];
    err = err.mid(3);
    alarmDetailsDialog_->ShowDetails(err.toInt());
}
