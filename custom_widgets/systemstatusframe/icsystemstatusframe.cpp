#include "icsystemstatusframe.h"
#include "ui_icsystemstatusframe.h"

#include "ictunemolddialog.h"

//ICSystemStatusFrame * ICSystemStatusFrame::instance_ = NULL;

ICSystemStatusFrame::ICSystemStatusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICSystemStatusFrame),
    currentStatus_(-1),
    singleCycle_(":/resource/step-running.png"),
    stopping_(":/resource/auto-stopping.png")
{
    ui->setupUi(this);
//    ui->programStatusLabel->hide();
//    setMinimumWidth(minimumSize().width() - 48);


//    this->setStyleSheet("background: gray; border: 1px solid rgb(255,255,255); \
//                        border-radius: 24px; font-size: 15px; color:rgb(255, 0, 255);");

    tuneMoldDialog_ = new ICTuneMoldDialog();
    InitInterface();
}

ICSystemStatusFrame::~ICSystemStatusFrame()
{
    delete ui;
}

void ICSystemStatusFrame::SetCurrentSpeed(const QString& currentSpeed)
{
    ui->speedStatusLabel->SetCurrentSpeed(currentSpeed);
}

QString ICSystemStatusFrame::CurrentSpeed() const
{
    return ui->speedStatusLabel->CurrentSpeed();
}

void ICSystemStatusFrame::SetManualStatus(const StatusLabel::DisplayStatus & displayStatus)
{
    if(currentStatus_ == 0)
    {
        return;
    }
    SetSystemStop();
    ui->manualStatusLabel->SetStatus(StatusLabel::ONSTATUS);
    currentStatus_ = 0;
}

void ICSystemStatusFrame::SetAutoStatus(AutoSatus status)
{
//    if(currentStatus_ == 1)
//    {
//        return;
//    }
//    SetSystemStop();
    SetSystemStop();
    if(status == SingleCycle)
    {
        ui->autoStatusLabel->setPixmap(singleCycle_);
    }
    else if(status == Stopping)
    {
        ui->autoStatusLabel->setPixmap(stopping_);
    }
    else if(status == Running)
    {
        ui->autoStatusLabel->SetStatus(StatusLabel::ONSTATUS);
    }
    else
    {
        ui->autoStatusLabel->SetStatus(StatusLabel::OFFSTATUS);
    }
    currentStatus_ = 1;
}

/*void ICSystemStatusFrame::SetReferenceStatus(const StatusLabel::DisplayStatus & displayStatus)
{
    switch(displayStatus)
    {
    case StatusLabel::ONSTATUS:
        break;
    case StatusLabel::OFFSTATUS:
        break;
    case StatusLabel::CLEAR:
        break;
    }
    ui->referenceStatusLabel->SetStatus(displayStatus);
}*/

void ICSystemStatusFrame::SetProgramStatus(const StatusLabel::DisplayStatus & displayStatus)
{
    SetSystemStop();
//    ui->programStatusLabel->SetStatus(StatusLabel::ONSTATUS);
    ui->immButton->setEnabled(true);
    currentStatus_ = 2;
}

void ICSystemStatusFrame::SetSystemStop()
{
//    if(currentStatus_ == 3)
//    {
//        return ;
//    }
    ui->autoStatusLabel->SetStatus(StatusLabel::CLEAR);
    ui->manualStatusLabel->SetStatus(StatusLabel::CLEAR);
    ui->immButton->setEnabled(false);
//    ui->programStatusLabel->SetStatus(StatusLabel::CLEAR);
//    ui->originStatusLabel->SetStatus(StatusLabel::CLEAR);
    currentStatus_ = 3;
}

void ICSystemStatusFrame::SetOriginStatus(const StatusLabel::DisplayStatus &displayStatus)
{
    if(currentStatus_ == 4)
    {
        return;
    }
//    SetSystemStop();
    ui->originStatusLabel->SetStatus(displayStatus);
    currentStatus_ = 4;
}

void ICSystemStatusFrame::InitInterface()
{
    ui->manualStatusLabel->SetOnStatusAttribute(":/resource/manual-opened.png");
    ui->manualStatusLabel->SetOffStatusAttibute(":/resource/manual-closed.png");
    ui->manualStatusLabel->SetStatus(StatusLabel::ONSTATUS);

//    ui->referenceStatusLabel->SetOnStatusAttribute(":/resource/reference-identified.png");
//    ui->referenceStatusLabel->SetOffStatusAttibute(":/resource/no-reference.png");
//    ui->referenceStatusLabel->SetStatus(StatusLabel::ONSTATUS);

    ui->autoStatusLabel->SetOnStatusAttribute(":/resource/auto-run.png");
    ui->autoStatusLabel->SetOffStatusAttibute(":/resource/auto-stop.png");
    ui->autoStatusLabel->SetStatus(StatusLabel::ONSTATUS);

//    ui->programStatusLabel->SetOnStatusAttribute(":/resource/close_normal.png");
//    ui->programStatusLabel->SetOffStatusAttibute(":/resource/close_long.png");
//    ui->programStatusLabel->SetStatus(StatusLabel::ONSTATUS);

    ui->originStatusLabel->SetOnStatusAttribute(":/resource/origin_off.png");
    ui->originStatusLabel->SetOffStatusAttibute(":/resource/reference-identified.png");
    ui->originStatusLabel->SetStatus(StatusLabel::OFFSTATUS);
//    ui->stepStatusLabel->SetOnStatusAttribute(":/resource/step-running.png");
//    ui->stepStatusLabel->SetOffStatusAttibute(":/resource/step-stoped.png");
//    ui->stepStatusLabel->SetStatus(StatusLabel::OFFSTATUS);
//    ui->stepStatusLabel->setText(QString());
}

void ICSystemStatusFrame::on_immButton_clicked()
{
    tuneMoldDialog_->show();
}
