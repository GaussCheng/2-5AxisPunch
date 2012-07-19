#include "icorigindialog.h"
#include "ui_icorigindialog.h"
#include "icvirtualhost.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"

ICOriginDialog::ICOriginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICOriginDialog)
{
    ui->setupUi(this);
    setModal(false);
    //    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    //    connect(ui->buttonBox,
    //            SIGNAL(accepted()),
    //            this,
    //            SLOT(accept()));
}

ICOriginDialog::~ICOriginDialog()
{
    delete ui;
}

void ICOriginDialog::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StepChanged(int)),
            this,
            SLOT(StepChanged(int)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    ui->label->setText(tr("Please press the start button to start execute origin action"));
    ui->yesBtn->hide();
    ui->noBtn->hide();
    QDialog::showEvent(e);
}

void ICOriginDialog::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StepChanged(int)),
               this,
               SLOT(StepChanged(int)));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    QDialog::hideEvent(e);
}

void ICOriginDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
    break;
    default:
        break;
    }
}

void ICOriginDialog::StepChanged(int step)
{
    if(step > 0 && step < 100)
    {
        ui->label->setText(tr("Originning..."));
    }
    if(step == 90)
    {
    //    emit OriginStatusChanged(false);
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        host->ChangeSystemParameter(ICVirtualHost::SYS_X_Origin, host->HostStatus(ICVirtualHost::DbgX0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_Y_Origin, host->HostStatus(ICVirtualHost::DbgY0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_Z_Origin, host->HostStatus(ICVirtualHost::DbgZ0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_P_Origin, host->HostStatus(ICVirtualHost::DbgP0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_Q_Origin, host->HostStatus(ICVirtualHost::DbgQ0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_A_Origin, host->HostStatus(ICVirtualHost::DbgA0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_B_Origin, host->HostStatus(ICVirtualHost::DbgB0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_C_Origin, host->HostStatus(ICVirtualHost::DbgC0).toUInt());
        host->SaveSystemConfig();
        this->accept();
    }
//    ui->label->setText(QString("X:%1 Y:%2 Z:%3")
//                       .arg(host->HostStatus(ICVirtualHost::DbgX0).toUInt())
//                       .arg(host->HostStatus(ICVirtualHost::DbgY0).toUInt())
//                       .arg(host->HostStatus(ICVirtualHost::DbgZ0).toUInt()));
}

void ICOriginDialog::StatusRefreshed()
{
    int hintcode = ICVirtualHost::GlobalVirtualHost()->HintNum();
    if(hintcode == 14)
    {
        ui->label->setText(tr("Do you need to auto position?"));
        ui->yesBtn->show();
        ui->noBtn->show();
    }
    else
    {
        ui->yesBtn->hide();
        ui->noBtn->hide();
    }
}


//void ICOriginDialog::on_buttonBox_accepted()
//{
//    //    if(step == 90)
//    //    {
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
//    //        emit OriginStatusChanged(true);

//    //        int ox =
//    host->ChangeSystemParameter(ICVirtualHost::SYS_X_Origin, host->HostStatus(ICVirtualHost::DbgX0).toUInt());
//    host->ChangeSystemParameter(ICVirtualHost::SYS_Y_Origin, host->HostStatus(ICVirtualHost::DbgY0).toUInt());
//    host->ChangeSystemParameter(ICVirtualHost::SYS_Z_Origin, host->HostStatus(ICVirtualHost::DbgZ0).toUInt());
//    host->SaveSystemConfig();
//    //        ui->label->setText(QString("X:%1 Y:%2 Z:%3")
//    //                           .arg(host->SystemParameter(ICVirtualHost::SYS_X_Origin).toUInt())
//    //                           .arg(host->SystemParameter(ICVirtualHost::SYS_Y_Origin).toUInt())
//    //                           .arg(host->SystemParameter(ICVirtualHost::SYS_Z_Origin).toUInt()));
//    this->accept();

//    //    }
//}

void ICOriginDialog::on_yesBtn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_GoOn, 0);
    ui->label->setText(tr("Originning"));
}

void ICOriginDialog::on_noBtn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_GiveUp, 0);
    ui->label->setText(tr("Originning"));
}

void ICOriginDialog::on_stopBtn_clicked()
{
    this->reject();
}
