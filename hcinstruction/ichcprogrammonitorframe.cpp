#include "ichcprogrammonitorframe.h"
#include "ui_ichcprogrammonitorframe.h"


#include "icinstructparam.h"
#include "icparameterconversion.h"
#include "icvirtualhost.h"
#include "icautorunrevise.h"
#include "icactioncommand.h"
#include "icmacrosubroutine.h"
#include "icvirtualkey.h"
#include "qmessagebox.h"
#include <QMessageBox>

ICHCProgramMonitorFrame::ICHCProgramMonitorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCProgramMonitorFrame),
    isModify_(false),
    currentStepItem_(NULL),
    isFollow_(true),
    oldStep_(-1),
    currentMoldNum_(8),
    switchOn_(":/resource/switch_on.png"),
    switchOff_(":/resource/switch_off.png")
{
    ui->setupUi(this);

    autoRunRevise_ = new ICAutoRunRevise(this);
    InitSignal();
    //    UpdateHostParam();
    //    ICInstructParam::Instance()->UpdateHostParam();
    //    moldContent_ = ICMold::CurrentMold()->MoldContent();
    connect(&timer_,
            SIGNAL(timeout()),
            this,
            SLOT(OnTimeOut()));
    connect(&refreshTimer_,
            SIGNAL(timeout()),
            SLOT(StatusRefreshed()));
    fixtureToCountMap_.insert(ICMold::ACTCLIP1ON, fixtureCount_ + 0);
    fixtureToCountMap_.insert(ICMold::ACTCLIP2ON, fixtureCount_ + 1);
    fixtureToCountMap_.insert(ICMold::ACTCLIP3ON, fixtureCount_ + 2);
    fixtureToCountMap_.insert(ICMold::ACTCLIP4ON, fixtureCount_ + 3);
    fixtureToCountMap_.insert(ICMold::ACTCLIP5ON, fixtureCount_ + 4);
    fixtureToCountMap_.insert(ICMold::ACTCLIP6ON, fixtureCount_ + 5);
    checkToFixtureMap_.insert(0, ICMold::ACTCLIP1ON);
    checkToFixtureMap_.insert(1, ICMold::ACTCLIP2ON);
    checkToFixtureMap_.insert(2, ICMold::ACTCLIP3ON);
    checkToFixtureMap_.insert(3, ICMold::ACTCLIP4ON);
    checkToFixtureMap_.insert(4, ICMold::ACTCLIP5ON);
    checkToFixtureMap_.insert(5, ICMold::ACTCLIP6ON);
    checkResultMap_.insert(0, tr("Has not teach check Fixture-1!"));
    checkResultMap_.insert(1, tr("Has not teach check Fixture-2!"));
    checkResultMap_.insert(2, tr("Has not teach check Fixture-3!"));
    checkResultMap_.insert(3, tr("Has not teach check Fixture-4!"));
    checkResultMap_.insert(4, tr("Has not teach check Sucker-1!"));
    checkResultMap_.insert(5, tr("Has not teach check Sucker-2!"));
}

ICHCProgramMonitorFrame::~ICHCProgramMonitorFrame()
{
    delete autoRunRevise_;
    delete ui;
}

void ICHCProgramMonitorFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        checkResultMap_.insert(0, tr("Has not teach check Fixture-1!"));
        checkResultMap_.insert(1, tr("Has not teach check Fixture-2!"));
        checkResultMap_.insert(2, tr("Has not teach check Fixture-3!"));
        checkResultMap_.insert(3, tr("Has not teach check Fixture-4!"));
        checkResultMap_.insert(4, tr("Has not teach check Sucker-1!"));
        checkResultMap_.insert(5, tr("Has not teach check Sucker-2!"));
        break;
    default:
        break;
    }
}

