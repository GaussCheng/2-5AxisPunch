#include "ichcinstructionpageframe.h"
#include "ui_ichcinstructionpageframe.h"

#include <QDir>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileInfoList>

#include "icmold.h"
#include "actionsettingframe.h"
#include "ichcfixturepage.h"
#include "ichcinjectionpage.h"
#include "ichcconditionpage.h"
#include "icflagseditor.h"
#include "icpneumaticactionpage.h"
#include "icwaitconditioneditor.h"
#include "icperipherypage.h"
#include "iccutpage.h"
#include "icprogramselector.h"
#include "icstackeditor.h"
#include "icparameterssave.h"
#include "icinstructparam.h"
#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"
#include "moldinformation.h"
#include "icactioncommand.h"
#include "icmacrosubroutine.h"
#include "icprogramheadframe.h"
#include "icinstructioneditorbase.h"
#include "mainframe.h"
#include "ickeyboard.h"
#include "icactioncommand.h"
#include "ichcotherpage.h"
#include "ichcstackedsettingsframe.h"
#include "icwaitmeditor.h"

#include <QDebug>
#include <QMessageBox>
#include <QCheckBox>
//#include <QMenu>

//struct MoldItem
//{
//    int stepNum;
//    int
//};

int ReserveTextToIndex(const QString& text)
{
    if(!text.right(1).at(0).isDigit())
        return 8;

    return text.right(1).toInt();
}

static QList<QCheckBox*> fixtureCheckList;

ICHCInstructionPageFrame::ICHCInstructionPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCInstructionPageFrame),
    injectionPage_(NULL),
    fixturePage_(NULL),
    pneumaticPage_(NULL),
    waitConditionPage_(NULL),
    peripheryPage_(NULL),
    cutPage_(NULL),
    mPage_(NULL),
    statckPage_(NULL),
    recordPath_("./records/"),
    currentAction_(None),
    currentEdit_(0)
{
    ui->setupUi(this);

//    ui->tabWidget->addTab(MoldInformation::Instance(), tr("Records"));
//    ui->tabWidget->addTab(new ICHCStackedSettingsFrame(), tr("Stack"));
    fixtureCheckList<<ui->check0<<ui->check1<<ui->check2<<ui->check3<<ui->check4
                   <<ui->check5<<ui->check6<<ui->check7<<ui->check8
                  <<ui->check9<<ui->check10<<ui->check11<<ui->check12
                 <<ui->check13<<ui->check14<<ui->check15<<ui->check16
                <<ui->check17<<ui->check18<<ui->check19<<ui->check20
               <<ui->check21<<ui->check22<<ui->check23<<ui->check24
              <<ui->check25<<ui->check26<<ui->check27<<ui->check28
             <<ui->check29<<ui->check30<<ui->check31<<ui->check32
            <<ui->check33<<ui->check34<<ui->check35<<ui->check36
           <<ui->check37<<ui->check38<<ui->check39<<ui->check40
          <<ui->check41<<ui->check42<<ui->check43<<ui->check44
         <<ui->check45<<ui->check46<<ui->check47<<ui->check48
        <<ui->check49<<ui->check50<<ui->check51<<ui->check52
       <<ui->check53<<ui->check54<<ui->check55<<ui->check56
      <<ui->check57<<ui->check58<<ui->check59<<ui->check60
     <<ui->check61<<ui->check62<<ui->check63;
    //    ui->otherButton->hide();

    ui->pneumaticButton->hide();
    InitInterface();
    InitSignal();
//    ui->tabWidget
    //    ui->moldContentListWidget->setBackgroundRole(QColor("gray"));

    //    LoadAllRecordFileInfo();

    InitParameter();
    const int fs = fixtureCheckList.size();
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserIOInfo info;
    for(int i = 0; i != fs; ++i)
    {
        info = config->XInfo(i);
        fixtureCheckList[i]->setText(info.GetLocaleName("zh") + tr("Check"));
    }

    connect(MoldInformation::Instance(),
            SIGNAL(MoldChanged(QString)),
            SLOT(OnMoldChanged(QString)));
    ui->stackButton->hide();
    nullButton = new QPushButton();
    nullButton->setCheckable(true);
    ui->buttonGroup->addButton(nullButton);

    oriStyle = ui->reserveBox->styleSheet();
    selStyle = "border: 2px solid gray;"
            "border-radius: 6px;"
            "padding: 1px 18px 1px 3px;"
           "background-color:  rgba(62, 113, 255, 255);";
}

ICHCInstructionPageFrame::~ICHCInstructionPageFrame()
{
    delete modifyDialog_;
    delete nullButton;
    delete ui;
}

