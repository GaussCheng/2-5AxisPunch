#include "icinstructmodifydialog.h"
#include "ui_icinstructmodifydialog.h"
#include "icmold.h"
#include "icvirtualhost.h"

ICInstructModifyDialog::ICInstructModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICInstructModifyDialog),
    currentItem(NULL)
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
    ui->selectEdit->setValidator(new QIntValidator(0, 3, this));
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
    currentItem = item;
    ui->positionLabel->hide();
    ui->posEdit->hide();
    ui->mmLabel->hide();
    ui->speedLabel->hide();
    ui->speedEdit->hide();
    ui->precentLabel->hide();
    ui->earlyEndCheckBox->hide();
    ui->earlyEndTimeEdit->hide();
    ui->mmLabel_2->hide();
    ui->selectEdit->hide();
    ui->selectLabel->hide();
    if(item->IsAction())
    {
        if(item->Action() >= ICMold::GC && item->Action() <= ICMold::GB)
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
    else if(item->Clip() == ICMold::ACTLAYOUTON)
    {
        ui->selectEdit->SetThisIntToThisText(item->SVal());
        ui->selectEdit->show();
        ui->selectLabel->show();
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
        if(item->Clip() == ICMold::ACTLAYOUTON)
        {
            item->SetSVal(ui->selectEdit->TransThisTextToThisInt());
        }
        else
        {
            item->SetSVal(ui->speedEdit->TransThisTextToThisInt());
        }
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

void ICInstructModifyDialog::on_setButton_clicked()
{
    if(currentItem == NULL)
    {
        return;
    }
    int currentPos = 0;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(currentItem->Action() == ICMold::GX)
    {
        currentPos = host->HostStatus(ICVirtualHost::XPos).toInt();
    }
    else if(currentItem->Action() == ICMold::GY)
    {
        currentPos = host->HostStatus(ICVirtualHost::YPos).toInt();
    }
    else if(currentItem->Action() == ICMold::GZ)
    {
        currentPos = host->HostStatus(ICVirtualHost::ZPos).toInt();
    }
    ui->posEdit->SetThisIntToThisText(currentPos);
}