void ICHCProgramMonitorFrame::showEvent(QShowEvent *e)
{
    //    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    ICVirtualHost::GlobalVirtualHost()->SetSpeedEnable(false);
    ui->speedEnableButton->setIcon(switchOff_);
    ui->speedEnableButton->setText(tr("Speed Disable"));
    SetProduct(ICMold::CurrentMold()->MoldParam(ICMold::Product));
    UpdateHostParam();
    programListBackup_ = programList_;
    qDebug("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxxXX");
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StepChanged(int)),
            this,
            SLOT(SelectCurrentStep(int)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(SubStepChanged(uint8_t*)),
            this,
            SLOT(SubStepChanged(uint8_t*)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(MoldNumChanged(int)),
            this,
            SLOT(MoldNumChanged(int)));
    refreshTimer_.start(20);
    timer_.start(30000);

    if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() != ICVirtualHost::Auto)
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnAuto,
                                                         0,
                                                         ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
                                                         ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsFixtureCheck())
    {
        return;
    }
    memset(fixtureCount_, 0, sizeof(int) * 6);
    QList<ICMoldItem> moldItems = ICMold::UIItemToMoldItem(programList_);
    ICMoldItem tmpItem;
    int tmpClip;
    int *tmpCount;
    for(int i = 0; i != moldItems.size(); ++i)
    {
        tmpItem = moldItems.at(i);
        if(tmpItem.Action() == ICMold::ACT_Cut)
        {
            if(checkToFixtureMap_.contains(tmpItem.SVal()))
            {
                tmpCount = fixtureToCountMap_.value(checkToFixtureMap_.value(tmpItem.SVal()));
                if(*tmpCount > 0)
                {
                    *tmpCount -= 1;
                }
            }
            continue;
        }
        if(tmpItem.IsClip())
        {
            tmpClip = tmpItem.Clip();
            if(fixtureToCountMap_.contains(tmpClip))
            {
                *(fixtureToCountMap_.value(tmpClip)) += 1;
            }
        }
    }
    QString checkResult;
    bool needWarn = false;
    for(int i = 0; i != 6; ++i)
    {
        if(fixtureCount_[i] != 0)
        {
            checkResult.append(checkResultMap_.value(i) + "\n");
            needWarn = true;
        }
    }
    if(!checkResult.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             checkResult);
    }
    //    if(needWarn)
    //    {
    //        if(QMessageBox::warning(this,
    //                             tr("Warning"),
    //                             checkResult + tr("Do you want to ignor this warning?"),
    //                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    //        {
    //            ICVirtualHost::GlobalVirtualHost()->SetFixtureCheck(false);
    //        }
    //    }
}

void ICHCProgramMonitorFrame::hideEvent(QHideEvent *e)
{
    qDebug("isModify change to false in hide");
    if(isModify_)
    {
        ICMold::CurrentMold()->SetMoldContent(ICMold::UIItemToMoldItem(programList_));
        ICMold::CurrentMold()->SaveMoldFile();
        isModify_ = false;
    }
    //    modifyMap_.clear();
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StepChanged(int)),
               this,
               SLOT(SelectCurrentStep(int)));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(SubStepChanged(uint8_t*)),
               this,
               SLOT(SubStepChanged(uint8_t*)));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(MoldNumChanged(int)),
               this,
               SLOT(MoldNumChanged(int)));
    timer_.stop();
    refreshTimer_.stop();
    ICVirtualHost::GlobalVirtualHost()->SetSpeedEnable(false);
    ui->speedEnableButton->setIcon(switchOff_);
    ui->speedEnableButton->setText(tr("Speed Disable"));
    //    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop,0);
}

void ICHCProgramMonitorFrame::SetTime(int time)
{
    ui->timeLabel->setText(ICParameterConversion::TransThisIntToThisText(time, 2));
}

//void ICHCProgramMonitorFrame::SetFullTime(int fullTime)
//{
//    ui->fullTimeLabel->setText(ICParameterConversion::TransThisIntToThisText(fullTime, 1));
//}