static bool isShow = false;

void ICHCInstructionPageFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    UpdateHostParam();
    isShow = true;
    ReserveProgConfig progConfig;
    progConfig.all =  ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv2).toInt();
//    ui->feedButton->setVisible(progConfig.b.r8);
    ui->reserveBox->blockSignals(true);
    ui->reserveBox->clear();
    QStringList items;
    if(progConfig.b.r1)
        items.append(tr("Reserve1"));
    if(progConfig.b.r2)
        items.append(tr("Reserve2"));
    if(progConfig.b.r3)
        items.append(tr("Reserve3"));
    if(progConfig.b.r4)
        items.append(tr("Reserve4"));
    if(progConfig.b.r5)
        items.append(tr("Reserve5"));
    if(progConfig.b.r6)
        items.append(tr("Reserve6"));
    if(progConfig.b.r7)
        items.append(tr("Reserve7"));
//    if(progConfig.b.r8)
//        items.append(tr("Feed"));
    if(!progConfig.b.r8 && items.isEmpty())
    {
        nullButton->click();
        ui->mainButton->setEnabled(false);
    }
    else
    {
        ui->mainButton->setEnabled(true);
    }

    ui->reserveBox->setVisible(!items.isEmpty());
    ui->fixtureCheckBtn->setVisible(progConfig.b.r8);
    ui->feedButton->setVisible(progConfig.b.r8);

    ui->reserveBox->addItems(items);
    ui->reserveBox->blockSignals(false);
//    ui->rP1->setCurrentIndex(progConfig.b.r1);
//    ui->rP2->setCurrentIndex(progConfig.b.r2);
//    ui->rP3->setCurrentIndex(progConfig.b.r3);
//    ui->rP4->setCurrentIndex(progConfig.b.r4);
//    ui->rP5->setCurrentIndex(progConfig.b.r5);
//    ui->rP6->setCurrentIndex(progConfig.b.r6);
//    ui->rP7->setCurrentIndex(progConfig.b.r7);
//    ui->rP8->setCurrentIndex(progConfig.b.r8);

}

void ICHCInstructionPageFrame::hideEvent(QHideEvent *e)
{
    if(!isShow)
    {
        QFrame::hideEvent(e);
        return;
    }
    isShow = false;
    qDebug("instruct hide");
    modifyDialog_->hide();
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int c4 = 0;
    for(int i = 0; i != 16; ++i)
    {
        if(fixtureCheckList.at(i)->isChecked())
        {
            c1 |= (1 << i);
        }
    }
    for(int i = 16; i != 32; ++i)
    {
        if(fixtureCheckList.at(i)->isChecked())
        {
            c2 |= (1 << (i - 16));
        }
    }
    for(int i = 32; i != 48; ++i)
    {
        if(fixtureCheckList.at(i)->isChecked())
        {
            c3 |= (1 << (i - 32));
        }
    }
    for(int i = 48; i != 64; ++i)
    {
        if(fixtureCheckList.at(i)->isChecked())
        {
            c4 |= (1 << (i - 48));
        }
    }

    ICMold* cm = ICMold::CurrentMold();
    if(cm == NULL) return;

    bool isCmChanged = false;

    if(cm->MoldParam(ICMold::check1) != c1)
    {
        cm->SetMoldParam(ICMold::check1, c1);
        isCmChanged = true;
    }
    if(cm->MoldParam(ICMold::check2) != c2)
    {
        cm->SetMoldParam(ICMold::check2, c2);
        isCmChanged = true;
    }

    if(cm->MoldParam(ICMold::check3) != c3)
    {
        cm->SetMoldParam(ICMold::check3, c3);
        isCmChanged = true;
    }
    if(cm->MoldParam(ICMold::check4) != c4)
    {
        cm->SetMoldParam(ICMold::check4, c4);
        isCmChanged = true;
    }
    if(SaveCurrentEdit() == true)
    {
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    if(isCmChanged)
    {
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    if(ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_ManualStatu)
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
    }
    actionPage_->ResetServo();
    QFrame::hideEvent(e);

    //   ICInstructParam::Instance()->UpdateHostParam();

}

void ICHCInstructionPageFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        ICInstructParam::Instance()->UpdateTranslate();
        const int fs = fixtureCheckList.size();
        ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
        ICUserIOInfo info;
        for(int i = 0; i != fs; ++i)
        {
            info = config->YInfo(i);
            fixtureCheckList[i]->setText(info.GetLocaleName("zh") + tr("Check"));
        }
    }
        break;
    default:
        break;
    }
}

