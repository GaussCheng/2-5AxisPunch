#include "icinstructmodifydialog.h"
#include "ui_icinstructmodifydialog.h"
#include "icmold.h"

ICInstructModifyDialog::ICInstructModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICInstructModifyDialog)
{
    ui->setupUi(this);
    QIntValidator* validator = new QIntValidator(0, 600, this);
    ui->delayTimeEdit->SetDecimalPlaces(1);
    ui->delayTimeEdit->setValidator(validator);

    validator = new QIntValidator(0, 100, this);
    ui->speedEdit->setValidator(validator);

    validator = new QIntValidator(0, 65535, this);
    ui->posEdit->SetDecimalPlaces(1);
    ui->posEdit->setValidator(validator);

    ui->earlyEndTimeEdit->SetDecimalPlaces(1);
    ui->earlyEndTimeEdit->setValidator(validator);
}

ICInstructModifyDialog::~ICInstructModifyDialog()
{
    delete ui;
}

void ICInstructModifyDialog::changeEvent(QEvent *e)
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

bool ICInstructModifyDialog::ShowModifyItem(ICMoldItem *item)
{
    ui->positionLabel->hide();
    ui->posEdit->hide();
    ui->mmLabel->hide();
    ui->speedLabel->hide();
    ui->speedEdit->hide();
    ui->precentLabel->hide();
    ui->earlyEndCheckBox->hide();
    ui->earlyEndTimeEdit->hide();
    ui->mmLabel_2->hide();
    if(item->IsAction())
    {
        if(item->Action() == ICMold::GY || item->Action() == ICMold::GZ || item->Action() == ICMold::GX)
        {
            ui->positionLabel->show();
            ui->posEdit->show();
            ui->mmLabel->show();
            ui->mmLabel_2->show();

            ui->speedLabel->setText(tr("speed"));
            ui->speedLabel->show();
            ui->speedEdit->show();
            ui->precentLabel->show();
            ui->earlyEndCheckBox->show();
            ui->earlyEndTimeEdit->show();
        }
//        else if(item->Action() == ICMold::ACTCHECKINPUT)
//        {
//            ui->positionLabel->setText(tr("Check Point"));
//            ui->positionLabel->show();
//            ui->speedLabel->setText(tr("Use Sub"));
//        }
    }
    else if(item->Clip() == ICMold::ACTCLIP7ON
            || item->Clip() == ICMold::ACTCLIP8ON)
    {
        ui->speedLabel->setText(tr("Times"));
        ui->speedLabel->show();
        ui->speedEdit->show();
//        ui->positionLabel->hide();
//        ui->posEdit->hide();
//        ui->mmLabel->hide();
//        ui->precentLabel->hide();
//        ui->mmLabel_2->hide();

//            ui->precentLabel->show();
    }
//    else
//    {
//        ui->positionLabel->hide();
//        ui->posEdit->hide();
//        ui->mmLabel->hide();
//        ui->speedLabel->hide();
//        ui->speedEdit->hide();
//        ui->precentLabel->hide();
//        ui->earlyEndCheckBox->hide();
//        ui->earlyEndTimeEdit->hide();
//    }
    ui->posEdit->SetThisIntToThisText(item->Pos());
    ui->speedEdit->SetThisIntToThisText(item->SVal());
    ui->delayTimeEdit->SetThisIntToThisText(item->DVal());
    ui->earlyEndCheckBox->setChecked(item->IFVal() == 1);
    ui->earlyEndTimeEdit->SetThisIntToThisText(item->IFPos());
    int isok = exec();
    if(isok == QDialog::Accepted)
    {
        item->SetPos(ui->posEdit->TransThisTextToThisInt());
        item->SetSVal(ui->speedEdit->TransThisTextToThisInt());
        item->SetDVal(ui->delayTimeEdit->TransThisTextToThisInt());
        item->SetIFVal(ui->earlyEndCheckBox->isChecked());
        item->SetIFPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
    }
    return isok;
}

void ICInstructModifyDialog::on_earlyEndCheckBox_toggled(bool checked)
{
    ui->earlyEndTimeEdit->setEnabled(checked);
}