void ICHCProgramMonitorFrame::SetProduct(int product)
{
    ui->settedProductsLabel->setText(QString::number(product));
}

//void ICHCProgramMonitorFrame::SetCurrentFinished(int currentFinished)
//{
//    ui->currentProductsLabel->setText(QString::number(currentFinished));
//}

void ICHCProgramMonitorFrame::StatusRefreshed()
{

    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    newTime_ = host->HostStatus(ICVirtualHost::DbgZ0).toUInt();

    if(newTime_ != oldTime_)
    {

        oldTime_ = newTime_;
        SetTime(newTime_);
    }
    ui->getTime->setText(ICParameterConversion::TransThisIntToThisText(host->HostStatus(ICVirtualHost::DbgY1).toUInt(), 2));
    newCycleTimes_ = host->HostStatus(ICVirtualHost::DbgX1).toUInt();
    if(newCycleTimes_ != oldCycleTimes_)
    {

        oldCycleTimes_ = newCycleTimes_;
        //        ui->cycleTimes->setText(QString::number(oldCycleTimes_));
    }
    newGoodP_ = host->HostStatus(ICVirtualHost::DbgY0).toUInt();
    if(newGoodP_ != oldGoodP_)
    {
        oldGoodP_ = newGoodP_;
        ui->goodProducts->setText(QString::number(oldGoodP_));
    }
    newStackedP_ = host->HostStatus(ICVirtualHost::DbgZ1).toUInt();
    if(newStackedP_ != oldStackedP_)
    {
        oldStackedP_ = newStackedP_;
        ui->stackedProducts->setText(QString::number(oldStackedP_));
    }
    //    if(host->CurrentStatus() != ICVirtualHost::Auto)
    //    {
    //        qDebug("isModify change to false in auto");
    //        isModify_ = false;
    //        modifyMap_.clear();
    //    }
    //    if(host->HostStatus(ICVirtualHost::DbgX0) != ICVirtualHost::AutoRunning &&
    //            host->HostStatus(ICVirtualHost::DbgX0) != ICVirtualHost::AutoStopping)
    //    {
    //        qDebug("isModify change to false in autoRunning");
    //        isModify_ = false;
    //        modifyMap_.clear();
    //    }
}

void ICHCProgramMonitorFrame::SelectCurrentStep(int currentStep)
{
    if((currentStep != 0  && currentStep < oldStep_ && isModify_) ||
            (oldStep_ == 0 && currentStep > oldStep_ && isModify_))
    {
        //        QMap<ICMoldItem*, ICMoldItem>::iterator p = modifyMap_.begin();
        //        while(p != modifyMap_.end())
        //        {
        //            *(p.key()) = p.value();
        //            ++p;
        //        }

        ICMold::CurrentMold()->SetMoldContent(ICMold::UIItemToMoldItem(programList_));
        ICMold::CurrentMold()->SaveMoldFile();
        programListBackup_ = programList_;
        qDebug("run modify");
        UpdateHostParam();
        isModify_ = false;
        //        modifyMap_.clear();
    }
    oldStep_ = currentStep;
    if(currentStep < 0 || currentStep >= programList_.size())
    {
        qDebug()<<"current step wrong"<<currentStep;
        return;
    }
    if(!isFollow_)
    {
        return;
    }
    ui->moldContentListWidget->clearSelection();
    ICGroupMoldUIItem* gItem = &programList_[currentStep];
    currentStepItem_ = gItem;
    //    int selectedCount = gItem->ItemCount();
    startIndex_ = 0;
    for(int i = 0; i != currentStep; ++i)
    {
        startIndex_ += programList_.at(i).ItemCount();
    }
    if(startIndex_ < ui->moldContentListWidget->count())
    {
        ui->moldContentListWidget->scrollToItem(ui->moldContentListWidget->item(startIndex_));
    }
    const int topItemCount = gItem->TopItemCount();
    int nextTopItemIndex = startIndex_;
    for(int i = 0; i != topItemCount; ++i)
    {
        if(nextTopItemIndex < ui->moldContentListWidget->count())
        {
            ui->moldContentListWidget->item(nextTopItemIndex)->setSelected(true);
        }
        if(gItem->at(i).SubItemCount() != 0)
        {
            nextTopItemIndex += gItem->at(i).ItemCount();
        }
        else
        {
            ++nextTopItemIndex;
        }
    }
}