void ICHCInstructionPageFrame::OptionButtonClicked()
{
    QAbstractButton * optionButton = qobject_cast<QAbstractButton *>(sender());

    if(fixturePage_ == NULL && optionButton == ui->fixtureButton)
    {
        fixturePage_ = new ICHCFixturePage;
        optionButtonToPage_.insert(ui->fixtureButton, fixturePage_);
        ui->settingStackedWidget->addWidget(fixturePage_);
    }
    else if(pneumaticPage_ == NULL && optionButton == ui->pneumaticButton)
    {
        pneumaticPage_ = new ICPneumaticActionPage();
        optionButtonToPage_.insert(ui->pneumaticButton, pneumaticPage_);
        ui->settingStackedWidget->addWidget(pneumaticPage_);
    }
    else if(peripheryPage_ == NULL && optionButton == ui->peripheryButton)
    {
        peripheryPage_ = new ICPeripheryPage();
        optionButtonToPage_.insert(ui->peripheryButton, peripheryPage_);
        ui->settingStackedWidget->addWidget(peripheryPage_);
    }
    else if(cutPage_ == NULL && optionButton == ui->cutButton)
    {
        cutPage_ = new ICCutPage();
        optionButtonToPage_.insert(ui->cutButton, cutPage_);
        ui->settingStackedWidget->addWidget(cutPage_);
    }
    else if(waitConditionPage_ == NULL && ui->workMachineIOButton == optionButton)
    {
        waitConditionPage_ = new ICWaitConditionEditor();
        optionButtonToPage_.insert(ui->workMachineIOButton, waitConditionPage_);
        ui->settingStackedWidget->addWidget(waitConditionPage_);
    }
    else if(mPage_ == NULL && ui->waitMButton == optionButton)
    {
        mPage_ = new ICWaitMEditor();
        optionButtonToPage_.insert(ui->waitMButton, mPage_);
        ui->settingStackedWidget->addWidget(mPage_);
    }
#ifdef HC_SK_8
    else if(statckPage_ == NULL && ui->stackButton == optionButton)
    {
        statckPage_ = new ICStackEditor();
        optionButtonToPage_.insert(ui->stackButton, statckPage_);
        ui->settingStackedWidget->addWidget(statckPage_);
    }
#endif
    ui->settingStackedWidget->setCurrentWidget(optionButtonToPage_.value(optionButton));
}

void ICHCInstructionPageFrame::InitInterface()
{
    //    ui->settingFrame->setLayout(ui->settingStackedWidget);

    actionPage_ = new ActionSettingFrame(ui->actionSelectPage);
    optionButtonToPage_.insert(ui->lineButton, actionPage_);
    ui->settingStackedWidget->addWidget(actionPage_);
    modifyDialog_ = new ICInstructModifyDialog(this);
    int c1 = ICMold::CurrentMold()->MoldParam(ICMold::check1);
    int c2 = ICMold::CurrentMold()->MoldParam(ICMold::check2);
    int c3 = ICMold::CurrentMold()->MoldParam(ICMold::check3);
    int c4 = ICMold::CurrentMold()->MoldParam(ICMold::check4);
    for(int i = 0; i != 16; ++i)
    {
        if(c1 & (1 <<i))
        {
            fixtureCheckList[i]->setChecked(true);
        }
    }
    for(int i = 16; i != 32; ++i)
    {
        if(c2 & (1 <<(i - 16)))
        {
            fixtureCheckList[i]->setChecked(true);
        }
    }
    for(int i = 32; i != 48; ++i)
    {
        if(c3 & (1 <<(i - 32)))
        {
            fixtureCheckList[i]->setChecked(true);
        }
    }
    for(int i = 48; i != 64; ++i)
    {
        if(c4 & (1 <<(i - 48)))
        {
            fixtureCheckList[i]->setChecked(true);
        }
    }

    //    injectionPage_ = new ICHCInjectionPage;
    //    optionButtonToPage_[ui->injectionButton] = injectionPage_;
    //    ui->settingStackedWidget->addWidget(injectionPage_);

    //    fixturePage_ = new ICHCFixturePage;
    //    optionButtonToPage_[ui->fixtureButton] = fixturePage_;
    //    ui->settingStackedWidget->addWidget(fixturePage_);

    //    conditionPage_ = new ICHCConditionPage;
    //    optionButtonToPage_[ui->conditionsToolButton] = conditionPage_;
    //    ui->settingStackedWidget->addWidget(conditionPage_);

    //    ui->settingStackedWidget->setCurrentWidget(actionPage_);
}

void ICHCInstructionPageFrame::InitSignal()
{
    connect(ui->lineButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->fixtureButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));

    connect(ui->pneumaticButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->peripheryButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->cutButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->workMachineIOButton,
            SIGNAL(clicked()),
            SLOT(OptionButtonClicked()));
    connect(ui->waitMButton,
            SIGNAL(clicked()),
            SLOT(OptionButtonClicked()));
