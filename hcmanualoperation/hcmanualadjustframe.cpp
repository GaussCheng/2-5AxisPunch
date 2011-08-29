#include "hcmanualadjustframe.h"
#include "ui_hcmanualadjustframe.h"
#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"

HCManualAdjustFrame::HCManualAdjustFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HCManualAdjustFrame),
    plusPic_(":/resource/plus(64).png"),
    minsPic_(":/resource/minus(64).png"),
    statusOnPix_(":/resource/ledgreen(16).png"),
    statusOffPix_(":/resource/ledgray(16).png"),
    currentStatus_(4, false)
{
    ui->setupUi(this);
    signalMapper_.setMapping(ui->jog1ToolButton, IC::VKEY_JOG1);
    signalMapper_.setMapping(ui->jog2ToolButton, IC::VKEY_JOG2);
    signalMapper_.setMapping(ui->jog3ToolButton, IC::VKEY_JOG3);
    signalMapper_.setMapping(ui->jog4ToolButton, IC::VKEY_JOG4);
    connect(ui->jog1ToolButton,
            SIGNAL(clicked()),
            &signalMapper_,
            SLOT(map()));
    connect(ui->jog2ToolButton,
            SIGNAL(clicked()),
            &signalMapper_,
            SLOT(map()));
    connect(ui->jog3ToolButton,
            SIGNAL(clicked()),
            &signalMapper_,
            SLOT(map()));
    connect(ui->jog4ToolButton,
            SIGNAL(clicked()),
            &signalMapper_,
            SLOT(map()));
    connect(&signalMapper_,
            SIGNAL(mapped(int)),
            this,
            SLOT(OnJogToolButtonClicked(int)));

}

HCManualAdjustFrame::~HCManualAdjustFrame()
{
    delete ui;
}

void HCManualAdjustFrame::showEvent(QShowEvent *e)
{
//    ui->jogSWButton->setIcon(QPixmap());
    //    ui->jogSWButton->setText("");
//    ui->jogSWButton->setChecked(false);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void HCManualAdjustFrame::ClearStatus()
{
    ui->jogSWButton->setChecked(false);
    ui->jogSWButton->setIcon(QPixmap()); //must behide the checked
}

void HCManualAdjustFrame::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void HCManualAdjustFrame::OnJogToolButtonClicked(int key)
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(key);
}

void HCManualAdjustFrame::on_jogSWButton_toggled(bool checked)
{
    if(checked)
    {
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_JOGINC);
        ui->jogSWButton->setIcon(plusPic_);
    }
    else
    {
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_JOGDEC);
        ui->jogSWButton->setIcon(minsPic_);
    }
}

void HCManualAdjustFrame::StatusRefreshed()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsOutputOn(10))
    {
        if(!currentStatus_.at(0))
        {
            currentStatus_.setBit(0);
            ui->jog1Status->setPixmap(statusOnPix_);
        }
    }
    else
    {
        if(currentStatus_.at(0))
        {
            currentStatus_.clearBit(0);;
            ui->jog1Status->setPixmap(statusOffPix_);
        }
    }
    if(host->IsOutputOn(11))
    {
        if(!currentStatus_.at(1))
        {
            currentStatus_.setBit(1);
            ui->jog2Status->setPixmap(statusOnPix_);
        }
    }
    else
    {
        if(currentStatus_.at(1))
        {
            currentStatus_.clearBit(1);
            ui->jog2Status->setPixmap(statusOffPix_);
        }
    }

    if(host->IsOutputOn(22))
    {
        if(!currentStatus_.at(2))
        {
            currentStatus_.setBit(2);
            ui->jog3Status->setPixmap(statusOnPix_);
        }
    }
    else
    {
        if(currentStatus_.at(2))
        {
            currentStatus_.clearBit(2);
            ui->jog3Status->setPixmap(statusOffPix_);
        }
    }

    if(host->IsOutputOn(21))
    {
        if(!currentStatus_.at(3))
        {
            currentStatus_.setBit(3);
            ui->jog4Status->setPixmap(statusOnPix_);
        }
    }
    else
    {
        if(currentStatus_.at(3))
        {
            currentStatus_.clearBit(3);
            ui->jog4Status->setPixmap(statusOffPix_);
        }
    }
}