void ICHCProgramMonitorFrame::SubStepChanged(uint8_t* subStep)
{
    if(!isFollow_)
    {
        return;
    }
    if(currentStepItem_ == NULL)
    {
        return;
    }
    const int topItemCount = currentStepItem_->TopItemCount();
    int currentSub = 0;
    int nextTopItemIndex = startIndex_ + 1;
    int size;
    for(int i = 0; i != topItemCount; ++i)
    {
        if(currentStepItem_->at(i).SubItemCount() != 0)
        {
            if(subStep[currentSub] != 0)
            {
                size = nextTopItemIndex + subStep[currentSub] - 1;
                if(size < ui->moldContentListWidget->count())
                {
                    ui->moldContentListWidget->item(nextTopItemIndex + subStep[currentSub] - 1)->setSelected(false);
                }
            }
            size = nextTopItemIndex + (subStep[currentSub]);
            if(size < ui->moldContentListWidget->count())
            {
                ui->moldContentListWidget->item(nextTopItemIndex + (subStep[currentSub++]))->setSelected(true);
            }
            nextTopItemIndex += currentStepItem_->at(i).ItemCount();
        }
        else
        {
            ++nextTopItemIndex;
        }
    }

}

void ICHCProgramMonitorFrame::UpdateHostParam()
{
    qDebug("m update");
    startIndex_ = 0;
    currentStepItem_ = NULL;
    if(currentMoldNum_ == 8)
    {
        programList_ = ICMold::CurrentMold()->ToUIItems();
    }
    else if(currentMoldNum_ >= 0 && currentMoldNum_ < 8)
    {
        programList_ = ICMold::MoldItemToUIItem(ICMacroSubroutine::Instance()->SubRoutine(currentMoldNum_));
    }
    else
    {
        programList_ = ICMold::CurrentMold()->ToUIItems();
        //        return;
    }
    qDebug("after m update");

    UpdateUIProgramList_();
}

void ICHCProgramMonitorFrame::InitSignal()
{
    //    connect(ICInstructParam::Instance(),
    //            SIGNAL(HostParamChanged()),
    //            this,
    //            SLOT(UpdateHostParam()));
    //    connect(autoRunRevise_,
    //            SIGNAL(MoldItemChanged()),
    //            this,
    //            SLOT(MoldItemChanged()));
}


