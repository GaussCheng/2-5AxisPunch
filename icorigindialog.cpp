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
    ui->label->setText(tr("Please press the start button to start execute origin action"));
    QDialog::showEvent(e);
}

void ICOriginDialog::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StepChanged(int)),
               this,
               SLOT(StepChanged(int)));
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
    ui->label->setText(tr("Originning..."));
    //    emit OriginStatusChanged(false);
    if(step == 90)
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        host->ChangeSystemParameter(ICVirtualHost::SYS_X_Origin, host->HostStatus(ICVirtualHost::DbgX0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_Y_Origin, host->HostStatus(ICVirtualHost::DbgY0).toUInt());
        host->ChangeSystemParameter(ICVirtualHost::SYS_Z_Origin, host->HostStatus(ICVirtualHost::DbgZ0).toUInt());
        host->SaveSystemConfig();
        this->accept();
    }
//    ui->label->setText(QString("X:%1 Y:%2 Z:%3")
//                       .arg(host->HostStatus(ICVirtualHost::DbgX0).toUInt())
//                       .arg(host->HostStatus(ICVirtualHost::DbgY0).toUInt())
//                       .arg(host->HostStatus(ICVirtualHost::DbgZ0).toUInt()));
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