#ifdef HC_SK_8
    connect(ui->stackButton,
            SIGNAL(clicked()),
            SLOT(OptionButtonClicked()));
#endif
}

void ICHCInstructionPageFrame::InitParameter()
{
    QString recordName = ICParametersSave::Instance()->MoldName("Default.act");

    //    ui->programSelectedComboBox->setCurrentIndex(ui->programSelectedComboBox->findText(recordName));

    //    ICInstructParam::Instance()->UpdateHostParam();
    UpdateHostParam();
    //    LoadMoldFile(recordName);
}

void ICHCInstructionPageFrame::UpdateHostParam()
{
    qDebug("update");
    qDebug()<<"Update"<<currentEdit_;
    qDebug()<<"UpdateMold size"<<ICMold::CurrentMold()->MoldContent().size();
    if(currentEdit_ == 0)
    {
        programList_ = ICMold::CurrentMold()->ToUIItems();
    }
    else
    {
        programList_ = ICMold::MoldItemToUIItem(ICMacroSubroutine::Instance()->SubRoutine(currentEdit_ - 1));
    }
    qDebug("after update");
    UpdateUIProgramList_();
}

void ICHCInstructionPageFrame::UpdateUIProgramList_()
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
        (i % 2 == 0 ? color.setRgb(255,255,154): color.setRgb(154,255,255));
        //        (i % 2 == 0 ? color.setRgb(191,255,191): color.setRgb(222,255,222));
        //        (i % 2 == 0 ? color.setRgb(255,255,154): color.setRgb(191,255,191));
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
                //                else if(tmp->Action() == ICMold::ACT_WaitMoldOpened)
                //                {
                //                    ui->moldContentListWidget->item(j + index)->setBackgroundColor("red");
                //                }
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

