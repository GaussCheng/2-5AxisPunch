#include "icinstructmodifydialog.h"
#include "ui_icinstructmodifydialog.h"
#include "icmold.h"
#include "icvirtualhost.h"
#include "config.h"
#include <qmath.h>

ICInstructModifyDialog::ICInstructModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICInstructModifyDialog),
    currentItem(NULL)
{
    ui->setupUi(this);
    /*****************BUG#120********************************/
    validator_ = new QIntValidator(0, 65530, this);
    speedValidator_ = new QIntValidator(0, 100, this);
    ui->delayTimeEdit->SetDecimalPlaces(2);
    ui->delayTimeEdit->setValidator(validator_);
    ui->limitTimeEdit->setValidator(validator_);
    ui->limitTimeEdit->SetDecimalPlaces(2);
    /*****************BUG177，178*******************************/
    validator = new QIntValidator(0, 255, this);
    ui->speedEdit->setValidator(speedValidator_);
    
//    posValidator_ = new QIntValidator(0, 65530 * qPow(10, SECTION_DECIMAL), this);
    posValidator_ = new QIntValidator(-32760, 32760, this);
    ifposValidator_ = new QIntValidator(0, 65530, this);
    ui->posEdit->SetDecimalPlaces(POS_DECIMAL);
    ui->posEdit->setValidator(posValidator_);

    esdValidator_ = new QIntValidator(0, 30, this);
    ui->earlyDownSpeedTimeEdit->setValidator(esdValidator_);
    
    ui->earlyEndTimeEdit->SetDecimalPlaces(IFPOS_DECIMAL);
    ui->earlyEndTimeEdit->setValidator(ifposValidator_);
    ui->selectEdit->setValidator(new QIntValidator(1, 4, this));
    
    ui->posEdit->SetModalKeyboard(true);
    ui->speedEdit->SetModalKeyboard(true);
    ui->delayTimeEdit->SetModalKeyboard(true);
    ui->selectEdit->SetModalKeyboard(true);
    ui->earlyEndTimeEdit->SetModalKeyboard(true);
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
    ui->label->hide();
    ui->earlySpeedDownCheckBox->hide();
    
    
    
    
    ui->earlyDownSpeedTimeEdit->hide();
    
    
    ui->mmLabel_4->hide();
    
    ui->selectEdit->hide();
    ui->selectLabel->hide();
    
    ui->badProductBox->hide();
    
    ui->onBox->hide();
    ui->offBox->hide();
    
    ui->delayLabel->setText(tr("Time"));
    ui->delayTimeEdit->SetDecimalPlaces(2);
    ui->delayUnitLabel->show();
    ui->delayTimeEdit->setValidator(validator_);
    ui->limitLabel->hide();
    ui->limitTimeEdit->hide();
    ui->limitUnitLabel->hide();

    ui->delayTimeEdit->SetDecimalPlaces(2);
    if(item->IsAction())
    {

        if( item->Action() <= ICMold::GB || item->Action() == ICMold::GARC)
        {
            ui->delayTimeEdit->SetDecimalPlaces(2);
            ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
            ICVirtualHost::ICSystemParameter addr;
            int action = item->Action();
            if(action == ICMold::GARC)
                action = item->IFPos();
            switch(action)
            {
            case ICMold::GX:
                addr = ICVirtualHost::SYS_X_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GY:
                addr = ICVirtualHost::SYS_Y_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GZ:
                addr = ICVirtualHost::SYS_Z_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GP:
                addr = ICVirtualHost::SYS_P_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GQ:
                addr = ICVirtualHost::SYS_Q_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GA:
                addr = ICVirtualHost::SYS_A_Length;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GB:
                addr = ICVirtualHost::SYS_B_Length;
                posValidator_->setBottom(-50);
                ifposValidator_->setBottom(-50);
                break;
            case ICMold::GC:
                addr = ICVirtualHost::SYS_C_Length;
                posValidator_->setBottom(-50);
                ifposValidator_->setBottom(-50);
                break;
            }
            
            speedValidator_->setRange(0, 100);
//            ui->speedEdit->SetThisIntToThisText(item->SVal());
//            posValidator_->setTop(host->SystemParameter(addr).toInt() * qPow(10, SECTION_DECIMAL));
            posValidator_->setTop(qint16(host->SystemParameter(ICVirtualHost::ICSystemParameter(addr - 1)).toInt()));
            posValidator_->setBottom(qint16(host->SystemParameter(addr).toInt()));

//            posValidator_->setBottom(host->SystemParameter(addr).toInt());
//            ifposValidator_->setTop(host->SystemParameter(ICVirtualHost::ICSystemParameter(addr - 1)).toInt() / 10);
            posValidator_->setTop(host->SystemParameter(ICVirtualHost::ICSystemParameter(addr)).toInt() * 10);
            posValidator_->setBottom(qint16(host->SystemParameter(ICVirtualHost::ICSystemParameter(addr+1)).toInt()) * 10);
            ifposValidator_->setTop(host->SystemParameter(ICVirtualHost::ICSystemParameter(addr)).toInt() / 10);


            validator->setTop(100);
            
            if(!item->IsBadProduct())
            {
                ui->positionLabel->show();
                ui->posEdit->show();
                ui->mmLabel->show();
                ui->mmLabel_2->show();
                ui->mmLabel_4->show();
                ui->earlyEndCheckBox->show();
                ui->earlySpeedDownCheckBox->show();
                ui->earlyDownSpeedTimeEdit->show();
                ui->earlyEndTimeEdit->show();
            }
            ui->label->show();
            ui->speedLabel->setText(tr("speed"));
            ui->speedLabel->show();
            ui->speedEdit->show();
            ui->precentLabel->show();


        }
        else if(item->Action() != ICMold::GWait && item->Action() != ICMold::ACTEND)
        {
            ui->offBox->show();
            ui->onBox->show();
            if(item->Action() == ICMold::GStack)
            {
                ui->selectEdit->SetThisIntToThisText(item->SVal() + 1);
                ui->selectEdit->show();
                ui->selectLabel->show();
                ui->onBox->hide();
                ui->offBox->hide();

            }
            if(item->Action() == ICMold::GCondition)
            {
                speedValidator_->setRange(-100, 100);
                ui->speedLabel->setText(tr("Return"));
                ui->speedLabel->show();
                ui->speedEdit->show();
                ui->precentLabel->hide();
//                ui->speedEdit->SetThisIntToThisText(item->SVal());
            }
            if(item->IFVal() & 0x7F)
            {
                ui->onBox->setChecked(true);
                ui->offBox->setChecked(false);
            }
            else
            {
                ui->onBox->setChecked(false);
                ui->offBox->setChecked(true);
            }
        }
        if( item->Action() == ICMold::GZ)
        {
//            ui->badProductBox->show();
        }
        else if(item->Action() == ICMold::GWait ||
                item->Action() == ICMold::GMWait ||
                item->Action() == ICMold::GCondition ||
                item->Action() == ICMold::GCheckX)
        {
            ui->delayTimeEdit->SetDecimalPlaces(1);
        }
        else if(item->Action() == ICMold::ACTEND||
                item->Action() == ICMold::GStack ||
                item->Action() == ICMold::GEuOut ||
                item->Action() == ICMold::GMOut)
        {
            ui->delayTimeEdit->SetDecimalPlaces(1);
        }
        else if(item->Action() >= ICMold::GOutY && item->Action() <= ICMold::GTwoXTwoY)
        {
            ui->delayTimeEdit->SetDecimalPlaces(2);
        }
    }

    ui->posEdit->SetThisIntToThisText(item->ActualPos());
    ui->delayTimeEdit->SetThisIntToThisText(item->DVal());
    ui->speedEdit->SetThisIntToThisText(item->SVal());
    
    if(item->IsEarlyEnd())  // is early end checked?
    {
        ui->earlyEndTimeEdit->setEnabled(true);
        ui->earlyEndCheckBox->setChecked(true);
    }
    if(item->IsEarlySpeedDown())   // is early speed down checked?
    {
        ui->earlyEndTimeEdit->setEnabled(true);
        ui->earlyDownSpeedTimeEdit->setEnabled(true);
        ui->earlySpeedDownCheckBox->setChecked(true);
    }
    if(item->IsEarlyEnd() == false && item->IsEarlySpeedDown() == false)
    {
        ui->earlyEndTimeEdit->setEnabled(false);
        ui->earlyDownSpeedTimeEdit->setEnabled(false);
        ui->earlySpeedDownCheckBox->setChecked(false);
        ui->earlyEndCheckBox->setChecked(false);
    }
    
    ui->earlyEndTimeEdit->SetThisIntToThisText(item->ActualIfPos());
    ui->earlyDownSpeedTimeEdit->SetThisIntToThisText(item->GetEarlyDownSpeed());    ///
    
    ui->badProductBox->setChecked(item->IsBadProduct());
    
    int isok = exec();
    if(isok == QDialog::Accepted)
    {
        item->SetActualPos(ui->posEdit->TransThisTextToThisInt());
        if(item->Action() == ICMold::GStack)
        {
            item->SetSVal(ui->selectEdit->TransThisTextToThisInt() - 1);
        }
        else
        {
            item->SetSVal(ui->speedEdit->TransThisTextToThisInt());
        }
        
        item->SetDVal(ui->delayTimeEdit->TransThisTextToThisInt());
        if(!ui->earlyEndCheckBox->isHidden())
        {
            item->SetEarlyEnd(ui->earlyEndCheckBox->isChecked());

        }
        if(!ui->earlyDownSpeedTimeEdit->isHidden())
        {
            item->SetEarlySpeedDown(ui->earlySpeedDownCheckBox->isChecked());
        }
        if(!ui->badProductBox->isHidden())
        {
            item->SetBadProduct(ui->badProductBox->isChecked());
        }
        
        if(ui->earlySpeedDownCheckBox->isChecked())
        {
//            item->SetIFPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
            item->SetActualIfPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
            item->SetEarlyDownSpeed(ui->earlyDownSpeedTimeEdit->TransThisTextToThisInt());   ///
        }
        else if(ui->earlyEndCheckBox->isChecked())
        {
//            item->SetIFPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
            item->SetActualIfPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
        }
        if(!ui->onBox->isHidden())
        {
            int ifv = item->IFVal();
            ifv &= 0x80;
            ifv |= ui->onBox->isChecked();
            item->SetIFVal(ifv);
        }
    }
    return isok;
}