void ICHCProgramMonitorFrame::on_editToolButton_clicked()
{
    const int selectedRow = ui->moldContentListWidget->currentRow();
    if(selectedRow < 0)
    {
        return;
    }
    int gIndex;
    int tIndex;
    int sIndex;
    FindIndex_(selectedRow, gIndex, tIndex, sIndex);
    if(sIndex < 0)
    {
        ICTopMoldUIItem * topItem = &programList_[gIndex].at(tIndex);
        ICTopMoldUIItem * topItemB = &programListBackup_[gIndex].at(tIndex);
        ICMoldItem* item = topItem->BaseItem();
        ICMoldItem * currentBackup;

        ICAutoAdjustCommand command;
        ICCommandProcessor* processor;
        ICMoldItem ret;
        bool isM = autoRunRevise_->ShowModifyItem(item, &ret, topItem->ToStringList().join("\n"));
        if(isM)
        {          
          if(isModify_)
            {
                currentBackup = programListBackup_[gIndex].at(tIndex).BaseItem();
                item->SetDVal(ret.DVal());
                item->SetSVal(ret.SVal());
                item->SetPos(currentBackup->Pos() + ret.Pos());
                item->ReSum();
                UpdateUIProgramList_();
                processor = ICCommandProcessor::Instance();
                command.SetSlave(processor->SlaveID());
                command.SetSequence(item->Seq());
                command.SetDelayTime(item->DVal());
                command.SetSpeed(item->SVal());
                command.SetDPos(ret.Pos());
                command.SetGMValue(item->GMVal());
                command.SetCheckSum(item->Sum());
            }
            else
            {
                currentBackup = programList_[gIndex].at(tIndex).BaseItem();
                currentBackup->SetDVal(ret.DVal());
                currentBackup->SetSVal(ret.SVal());
                currentBackup->SetPos(currentBackup->Pos() + ret.Pos());
                currentBackup->ReSum();
                UpdateUIProgramList_();
                processor = ICCommandProcessor::Instance();
                command.SetSlave(processor->SlaveID());
                command.SetSequence(currentBackup->Seq());
                command.SetDelayTime(currentBackup->DVal());
                command.SetSpeed(currentBackup->SVal());
                command.SetDPos(ret.Pos());
                command.SetGMValue(currentBackup->GMVal());
                command.SetCheckSum(currentBackup->Sum());
            }
#ifdef Q_WS_X11
          isM = true ;
#else
            isM = processor->ExecuteCommand(command).toBool();
#endif
            qDebug()<<"after show"<<isM;
            isModify_ = isModify_ || isM;
        }
    }
    else
    {
        ICSubMoldUIItem *subItem = &programList_[gIndex].at(tIndex).at(sIndex);
        ICSubMoldUIItem *subItemB = &programListBackup_[gIndex].at(tIndex).at(sIndex);
        ICMoldItem* item = subItem->BaseItem();
        ICMoldItem * currentBackup;
        ICMoldItem ret;
        ICAutoAdjustCommand command;
        ICCommandProcessor* processor;
        bool isM = autoRunRevise_->ShowModifyItem(subItem->BaseItem(), &ret, subItem->ToString());
        if(isM)
        {
            if(isModify_)
            {
               currentBackup = programListBackup_[gIndex].at(tIndex).BaseItem();
               item->SetDVal(ret.DVal());
               item->SetSVal(ret.SVal());
               item->SetPos(currentBackup->Pos() + ret.Pos());
               item->ReSum();
               UpdateUIProgramList_();
               processor = ICCommandProcessor::Instance();
               command.SetSlave(processor->SlaveID());
               command.SetSequence(item->Seq());
               command.SetDelayTime(item->DVal());
               command.SetSpeed(item->SVal());
               command.SetDPos(ret.Pos());
               command.SetGMValue(item->GMVal());
               command.SetCheckSum(item->Sum());
            }
            else
            {
               currentBackup = programList_[gIndex].at(tIndex).BaseItem();
               currentBackup->SetDVal(ret.DVal());
               currentBackup->SetSVal(ret.SVal());
               currentBackup->SetPos(currentBackup->Pos() + ret.Pos());
               currentBackup->ReSum();
               UpdateUIProgramList_();
               processor = ICCommandProcessor::Instance();
               command.SetSlave(processor->SlaveID());
               command.SetSequence(currentBackup->Seq());
               command.SetDelayTime(currentBackup->DVal());
               command.SetSpeed(currentBackup->SVal());
               command.SetDPos(ret.Pos());
               command.SetGMValue(currentBackup->GMVal());
               command.SetCheckSum(currentBackup->Sum());
            }
            isM = processor->ExecuteCommand(command).toBool();
            qDebug()<<"after show"<<isM;
            isModify_ = isModify_ || isM;

        }
    }
    qDebug()<<"Modify"<<isModify_;

}

void ICHCProgramMonitorFrame::MoldItemChanged()
{
    //    ICMold::Modify(QList<ICMoldItem>()<<item, moldContent_);

    isModify_ = true;
}