void ICHCInstructionPageFrame::on_insertToolButton_clicked()
{
    ICInstructionEditorBase* editor = qobject_cast<ICInstructionEditorBase*>(ui->settingStackedWidget->currentWidget());
    ICFlagsEditor *flagsEditor = qobject_cast<ICFlagsEditor*> (editor);
    ActionSettingFrame *servoEditor = qobject_cast<ActionSettingFrame*>(editor);
    if(editor == NULL)
    {
        return;
    }
    int index = ui->moldContentListWidget->currentRow();
    int gIndex;
    int tIndex;
    int sIndex;
    if(index < 0)
    {
        return;
    }
    FindIndex_(index, gIndex, tIndex, sIndex);
    QString currentmoldname  = ICParametersSave::Instance()->MoldName("");
    /******currentEdit_标记为0表示为主程序（主程序时不能删除待机点）*****/
    //    if(programList_.at(gIndex).StepNum() == 0 && currentEdit_ == 0 && currentmoldname.left(4) != "szhc")
    //    {
    //        QMessageBox::warning(this,
    //                             tr("Warning"),
    //                             tr("Can not add standby position program"));
    //        return;
    //    }
    bool isParallel = false;
    bool isServo = false;
    if(flagsEditor != NULL)
    {
        isParallel = true;
    }
    if(servoEditor != NULL)
    {
        isServo = true;
    }
    QList<ICMoldItem> items = editor->CreateCommand();
    if(items.isEmpty() && !isParallel)
    {
        return;
    }
    if(sIndex == -1)
    {
        if(tIndex == 0) //insert GroupItem
        {
            QList<ICGroupMoldUIItem> insertedGroupItems;
            if(isParallel)
            {
                ICGroupMoldUIItem groupItem;
                ICTopMoldUIItem topItem = flagsEditor->CreateTopUIItem();
                groupItem.AddToMoldUIItem(topItem);
                groupItem.SetStepNum(gIndex);
                insertedGroupItems.append(groupItem);
            }
            else if(isServo)
            {
                ICTopMoldUIItem topItem;
                ICGroupMoldUIItem groupItem;
                if(items.at(0).Action() == ICMold::GARC)
                {
                    for(int i = 0; i != items.size(); ++i)
                    {
                        topItem.SetBaseItem(items.at(i));
                        groupItem.AddToMoldUIItem(topItem);
                    }
                    groupItem.SetStepNum(gIndex);
                    insertedGroupItems.append(groupItem);
                }
                else
                {
                    for(int i = 0; i != items.size(); ++i)
                    {
                        topItem.SetBaseItem(items.at(i));
                        ICGroupMoldUIItem groupItem;
                        groupItem.AddToMoldUIItem(topItem);
                        groupItem.SetStepNum(gIndex + i);
                        insertedGroupItems.append(groupItem);
                    }
                }
            }
            else
            {
                ICTopMoldUIItem topItem;
                for(int i = 0; i != items.size(); ++i)
                {
                    topItem.SetBaseItem(items.at(i));
                    ICGroupMoldUIItem groupItem;
                    groupItem.AddToMoldUIItem(topItem);
                    groupItem.SetStepNum(gIndex + i);
                    insertedGroupItems.append(groupItem);
                }
            }
            const int addedCount = insertedGroupItems.size();
            for(int i = gIndex; i != programList_.size(); ++i)
            {
                programList_[i].SetStepNum(i + addedCount);
            }
            for(int i = 0; i != insertedGroupItems.size(); ++i)
            {
                programList_.insert(gIndex++, insertedGroupItems.at(i));
            }
        }
        else  //insert TopItem
        {
            QList<ICTopMoldUIItem> insertedTopItems;
            ICTopMoldUIItem topItem;
            const int stepNum = programList_.at(gIndex).StepNum();
            if(isParallel)
            {
                topItem = flagsEditor->CreateTopUIItem();
                topItem.SetStepNum(stepNum);
                insertedTopItems.append(topItem);
            }
            else
            {
                for(int i = 0; i != items.size(); ++i)
                {
                    topItem.SetBaseItem(items.at(i));
                    topItem.SetStepNum(stepNum);
                    insertedTopItems.append(topItem);
                }
            }
            for(int i = 0; i != insertedTopItems.size(); ++i)
            {
                programList_[gIndex].InsertTopItem(tIndex++, insertedTopItems.at(i));
            }
        }
    }
    else //insert SubItem
    {
        if(isParallel)
        {
            return;
        }
        QList<ICSubMoldUIItem> insertedSubItems;
        ICSubMoldUIItem item;
        const int stepNum = programList_.at(gIndex).StepNum();
        const int subNum = programList_.at(gIndex).at(tIndex).at(sIndex).SubNum();
        bool isSyncItem;
        if(sIndex == 0)
        {
            isSyncItem = false;
        }
        else
        {
            isSyncItem = programList_.at(gIndex).at(tIndex).at(sIndex).SubNum() ==
                    programList_.at(gIndex).at(tIndex).at(sIndex - 1).SubNum();
        }
        for(int i = 0; i != items.size(); ++i)
        {
            item.SetBaseItem(items.at(i));
            item.SetStepNum(stepNum);
            item.SetSyncItem(isSyncItem);
            isSyncItem ? item.SetSubNum(subNum):item.SetSubNum(subNum + i);
            insertedSubItems.append(item);
        }
        ICTopMoldUIItem* topItem = &programList_[gIndex].at(tIndex);
        for(int i = 0; i != insertedSubItems.size(); ++i)
        {
            topItem->InsertSubMoldUIItem(sIndex++, insertedSubItems.at(i));
        }
        if(!isSyncItem)
        {
            int addCount = insertedSubItems.size();
            for(int i = sIndex; i != topItem->SubItemCount(); ++i)
            {
                topItem->at(i).SetSubNum(topItem->at(i).SubNum() + addCount);
            }
        }

        //        topItem->ReCalSubNum();
    }
    UpdateUIProgramList_();
    index += items.size();
    if(index >= ui->moldContentListWidget->count())
    {
        index = ui->moldContentListWidget->count() - 1;
    }
    ui->moldContentListWidget->setCurrentRow(index);
}

void ICHCInstructionPageFrame::on_modifyToolButton_clicked()
{
    QList<QListWidgetItem*> items = ui->moldContentListWidget->selectedItems();
    if(items.isEmpty())
    {
        return;
    }
    const int selectedRow = ui->moldContentListWidget->row(items.at(0));
    int gIndex;
    int tIndex;
    int sIndex;
    FindIndex_(selectedRow, gIndex, tIndex, sIndex);
    if(sIndex < 0)
    {
        ICTopMoldUIItem * topItem = &programList_[gIndex].at(tIndex);
        ICMoldItem* item = topItem->BaseItem();
        bool isModify = modifyDialog_->ShowModifyItem(item);
        if(isModify)
        {
            ui->moldContentListWidget->item(selectedRow)->setText(topItem->ToStringList().at(0));
        }
    }
    else
    {
        ICSubMoldUIItem *subItem = &programList_[gIndex].at(tIndex).at(sIndex);
        bool isModify = modifyDialog_->ShowModifyItem(subItem->BaseItem());
        if(isModify)
        {
            ui->moldContentListWidget->item(selectedRow)->setText(subItem->ToString());
        }

    }

    //    if(isModify)

}

