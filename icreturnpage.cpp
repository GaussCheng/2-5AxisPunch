#include "icreturnpage.h"
#include "ui_icreturnpage.h"
#include "icvirtualhost.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"

ICReturnPage::ICReturnPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICReturnPage),
    isRan_(false)
{
    ui->setupUi(this);
}

ICReturnPage::~ICReturnPage()
{
    delete ui;
}

void ICReturnPage::showEvent(QShowEvent *e)
{
    ui->label->setText(tr("Please press the start button to execute return"));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefresh()));
    QDialog::showEvent(e);
}

void ICReturnPage::hideEvent(QHideEvent *e)
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefresh()));
    QDialog::hideEvent(e);
}

void ICReturnPage::changeEvent(QEvent *e)
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

void ICReturnPage::StatusRefresh()
{
    int isReturn = ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::DbgX0).toInt();
    if(isReturn > 0)
    {
        ui->label->setText(tr("Returnning"));
        isRan_ = true;
    }
    else if(isReturn == 0)
    {
        if(isRan_)
        {
            ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
            isRan_ = false;
            this->accept();
        }
    }
}

void ICReturnPage::on_stopBtn_clicked()
{
    this->reject();
}
