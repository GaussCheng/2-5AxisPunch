#include "icactiondialog.h"
#include "ui_icactiondialog.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"

ICActionDialog::ICActionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICActionDialog)
{
    ui->setupUi(this);
}

ICActionDialog::~ICActionDialog()
{
    delete ui;
}

void ICActionDialog::changeEvent(QEvent *e)
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

void ICActionDialog::on_goOnButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_GoOn, 0);
//    accept();
}

void ICActionDialog::on_giveupButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_GiveUp, 0);
//    accept();
}