void ICHCProgramMonitorFrame::UpdateUIProgramList_()
{
    ui->moldContentListWidget->clear();
    qDebug("after clear");

    ui->moldContentListWidget->addItems(ICMold::UIItemsToStringList(programList_));
    ICGroupMoldUIItem groupItem;
    int topItemRowCount;
    int index = 0;
    QColor color;
    ICMoldItem* tmp = NULL;
    for(int i = 0; i != programList_.size(); ++i)
    {
        (i % 2 == 0 ? color.setRgb(255, 255, 154):color.setRgb(154, 255, 255));
        groupItem = programList_.at(i);
        topItemRowCount = groupItem.ItemCount();
        for(int j = 0; j != topItemRowCount; ++j)
        {
            tmp = groupItem.MoldItemAt(j);
            if(tmp != NULL)
            {
                if(tmp->Num() == 0)
                {
                    ui->moldContentListWidget->item(j + index)->setBackgroundColor(QColor(239, 235, 231));
                    //                    ui->moldContentListWidget->item(j + index)->setForeground(QColor("white"));
                }
                else if(tmp->Action() == ICInstructParam::ACT_WaitMoldOpened)
                {
                    ui->moldContentListWidget->item(j + index)->setBackgroundColor("red");
                }
                else
                {
                    ui->moldContentListWidget->item(j + index)->setBackgroundColor(color);
                }
            }
        }
        index += topItemRowCount;
    }
    qDebug("updatge end");
}

void ICHCProgramMonitorFrame::on_singleStepButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_F6);
}

void ICHCProgramMonitorFrame::FindIndex_(int currentIndex, int& groupItemIndex, int &topItemIndex, int &subItemIndex)
{
    for(int i = 0; i != programList_.size(); ++i)
    {
        if(currentIndex < programList_.at(i).ItemCount())
        {
            groupItemIndex = i;
            for(int j = 0; j != programList_.at(i).TopItemCount(); ++j)
            {
                if(currentIndex < programList_.at(i).at(j).ItemCount())
                {
                    topItemIndex = j;
                    if(currentIndex == 0)
                    {
                        subItemIndex = -1;
                    }
                    else
                    {
                        subItemIndex = currentIndex - 1;
                    }
                    break;
                }
                else
                {
                    currentIndex -= programList_.at(i).at(j).ItemCount();
                }
            }
            break;
        }
        else
        {
            currentIndex -= programList_.at(i).ItemCount();
        }
    }
}

void ICHCProgramMonitorFrame::MoldNumChanged(int mold)
{
    this->blockSignals(true);
    currentMoldNum_ = mold;
    UpdateHostParam();
    this->blockSignals(false);
}

void ICHCProgramMonitorFrame::on_followToolButton_clicked()
{
    if(isFollow_)
    {
        isFollow_ = false;
        ui->followToolButton->setIcon(QPixmap(":/resource/play.png"));
        ui->followToolButton->setText(tr("Follow"));
    }
    else
    {
        isFollow_ = true;
        ui->followToolButton->setIcon(QPixmap(":/resource/stop.png"));
        ui->followToolButton->setText(tr("No Follow"));
    }
}

void ICHCProgramMonitorFrame::on_speedEnableButton_clicked()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    host->SetSpeedEnable(!host->IsSpeedEnable());
    ui->speedEnableButton->setIcon(host->IsSpeedEnable()? switchOn_ : switchOff_);
    ui->speedEnableButton->setText(host->IsSpeedEnable() ? tr("Speed Enable") : tr("Speed Disable"));
}

void ICHCProgramMonitorFrame::OnTimeOut()
{
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    if(!host->HasTuneSpeed())
    {
        host->SetSpeedEnable(false);
        ui->speedEnableButton->setIcon(switchOff_);
        ui->speedEnableButton->setText(tr("Speed Disable"));
    }
    host->SetTuneSpeed(false);
}