void ICHCInstructionPageFrame::on_deleteToolButton_clicked()
{
    int currentRow = ui->moldContentListWidget->currentRow();
    if(currentRow == ui->moldContentListWidget->count() - 1)
    {
        return;
    }
    if(currentRow < 0)
    {
        return;
    }
    if(MoldInformation::Instance()->IsStandProgram(ICParametersSave::Instance()->MoldName("")))
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("Stand program can not be delete action"));
        return;
    }
    int gIndex;
    int tIndex;
    int sIndex;
    FindIndex_(currentRow, gIndex, tIndex, sIndex);
    /************BUG181:子程序位置插入后不能删除******************/

    QString currentmoldname  = ICParametersSave::Instance()->MoldName("");
    //    if(programList_.at(gIndex).StepNum() == 0 && currentEdit_ == 0 && currentmoldname.left(4) != "szhc")
    //    {
    //        QMessageBox::warning(this,
    //                             tr("Warning"),
    //                             tr("Can not delete standby position program"));
    //        return;
    //    }
    if(sIndex == -1)
    {
        if(programList_.at(gIndex).TopItemCount() == 1) //delete Group Item
        {
            programList_.removeAt(gIndex);
            for(int i = gIndex; i != programList_.size(); ++i)
            {
                programList_[i].SetStepNum(i);
            }
        }
        else
        {
            programList_[gIndex].RemoveTopItem(tIndex);
        }
    }
    else
    {
        ICTopMoldUIItem* topItem = &programList_[gIndex].at(tIndex);
        bool isSyncItem = topItem->IsSyncSubItem(sIndex);
        //        if(topItem->SubItemCount() == 1)
        //        {
        //            isSyncItem = false;
        //        }
        //        else if(sIndex == 0)
        //        {
        //            isSyncItem = topItem->at(sIndex + 1).SubNum() == topItem->at(sIndex).SubNum();
        //        }
        //        else if(sIndex == topItem->SubItemCount() - 1)
        //        {
        //            isSyncItem = topItem->at(sIndex - 1).SubNum() == topItem->at(sIndex).SubNum();
        //        }
        //        else
        //        {
        //            isSyncItem = (topItem->at(sIndex + 1).SubNum() == topItem->at(sIndex).SubNum()) ||
        //                    topItem->at(sIndex - 1).SubNum() == topItem->at(sIndex).SubNum();
        //        }
        topItem->RemoveSubItem(sIndex);
        if(topItem->ItemCount() == 1)
        {
            programList_[gIndex].RemoveTopItem(tIndex);
        }
        else
        {
            if(!isSyncItem)
            {
                for(int i = sIndex; i != topItem->SubItemCount(); ++i)
                {
                    topItem->at(i).SubNumMinus();
                }
            }
        }
    }
    UpdateUIProgramList_();

    if(currentRow >= ui->moldContentListWidget->count())
    {
        currentRow = ui->moldContentListWidget->count() - 1;
    }
    ui->moldContentListWidget->setCurrentRow(currentRow);
}

//void ICHCInstructionPageFrame::LoadFileInfoButtonClicked(const QString &moldName)
//{
//    if(ICParametersSave::Instance()->MoldName(QString()) != moldName)
//    {
//        on_programSelectedComboBox_activated(moldName);
//    }
//}

//void ICHCInstructionPageFrame::NewFileCreated(const QString & moldName)
//{
//    ui->programSelectedComboBox->addItem(moldName);
//}

//void ICHCInstructionPageFrame::DeleteFile(const QString & moldName)
//{
//    ui->programSelectedComboBox->removeItem(
//                ui->programSelectedComboBox->findText(moldName));
//}

void ICHCInstructionPageFrame::on_commandButton_clicked()
{
    ui->settingStackedWidget->setCurrentIndex(0);
}

void ICHCInstructionPageFrame::FindIndex_(int currentIndex, int& groupItemIndex, int &topItemIndex, int &subItemIndex)
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
                    //                    if(currentIndex == 0)
                    //                    {
                    subItemIndex = -1;
                    //                    }
                    //                    else
                    //                    {
                    subItemIndex = currentIndex - 1;
                    //                    }
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

