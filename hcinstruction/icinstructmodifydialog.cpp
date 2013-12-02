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
    validator_ = new QIntValidator(0, 30000, this);
    returnStepValidator = new QIntValidator(-32767, 32767, this);
    ui->delayTimeEdit->SetDecimalPlaces(2);
    ui->delayTimeEdit->setValidator(validator_);
    ui->limitTimeEdit->setValidator(validator_);
    ui->limitTimeEdit->SetDecimalPlaces(2);
    /*****************BUG177，178*******************************/
    validator = new QIntValidator(0, 255, this);
    ui->speedEdit->setValidator(validator);
    
    posValidator_ = new QIntValidator(0, 65530 * qPow(10, SECTION_DECIMAL), this);
    ifposValidator_ = new QIntValidator(0, 6553, this);
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
    
    ui->horizontalBox->hide();
    ui->verticalBox->hide();
    
    ui->delayLabel->setText(tr("Delay Time"));
    ui->delayTimeEdit->SetDecimalPlaces(2);
    ui->delayUnitLabel->show();
    ui->delayTimeEdit->setValidator(validator_);
    ui->limitLabel->hide();
    ui->limitTimeEdit->hide();
    ui->limitUnitLabel->hide();
    bool isMoldCount = false;


    if(item->IsAction())
    {
        if( item->Action() <= ICMold::GB)
        {
            ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
            ICVirtualHost::ICSystemParameter addr;
            switch(item->Action())
            {
            case ICMold::GX:
                addr = ICVirtualHost::SYS_X_Maxium;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GY:
                addr = ICVirtualHost::SYS_Y_Maxium;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GZ:
                addr = ICVirtualHost::SYS_Z_Maxium;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GP:
                addr = ICVirtualHost::SYS_P_Maxium;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GQ:
                addr = ICVirtualHost::SYS_Q_Maxium;
                posValidator_->setBottom(0);
                ifposValidator_->setBottom(0);
                break;
            case ICMold::GA:
                addr = ICVirtualHost::SYS_C_Length;
                posValidator_->setBottom(-50);
                ifposValidator_->setBottom(-50);
                break;
            case ICMold::GB:
                addr = ICVirtualHost::SYS_C_Length;
                posValidator_->setBottom(-50);
                ifposValidator_->setBottom(-50);
                break;
            case ICMold::GC:
                addr = ICVirtualHost::SYS_C_Length;
                posValidator_->setBottom(-50);
                ifposValidator_->setBottom(-50);
                break;
            }
            
            posValidator_->setTop(host->SystemParameter(addr).toInt() * qPow(10, SECTION_DECIMAL));
            ifposValidator_->setTop(host->SystemParameter(addr).toInt() / 10);
            validator->setTop(100);
            
            ui->positionLabel->show();
            ui->posEdit->show();
            ui->mmLabel->show();
            ui->mmLabel_2->show();
            
            ui->mmLabel_4->show();
            ui->label->show();
            ui->speedLabel->setText(tr("speed"));
            ui->speedLabel->show();
            ui->speedEdit->show();
            ui->precentLabel->show();
            ui->earlyEndCheckBox->show();
            ui->earlyEndTimeEdit->show();
            
            ui->earlySpeedDownCheckBox->show();
            ui->earlyDownSpeedTimeEdit->show();
        }
        if( item->Action() == ICMold::GZ)
        {
            ui->badProductBox->show();
        }
        /************任务：待机点姿势可以修改*******/
//        if(item->Action() == ICMold::ACTPOSEHORI || item->Action() == ICMold::ACT_PoseHori2)
//        {
//            ui->horizontalBox->show();
//            ui->verticalBox->show();
//            ui->horizontalBox->setChecked(true);
//        }
//        else if(item->Action() == ICMold::ACTPOSEVERT || item->Action() == ICMold::ACT_PoseVert2)
//        {
//            ui->horizontalBox->show();
//            ui->verticalBox->show();
//            ui->verticalBox->setChecked(true);
//        }
//        //子程序编辑，可以修改返回步号
//        else if(item->Action() == ICMold::ACTCHECKINPUT)
//        {
//            ui->delayLabel->setText(tr("Return Step"));
//            ui->delayTimeEdit->SetDecimalPlaces(0);
//            ui->delayTimeEdit->setValidator(returnStepValidator);
//            ui->delayUnitLabel->hide();
//            ui->limitLabel->show();
//            ui->limitTimeEdit->show();
//            ui->limitUnitLabel->show();
//            ui->limitTimeEdit->SetThisIntToThisText(item->Pos());
//        }
//        else if(item->Action() == ICMold::ACT_WaitMoldOpened && item->SVal() != 1)
//        {
//            ui->delayLabel->setText(tr("Limit Time"));
//        }
    }
    /******************BUG#117****************************/
    else if(item->Clip() == ICMold::ACTCLIP7ON
            || item->Clip() == ICMold::ACTCLIP8ON )
        
    {
        validator->setTop(20000);
        ui->speedLabel->setText(tr("Times"));
        ui->speedLabel->show();
        ui->speedEdit->show();
        isMoldCount = true;
        
    }
    else if(item->Clip() == ICMold:: ACT_AUX1
            || item->Clip() == ICMold:: ACT_AUX2
            || item->Clip() == ICMold:: ACT_AUX3
            || item->Clip() == ICMold:: ACT_AUX4
            || item->Clip() == ICMold:: ACT_AUX5
            || item->Clip() == ICMold:: ACT_AUX6 )
    {
        if(item->IFVal() != 0)
        {
            validator->setTop(20000);
            ui->speedLabel->setText(tr("Times"));
            ui->speedLabel->show();
            ui->speedEdit->show();
            isMoldCount = true;
        }
    }
    else if(item->Clip() == ICMold::ACTLAYOUTON)
    {
        ui->selectEdit->SetThisIntToThisText(item->SVal() + 1);
        ui->selectEdit->show();
        ui->selectLabel->show();
    }
    
