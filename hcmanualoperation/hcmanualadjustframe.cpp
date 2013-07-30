#include "hcmanualadjustframe.h"
#include "ui_hcmanualadjustframe.h"
#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"
#include "iccommandkeywrapper.h"
#include "ictimerpool.h"
#include <QMessageBox>

HCManualAdjustFrame::HCManualAdjustFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HCManualAdjustFrame),
//    plusPic_(":/resource/plus(64).png"),
//    minsPic_(":/resource/minus(64).png"),
    offPixmap_(":/resource/ledgray(16).png"),
    inputOnPixmap_(":/resource/ledred(16).png"),
    outputOnPixmap_(":/resource/ledgreen(16).png"),
    currentStatus_(9, false)
{
    ui->setupUi(this);
    ICCommandKeyWrapper *wrapper;
    wrapper = new ICCommandKeyWrapper(ui->subArmAdvanceButton1, IC::VKEY_JOG2);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmAdvanceButton2, IC::VKEY_JOG3);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmBackButton1, IC::VKEY_JOG0);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmBackButton2, IC::VKEY_JOG1);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmUpButton1, IC::VKEY_JOG4);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmUpButton2, IC::VKEY_JOG5);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmDownButton1, IC::VKEY_JOG6);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->subArmDownButton2, IC::VKEY_JOG7);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->AdjustForbidButton, IC::VKEY_JOGEn);
    wrappers_.append(wrapper);
}

HCManualAdjustFrame::~HCManualAdjustFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void HCManualAdjustFrame::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
}

void HCManualAdjustFrame::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
}
void HCManualAdjustFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
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

void HCManualAdjustFrame::ChangeButtonColor()
{
    ui->AdjustForbidButton->setStyleSheet("color:red;");
}

void HCManualAdjustFrame::StatusRefreshed()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    if(host->IsInputOn(11))  //x023
    {
        if(!currentStatus_.at(0))
        {
            currentStatus_.setBit(0);
            ui->x023Status->setPixmap(inputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(0))
        {
            currentStatus_.clearBit(0);
            ui->x023Status->setPixmap(offPixmap_);
        }
    }
//    if(host->IsInputOn(14))  //x026
//    {
//        if(!currentStatus_.at(1))
//        {
//            currentStatus_.setBit(1);
//            ui->x026Status->setPixmap(inputOnPixmap_);
//        }
//    }
//    else
//    {
//        if(currentStatus_.at(1))
//        {
//            currentStatus_.clearBit(1);
//            ui->x026Status->setPixmap(offPixmap_);
//        }
//    }
    if(host->IsInputOn(16))  //x030
    {
        if(!currentStatus_.at(2))
        {
            currentStatus_.setBit(2);
            ui->x030Status->setPixmap(inputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(2))
        {
            currentStatus_.clearBit(2);
            ui->x030Status->setPixmap(offPixmap_);
        }
    }
    if(host->IsInputOn(19))   //x033
    {
        if(!currentStatus_.at(3))
        {
            currentStatus_.setBit(3);
            ui->x033Status->setPixmap(inputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(3))
        {
            currentStatus_.clearBit(3);
            ui->x033Status->setPixmap(offPixmap_);
        }
    }

    if(host->IsOutputOn(7))   //y017
    {
        if(!currentStatus_.at(4))
        {
            currentStatus_.setBit(4);
            ui->y017Status->setPixmap(outputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(4))
        {
            currentStatus_.clearBit(4);
            ui->y017Status->setPixmap(offPixmap_);
        }
    }

    if(host->IsOutputOn(10))   //y022
    {
        if(!currentStatus_.at(5))
        {
            currentStatus_.setBit(5);
            ui->y022Status->setPixmap(outputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(5))
        {
            currentStatus_.clearBit(5);
            ui->y022Status->setPixmap(offPixmap_);
        }
    }
    if(host->IsOutputOn(17))    //y031
    {
        if(!currentStatus_.at(6))
        {
            currentStatus_.setBit(6);
            ui->y031Status->setPixmap(outputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(6))
        {
            currentStatus_.clearBit(6);
            ui->y031Status->setPixmap(offPixmap_);
        }
    }
    if(host->IsOutputOn(18))  //y032
    {
        if(!currentStatus_.at(7))
        {
            currentStatus_.setBit(7);
            ui->y032Status->setPixmap(outputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(7))
        {
            currentStatus_.clearBit(7);
            ui->y032Status->setPixmap(offPixmap_);
        }
    }
    if(host->IsOutputOn(21))   //y035
    {
        if(!currentStatus_.at(8))
        {
            currentStatus_.setBit(8);
            ui->y035Status->setPixmap(outputOnPixmap_);
        }
    }
    else
    {
        if(currentStatus_.at(8))
        {
            currentStatus_.clearBit(8);
            ui->y035Status->setPixmap(offPixmap_);
        }
    }
}

void HCManualAdjustFrame::on_AdjustForbidButton_clicked()
{
    ui->AdjustForbidButton->setStyleSheet("color:green;");
}
