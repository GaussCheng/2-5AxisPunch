#include "ictunemolddialog.h"
#include "ui_ictunemolddialog.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"

ICTuneMoldDialog::ICTuneMoldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICTuneMoldDialog)
{
    ui->setupUi(this);
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(OnTimeOut()));
}

ICTuneMoldDialog::~ICTuneMoldDialog()
{
    delete ui;
}

void ICTuneMoldDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICTuneMoldDialog::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            SLOT(RefreshStatus()));
    QDialog::showEvent(e);
}

void ICTuneMoldDialog::hideEvent(QHideEvent *e)
{
    timer_.stop();
    ui->startButton->setChecked(false);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
               this,
            SLOT(RefreshStatus()));
    QDialog::hideEvent(e);
}

void ICTuneMoldDialog::on_cancelButton_clicked()
{
    this->reject();
}

void ICTuneMoldDialog::on_startButton_toggled(bool checked)
{
    if(checked)
    {
        timer_.start(500);
    }
    else
    {
        this->accept();
    }
}

void ICTuneMoldDialog::OnTimeOut()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TuneMold, 0);
}

void ICTuneMoldDialog::RefreshStatus()
{
    if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() != ICVirtualHost::Stop)
    {
        accept();
    }
}