quint32 ICInstructModifyDialog::GetPointValue(quint16 pos)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    quint32 s  = host->HostStatus(ICVirtualHost::DbgB0).toUInt() << 16;
    s = s + host->HostStatus(ICVirtualHost::DbgA1).toUInt();

    return ( (s >>( (pos -  ICVirtualHost::XPos)* 4 ) )& 0xF);
}



qint32 ICInstructModifyDialog::GetPosValue(ICVirtualHost::ICStatus status)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    qint16  p =  host->HostStatus(status).toInt() ;
    if(p < 0){
        qint32 v = p * 10 -  GetPointValue(status) ;
        return v;
    }
    else{
        qint32 v = p * 10 + GetPointValue(status) ;
        return v;
    }
}


void ICInstructModifyDialog::on_setButton_clicked()
{
    if(currentItem == NULL)
    {
        return;
    }
    int currentPos = 0;
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    uint axisLast = host->HostStatus(ICVirtualHost::AxisLastPos1).toUInt() |
//            (host->HostStatus(ICVirtualHost::AxisLastPos2).toUInt() << 16);
//    uint axisLast = 0;
    if(currentItem->Action() == ICMold::GX)
    {
        currentPos = GetPosValue(ICVirtualHost::XPos);
    }
    else if(currentItem->Action() == ICMold::GY)
    {
        currentPos = GetPosValue(ICVirtualHost::YPos);
    }
    else if(currentItem->Action() == ICMold::GZ)
    {
        currentPos = GetPosValue(ICVirtualHost::ZPos);
    }
    else if(currentItem->Action() == ICMold::GP)
    {
        currentPos = GetPosValue(ICVirtualHost::PPos);
    }
    else if(currentItem->Action() == ICMold::GQ)
    {
        currentPos = GetPosValue(ICVirtualHost::QPos);
    }
    ui->posEdit->SetThisIntToThisText(currentPos);
}

void ICInstructModifyDialog::on_earlyEndCheckBox_clicked(bool checked)
{
    if(checked)
    {
        ui->earlyEndTimeEdit->setEnabled(checked);
//        ui->earlySpeedDownCheckBox->setCheckState(Qt::Unchecked);
//        ui->earlyDownSpeedTimeEdit->setEnabled(false);
    }
    else if(!ui->earlySpeedDownCheckBox->isChecked())
    {
        ui->earlyEndTimeEdit->setEnabled(false);
    }
}

void ICInstructModifyDialog::on_earlySpeedDownCheckBox_clicked(bool checked)
{
//    if(checked)
//    {
//        ui->earlyEndCheckBox->setCheckState(Qt::Unchecked);
//    }
    ui->earlyDownSpeedTimeEdit->setEnabled(checked);
    if(checked)
    {
        ui->earlyEndTimeEdit->setEnabled(true);
    }
    else if(!ui->earlyEndCheckBox->isChecked())
    {
        ui->earlyEndTimeEdit->setEnabled(false);
    }
}
