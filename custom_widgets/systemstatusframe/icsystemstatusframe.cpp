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

    tuneMoldDialog_ = new ICTuneMoldDialog();
    InitInterface();
}

ICSystemStatusFrame::~ICSystemStatusFrame()
{
    delete ui;
}

void ICSystemStatusFrame::changeEvent(QEvent *e)
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
    Q_UNUSED(displayStatus)
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


void ICSystemStatusFrame::SetProgramStatus(const StatusLabel::DisplayStatus & displayStatus)
{
    Q_UNUSED(displayStatus)
    SetSystemStop();
    ui->immButton->setEnabled(true);
    currentStatus_ = 2;
}

void ICSystemStatusFrame::SetSystemStop()
{
    ui->autoStatusLabel->SetStatus(StatusLabel::CLEAR);
    ui->manualStatusLabel->SetStatus(StatusLabel::CLEAR);
    ui->immButton->setEnabled(false);
    currentStatus_ = 3;
}

void ICSystemStatusFrame::SetOriginStatus(const StatusLabel::DisplayStatus &displayStatus)
{
    ui->originStatusLabel->SetStatus(displayStatus);
    currentStatus_ = 4;
}

void ICSystemStatusFrame::InitInterface()
{
    ui->manualStatusLabel->SetOnStatusAttribute(":/resource/manual-opened.png");
    ui->manualStatusLabel->SetOffStatusAttibute(":/resource/manual-closed.png");
    ui->manualStatusLabel->SetStatus(StatusLabel::ONSTATUS);
    ui->autoStatusLabel->SetOnStatusAttribute(":/resource/auto-run.png");
    ui->autoStatusLabel->SetOffStatusAttibute(":/resource/auto-stop.png");
    ui->autoStatusLabel->SetStatus(StatusLabel::ONSTATUS);

    ui->originStatusLabel->SetOnStatusAttribute(":/resource/origin_off.png");
    ui->originStatusLabel->SetOffStatusAttibute(":/resource/reference-identified.png");
    ui->originStatusLabel->SetStatus(StatusLabel::CLEAR);
}

void ICSystemStatusFrame::on_immButton_clicked()
{
    tuneMoldDialog_->show();
}