//    ui->posEdit->SetThisIntToThisText(item->Pos());
    ui->posEdit->SetThisIntToThisText(item->ActualPos());
    

    if(isMoldCount)
    {
        ui->speedEdit->SetThisIntToThisText(item->ActualMoldCount());
    }
    else
    {
        ui->speedEdit->SetThisIntToThisText(item->SVal());
    }
    ui->delayTimeEdit->SetThisIntToThisText(item->DVal());
    
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
    
//    ui->earlyEndTimeEdit->SetThisIntToThisText(item->IFPos());
    ui->earlyEndTimeEdit->SetThisIntToThisText(item->ActualIfPos());
    ui->earlyDownSpeedTimeEdit->SetThisIntToThisText(item->GetEarlyDownSpeed());    ///
    
    ui->badProductBox->setChecked(item->IsBadProduct());
    
    int isok = exec();
    if(isok == QDialog::Accepted)
    {
//        if(item->Action() == ICMold::ACTCHECKINPUT)
//        {
//            item->SetPos(ui->limitTimeEdit->TransThisTextToThisInt());
//        }
//        else
//        {
//            item->SetPos(ui->posEdit->TransThisTextToThisInt());
            item->SetActualPos(ui->posEdit->TransThisTextToThisInt());
//        }
//        if(item->Clip() == ICMold::ACTLAYOUTON)
//        {
//            item->SetSVal(ui->selectEdit->TransThisTextToThisInt() - 1);
//        }
//        else if(isMoldCount)
//        {
//            item->SetActualMoldCount(ui->speedEdit->TransThisTextToThisInt());
//        }
//        else
//        {
            item->SetSVal(ui->speedEdit->TransThisTextToThisInt());
//        }
        
        item->SetDVal(ui->delayTimeEdit->TransThisTextToThisInt());
        item->SetEarlyEnd(ui->earlyEndCheckBox->isChecked());
        item->SetEarlySpeedDown(ui->earlySpeedDownCheckBox->isChecked());
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
        
        //        item->SetIFVal(ui->earlyEndCheckBox->isChecked());
        //        if(ui->earlyEndCheckBox->isChecked())
        //            item->SetIFPos(ui->earlyEndTimeEdit->TransThisTextToThisInt());
        // if(ui->earlySpeedDownCheckBox->isChecked())
        //   {
        //            item->SetESVal(ui->earlyDownSpeedTimeEdit->TransThisTextToThisInt());   ///
        //            item->SetEarlyDownSpeed(ui->earlyDownSpeedTimeEdit->TransThisTextToThisInt());
        //    }
        
        
        /************接以上任务*************/
//        if(ui->verticalBox->isChecked() && !ui->verticalBox->isHidden())
//        {
//            if(item->Action() == ICMold::ACTPOSEHORI || item->Action() == ICMold::ACTPOSEVERT)
//            {
//                item->SetAction(ICMold::ACTPOSEVERT);
//            }
//            else
//            {
//                item->SetAction(ICMold::ACT_PoseVert2);
//            }
//        }
        
//        if(ui->horizontalBox->isChecked() && !ui->horizontalBox->isHidden())
//        {
//            if(item->Action() == ICMold::ACTPOSEVERT || item->Action() == ICMold::ACTPOSEHORI)
//            {
//                item->SetAction(ICMold::ACTPOSEHORI);
//            }
//            else
//            {
//                item->SetAction(ICMold::ACT_PoseHori2);
//            }
//        }
        
    }
    return isok;
}

void ICInstructModifyDialog::on_setButton_clicked()
{
    if(currentItem == NULL)
    {
        return;
    }
    int currentPos = 0;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    uint axisLast = host->HostStatus(ICVirtualHost::AxisLastPos1).toUInt() |
//            (host->HostStatus(ICVirtualHost::AxisLastPos2).toUInt() << 16);
    uint axisLast = 0;
    if(currentItem->Action() == ICMold::GX)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisX1, axisLast);
    }
    else if(currentItem->Action() == ICMold::GY)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisY1, axisLast);
    }
    else if(currentItem->Action() == ICMold::GZ)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisZ, axisLast);
    }
    else if(currentItem->Action() == ICMold::GP)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisX2, axisLast);
    }
    else if(currentItem->Action() == ICMold::GQ)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisY2, axisLast);
    }
    else if(currentItem->Action() == ICMold::GA)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisA, axisLast);
    }
    else if(currentItem->Action() == ICMold::GB)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisB, axisLast);
    }
    else if(currentItem->Action() == ICMold::GC)
    {
        currentPos = host->GetActualPos(ICVirtualHost::ICAxis_AxisC, axisLast);
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