void ICHCInstructionPageFrame::on_upButton_clicked()
{
    int gIndex;
    int tIndex;
    int sIndex;
    const int currentRow = ui->moldContentListWidget->currentRow();
    if(currentRow < 0 || currentRow >= ui->moldContentListWidget->count() - 1)
    {
        return;
    }
    FindIndex_(currentRow, gIndex, tIndex, sIndex);
    //子程序也可以分解和组合，所以无需判断
    //    if(programList_.at(gIndex).StepNum() == 1)
    //    {
    //        QMessageBox::warning(this,
    //                             tr("Warning"),
    //                             tr("Can not group up to standby position program"));
    //        return;
    //    }
    if(sIndex == -1)
    {
        if(gIndex == 0)
        {
            return;
        }
        if(gIndex == programList_.size() - 1)
        {
            return;
        }
        ICGroupMoldUIItem *item = &programList_[gIndex];
        for(int i = 0; i != item->ItemCount(); ++i)
        {
            if(item->MoldItemAt(i)->Action() <= 8)
            {
                return;
            }
        }
        if(item->TopItemCount() == 1) //group up
        {
            item->SetStepNum(gIndex - 1);
            programList_[gIndex - 1].AddToMoldUIItem(item->at(0));
            programList_.removeAt(gIndex);
            for(int i = gIndex; i != programList_.size(); ++i)
            {
                programList_[i].SetStepNum(i);
            }
        }
        else if(tIndex == 0)
        {
            programList_[gIndex - 1].AddToMoldUIItem(item->at(0));
            programList_[gIndex - 1].SetStepNum(gIndex - 1);
            programList_[gIndex].RemoveTopItem(tIndex);
        }
    }
    else
    {
        if(programList_[gIndex].at(tIndex).at(sIndex).SubNum() == 0)
        {
            return;
        }

        ICTopMoldUIItem *topItem = &programList_[gIndex].at(tIndex);
        ICSubMoldUIItem *subItem = &topItem->at(sIndex);
        int subNum = subItem->SubNum();
        if(subNum != topItem->at(sIndex - 1).SubNum())
        {
            bool isSync = topItem->IsSyncSubItem(sIndex);
            topItem->at(sIndex - 1).SetSyncItem(true);
            subItem->SubNumMinus();
            subItem->SetSyncItem(true);
            if(!isSync)
            {
                for(int i = sIndex + 1; i != topItem->SubItemCount(); ++i)
                {
                    topItem->at(i).SubNumMinus();
                }
            }
        }
    }
    UpdateUIProgramList_();
}

void ICHCInstructionPageFrame::on_downButton_clicked()
{
    int gIndex;
    int tIndex;
    int sIndex;
    const int currentRow = ui->moldContentListWidget->currentRow();
    if(currentRow < 0)
    {
        return;
    }
    FindIndex_(currentRow, gIndex, tIndex, sIndex);
    //子程序也可以分解和组合，所以无需判断
    //    if(programList_.at(gIndex).StepNum() == 0)
    //    {
    //        QMessageBox::warning(this,
    //                             tr("Warning"),
    //                             tr("Can not decompose standby position program"));
    //        return;
    //    }
    if(sIndex == -1)
    {
        if(gIndex == 0 && tIndex == 0)// if is the first TopItem, do nothing
        {
            return;
        }
        if(gIndex >= programList_.size() - 1) //if is the last two item, do nothing
        {
            return;
        }
        if(tIndex == 0)
        {
            return;
        }
        ICGroupMoldUIItem* groupItem = &programList_[gIndex];
        if(groupItem->ItemCount() == 1) //group item down
        {
            groupItem->AddOtherGroup(programList_.at(gIndex + 1));
            programList_.removeAt(gIndex + 1);
            for(int i = gIndex; i != programList_.size(); ++i)
            {
                programList_[i].SetStepNum(i);
            }
        }
        else //split group item
        {
            QList<ICGroupMoldUIItem> gItems = programList_.at(gIndex).SpliteToTwoGroup(tIndex);
            gItems[0].SetStepNum(gIndex);
            gItems[1].SetStepNum(gIndex + 1);
            programList_.removeAt(gIndex);
            programList_.insert(gIndex++, gItems.at(0));
            programList_.insert(gIndex++, gItems.at(1));
            for(int i = gIndex; i != programList_.size(); ++i)
            {
                programList_[i].SetStepNum(i);
            }
        }

    }
    else //sub item
    {
        //        if(sIndex == 0) //sub item1 do nothing
        //        {
        //            return;
        //        }
        //        ICTopMoldUIItem *topItem = &programList_[gIndex].at(tIndex);
        //        ICSubMoldUIItem *subItem = topItem->at(sIndex);
        //        if(sIndex == 0 ||
        //           sIndex == topItem->SubItemCount() - 1) //it's not work when select the oth or last subItem
        //        {
        //            return;
        //        }
    }
    UpdateUIProgramList_();
}

