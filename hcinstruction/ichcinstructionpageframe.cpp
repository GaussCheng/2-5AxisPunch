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

#include <QDebug>
#include <QMessageBox>

//struct MoldItem
//{
//    int stepNum;
//    int
//};

ICHCInstructionPageFrame::ICHCInstructionPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCInstructionPageFrame),
    injectionPage_(NULL),
    fixturePage_(NULL),
    conditionPage_(NULL),
    flagsPage_(NULL),
    pneumaticPage_(NULL),
    waitConditionPage_(NULL),
    peripheryPage_(NULL),
    cutPage_(NULL),
    programPage_(NULL),
    stackPage_(NULL),
    recordPath_("./records/"),
    currentAction_(None),
    currentEdit_(0),
    isProgramChanged_(false)
{
    ui->setupUi(this);

    InitInterface();
    InitSignal();
//    ui->moldContentListWidget->setBackgroundRole(QColor("gray"));

//    LoadAllRecordFileInfo();

    InitParameter();
//    ui->conditionsToolButton->hide();
}

ICHCInstructionPageFrame::~ICHCInstructionPageFrame()
{
    delete modifyDialog_;
    delete ui;
}

void ICHCInstructionPageFrame::showEvent(QShowEvent *e)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_None)
    {
        ui->aMinusBtn->hide();
        ui->aPlusBtn->hide();
    }
    else
    {
        ui->aMinusBtn->show();
        ui->aPlusBtn->show();
    }
    if(ICParametersSave::Instance()->IsExtentFunctionUsed())
    {
        ui->flagsButton->show();
        ui->conditionsToolButton->show();
    }
    else
    {
        ui->flagsButton->hide();
        ui->conditionsToolButton->hide();
    }
    QFrame::showEvent(e);
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StepChanged(int)),
//            this,
//            SLOT(SelectCurrentStep(int)));
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(NeedToGetTeach()),
//            this,
//            SLOT(GetTeachContent()));
//    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnTeach,
//                                                     0,
//                                                     ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
//                                                     ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());
    UpdateHostParam();
//    if(!icMainFrame->IsOrigined())
//    {
//        QMessageBox::warning(this, tr("Warning"), tr("Need to origin!"));
//    }
}

void ICHCInstructionPageFrame::hideEvent(QHideEvent *e)
{
    qDebug("instruct hide");
    QFrame::hideEvent(e);
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//               SIGNAL(StepChanged(int)),
//               this,
//               SLOT(SelectCurrentStep(int)));
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//               SIGNAL(NeedToGetTeach()),
//               this,
//               SLOT(GetTeachContent()));
//    ICMold::CurrentMold()->SetMoldContent(ICMold::UIItemToMoldItem(programList_));
//    ICMold::CurrentMold()->SaveMoldFile();
    if(SaveCurrentEdit() == true || isProgramChanged_)
    {
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
        isProgramChanged_ = false;
    }
    if(ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_ManualStatu)
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
    }
 //   ICInstructParam::Instance()->UpdateHostParam();

}

void ICHCInstructionPageFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICHCInstructionPageFrame::OptionButtonClicked()
{
    QAbstractButton * optionButton = qobject_cast<QAbstractButton *>(sender());

    if(injectionPage_ == NULL && optionButton == ui->injectionButton)
    {
        injectionPage_ = new ICHCInjectionPage;
        optionButtonToPage_.insert(ui->injectionButton, injectionPage_);
        ui->settingStackedWidget->addWidget(injectionPage_);

    }
    else if(fixturePage_ == NULL && optionButton == ui->fixtureButton)
    {
        fixturePage_ = new ICHCFixturePage;
        optionButtonToPage_.insert(ui->fixtureButton, fixturePage_);
        ui->settingStackedWidget->addWidget(fixturePage_);
    }
    else if(conditionPage_ == NULL && optionButton == ui->conditionsToolButton)
    {
        conditionPage_ = new ICHCConditionPage;
        optionButtonToPage_.insert(ui->conditionsToolButton, conditionPage_);
        ui->settingStackedWidget->addWidget(conditionPage_);
    }
    else if(flagsPage_ == NULL && optionButton == ui->flagsButton)
    {
        flagsPage_ = new ICFlagsEditor();
        optionButtonToPage_.insert(ui->flagsButton, flagsPage_);
        ui->settingStackedWidget->addWidget(flagsPage_);
    }
    else if(pneumaticPage_ == NULL && optionButton == ui->pneumaticButton)
    {
        pneumaticPage_ = new ICPneumaticActionPage();
        optionButtonToPage_.insert(ui->pneumaticButton, pneumaticPage_);
        ui->settingStackedWidget->addWidget(pneumaticPage_);
    }
    else if(waitConditionPage_ == NULL && optionButton == ui->waitToolButton)
    {
        waitConditionPage_ = new ICWaitConditionEditor();
        optionButtonToPage_.insert(ui->waitToolButton, waitConditionPage_);
        ui->settingStackedWidget->addWidget(waitConditionPage_);
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
    else if(programPage_ == NULL && optionButton == ui->programButton)
    {
        programPage_ = new ICProgramSelector();
        optionButtonToPage_.insert(ui->programButton, programPage_);
        ui->settingStackedWidget->addWidget(programPage_);
        connect(programPage_,
                SIGNAL(ProgramChanged(int, QString)),
                this,
                SLOT(OnProgramChanged(int, QString)));
    }
    else if(stackPage_ == NULL && optionButton == ui->stackButton)
    {
        stackPage_ = new ICStackEditor();
        optionButtonToPage_.insert(ui->stackButton, stackPage_);
        ui->settingStackedWidget->addWidget(stackPage_);
    }
    ui->settingStackedWidget->setCurrentWidget(optionButtonToPage_.value(optionButton));
}

void ICHCInstructionPageFrame::InitInterface()
{
    //    ui->settingFrame->setLayout(ui->settingStackedWidget);

    actionPage_ = new ActionSettingFrame(ui->actionSelectPage);
    optionButtonToPage_.insert(ui->lineButton, actionPage_);
    ui->settingStackedWidget->addWidget(actionPage_);
    modifyDialog_ = new ICInstructModifyDialog(this);
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
    connect(ui->injectionButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->fixtureButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->conditionsToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));

    connect(ui->flagsButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->pneumaticButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->waitToolButton,
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
    connect(ui->programButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->stackButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->aMinusBtn,
            SIGNAL(released()),
            SLOT(OnActionButtonReleased()));
    connect(ui->aPlusBtn,
            SIGNAL(released()),
            SLOT(OnActionButtonReleased()));
}

//void ICHCInstructionPageFrame::SelectCurrentStep(int currentStep)
//{
//    ui->moldContentListWidget->clearSelection();
//    QList<ICMoldItem> moldContent = ICMold::CurrentMold()->MoldContent();
//    QList<ICMoldItem> stepContent;
//    bool isFoundFirst = false;
//    int i;
//    for(i = 0; i != moldContent.size(); ++i)
//    {
//        if(moldContent.at(i).Num() == currentStep)
//        {
//            isFoundFirst = true;
//            ui->moldContentListWidget->item(i)->setSelected(true);
//            stepContent.append(moldContent.at(i));
//        }
//        else if(isFoundFirst)
//        {
//            break;
//        }
//    }
//    ui->moldContentListWidget->scrollToItem(ui->moldContentListWidget->item(i));

//    ICInstructionEditorBase* editor;
//    for(int i = 0; i != ui->settingStackedWidget->count(); ++i)
//    {
//        editor = qobject_cast<ICInstructionEditorBase*>(ui->settingStackedWidget->widget(i));
//        editor->SyncStatus(stepContent);
//    }
//}

//void ICHCInstructionPageFrame::LoadProgram(const QString & moldName)
//{
//}

//void ICHCInstructionPageFrame::LoadMacro(const QString & macroName)
//{
//}

//void ICHCInstructionPageFrame::on_programSelectedComboBox_activated(QString moldName)
//{
//    if(ICParametersSave::Instance()->MoldName(QString()) == moldName)
//    {
//        return;
//    }

//    int ret = QMessageBox::warning(this, tr("Warning"),
//                                   tr("Do you want to change current mold to ")
//                                   + moldName,
//                                   QMessageBox::Yes | QMessageBox::Cancel,
//                                   QMessageBox::Yes);
//    if(ret == QMessageBox::Yes)
//    {
//        LoadMoldFile(moldName);
//    }
//    ui->programSelectedComboBox->setCurrentIndex(
//                ui->programSelectedComboBox->findText(ICParametersSave::Instance()->MoldName(QString())));
//}

//void ICHCInstructionPageFrame::LoadMoldFile(const QString & moldName)
//{
//    qDebug("Load mold file");
//    QString filePathName = recordPath_ + moldName;
//    if(QFile::exists(filePathName))
//    {
//        ICMold::CurrentMold()->ReadMoldFile(filePathName);
//        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
////        emit CurrentProgramChanged();
//        qDebug("after emit updatehostparam");
//        //        UpdateHostParam();

//        ICParametersSave::Instance()->SetMoldName(moldName);
//        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnTeach,
//                                                         0,
//                                                         ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
//                                                         ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());

//        ICProgramHeadFrame::Instance()->SetCurrentMoldName(moldName);
//        //        emit CurrentMoldChanged(moldName);
//    }
//}

//void ICHCInstructionPageFrame::LoadAllRecordFileInfo()
//{
//    ui->programSelectedComboBox->clear();

//    //    ICStandardProgram* standardPrograms = ICStandardProgram::Instance();
//    QDir recordDir(recordPath_);
//    QFileInfoList fileInfoList = recordDir.entryInfoList(QDir::Files);
//    //    QFileInfoList standardProgramList;
//    QFileInfoList userProgramList;
//    foreach(const QFileInfo &fileInfo, fileInfoList)
//    {
//        /*if(standardPrograms->IsStandardProgram(fileInfo.fileName()))
//        {
//            standardProgramList.append(fileInfo);
//        }
//        else
//        {*/
//        if(fileInfo.fileName().right(3) != "fnc")
//        {
//            userProgramList.append(fileInfo);
//        }
//        //}
//    }

//    /*foreach(QFileInfo fileInfo, standardProgramList)
//    {
//        ui->programSelectedComboBox->addItem(fileInfo.fileName());
//    }*/
//    foreach(const QFileInfo &fileInfo, userProgramList)
//    {
//        ui->programSelectedComboBox->addItem(fileInfo.fileName());
//    }
//    ui->programSelectedComboBox->setCurrentIndex(-1);
//    //    ui->instructTableWidget->clearContents();
//    //    ui->instructTableWidget->setRowCount(0);
//}

void ICHCInstructionPageFrame::InitParameter()
{
    QString recordName = ICParametersSave::Instance()->MoldName("TEST.act");

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
    if(programList_.at(gIndex).StepNum() == 0)
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Can not add standby position program"));
        return;
    }
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
    if(programList_.at(gIndex).StepNum() == 0)
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Can not delete standby position program"));
        return;
    }
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
    if(programList_.at(gIndex).StepNum() == 1)
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Can not group up to standby position program"));
        return;
    }
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
    if(programList_.at(gIndex).StepNum() == 0)
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Can not decompose standby position program"));
        return;
    }
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
    isProgramChanged_ = SaveCurrentEdit();
    ui->moldContentListWidget->clear();
    programList_.clear();
    currentEdit_ = index;
//    ui->moldComboBox->setCurrentIndex(index);
    ui->programLabel->setText(name);
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
        return ICMacroSubroutine::Instance()->SaveMacroSubroutieFile(currentEdit_ - 1);
    }
}

void ICHCInstructionPageFrame::on_aPlusBtn_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_AAdd);
    ICKeyboard::Instace()->SetPressed(true);
    ShowServoAction(ICKeyboard::VFB_AAdd);
}

void ICHCInstructionPageFrame::on_aMinusBtn_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_ASub);
    ICKeyboard::Instace()->SetPressed(true);
    ShowServoAction(ICKeyboard::VFB_ASub);
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