void ICHCInstructionPageFrame::OnProgramChanged(int index, QString name)
{
    if(index < 0 || index > 8)
    {
        return;
    }
    SaveCurrentEdit();
    ui->moldContentListWidget->clear();
    programList_.clear();
    currentEdit_ = index;
    UpdateHostParam();

}

bool ICHCInstructionPageFrame::SaveCurrentEdit()
{
    if(currentEdit_ == 0)
    {
        ICMold::CurrentMold()->SetMoldContent(ICMold::UIItemToMoldItem(programList_));
        return ICMold::CurrentMold()->SaveMoldFile();
    }
    else
    {
        ICMacroSubroutine::Instance()->SetSubRoutine(ICMold::UIItemToMoldItem(programList_), currentEdit_ - 1);
        bool ret =  ICMacroSubroutine::Instance()->SaveMacroSubroutieFile(currentEdit_ - 1);
        if(currentEdit_ == 8)
        {
            QString moldName = ICParametersSave::Instance()->MoldName("Base.act");
            moldName.chop(3);
            moldName += "sub";
            moldName = QString("records/%1").arg(moldName);
            QFile::remove(moldName);
            ret = QFile::copy("subs/sub7.prg", moldName);
        }
        if(currentEdit_ < 8 )
        {
            QString moldName = ICParametersSave::Instance()->MoldName("Base.act");
            moldName.chop(3);
            moldName += "reserve";
            moldName = QString("records/%1%2").arg(moldName).arg(currentEdit_);
            QFile::remove(moldName);
            ret = QFile::copy(QString("subs/sub%1.prg").arg(currentEdit_ - 1), moldName);
        }
        return ret;
    }
}

void ICHCInstructionPageFrame::OnActionButtonReleased()
{
    ICKeyboard::Instace()->SetPressed(false);
}

void ICHCInstructionPageFrame::ShowServoAction(int key)
{
    if(!this->isHidden())
    {

        ui->lineButton->click();
        actionPage_->KeyToActionCheck(key);
    }

}

//void ICHCInstructionPageFrame::on_tabWidget_currentChanged(int index)
//{
//    if(index == 0)
//    {
//        UpdateHostParam();
//    }
//    else
//    {
//        SaveCurrentEdit();
//        //        isProgramChanged_ = false;
//    }
//}

void ICHCInstructionPageFrame::on_mainButton_clicked()
{
    OnProgramChanged(0, "");
    ui->reserveBox->setStyleSheet(oriStyle);
}

void ICHCInstructionPageFrame::on_feedButton_clicked()
{
    OnProgramChanged(8, "");
    ui->reserveBox->setStyleSheet(oriStyle);
}

void ICHCInstructionPageFrame::OnReadyLoadMold(const QString &name)
{
    //    UpdateHostParam();
}

void ICHCInstructionPageFrame::OnMoldChanged(const QString &name)
{
    UpdateHostParam();
    //    programList_ = ICMold::CurrentMold()->ToUIItems();
    //    programList_ = ICMold::MoldItemToUIItem(ICMacroSubroutine::Instance()->SubRoutine(currentEdit_ - 1));
}


void ICHCInstructionPageFrame::on_reserveBox_activated(int index)
{
    OnProgramChanged(ReserveTextToIndex(ui->reserveBox->itemText(index)), "");
    nullButton->click();
    ui->reserveBox->setStyleSheet(selStyle);
}

void ICHCInstructionPageFrame::on_fixtureCheckBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void ICHCInstructionPageFrame::on_returnButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void ICHCInstructionPageFrame::on_singleButton_clicked()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICMold* mold = ICMold::CurrentMold();
    if(!host->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
//    bool isSingleRunFinished = host->HostStatus(ICVirtualHost::ActL).toInt() == 0;
//    if(isSingleRunFinished)return;
    int currentStep = ui->moldContentListWidget->currentRow();
    ICManualRun cmd;
    if(mold->MoldContent().empty()) return;
    if(currentStep >= mold->MoldContent().size()) return;
    ICMoldItem item = mold->MoldContent().at(currentStep);
    cmd.SetSlave(1);
    if(item.GMVal() == ICMold::GARC)
    {
        cmd.SetGM(item.IFPos());
    }
    else
    {
        cmd.SetGM(item.GMVal());
    }
    cmd.SetNumber(currentStep);
    cmd.SetPoint(item.SubNum());
    cmd.SetPos(item.Pos());
    cmd.SetIFVal(item.IFVal());
    //    cmd.SetIFVal(1);
    if(ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
        //        ++currentStep;
        //        currentStep %= mold->MoldContent().size();
//        host->SetHostStatus(ICVirtualHost::ActL, 1);
//        host->SetSingleRun(true);
//        ui->singleButton->setEnabled(false);
    }

}

