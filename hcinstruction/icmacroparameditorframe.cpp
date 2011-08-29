#include "icmacroparameditorframe.h"
#include "ui_icmacroparameditorframe.h"

#include <QStackedLayout>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>

#include "actionsettingframe.h"
#include "ichcfixturepage.h"
#include "ichcconditionpage.h"
#include "ichcinjectionpage.h"
#include "icinstructparam.h"

#include "icmacrosubroutine.h"
#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"
#include "icvirtualkey.h"

#include <QDebug>

ICMacroParamEditorFrame::ICMacroParamEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICMacroParamEditorFrame),
    currentMarcoNum(-1),
    currentAction_(None),
    recordPath_("./subs/"),
    settingStackedLayout_(new QStackedLayout)
{
    ui->setupUi(this);

    InitInterface();
    InitSignal();

    LoadAllRecordFileInfo();
//    InitParameter();
}

void ICMacroParamEditorFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StepChanged(int)),
            this,
            SLOT(SelectCurrentStep(int)));
    LoadMacro(currentAction_);
}

void ICMacroParamEditorFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StepChanged(int)),
               this,
               SLOT(SelectCurrentStep(int)));
    ICMacroSubroutine::Instance()->SaveMacroSubroutieFiles();
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
}

void ICMacroParamEditorFrame::changeEvent(QEvent *e)
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

ICMacroParamEditorFrame::~ICMacroParamEditorFrame()
{
    delete ui;
}

void ICMacroParamEditorFrame::SelectCurrentStep(int currentStep)
{
    ui->moldContentListWidget->clearSelection();
    QList<ICMoldItem> moldContent = ICMacroSubroutine::Instance()->SubRoutine(currentMarcoNum);
    bool isFoundFirst = false;
    int i;
    for(i = 0; i != moldContent.size(); ++i)
    {
        if(moldContent.at(i).Num() == currentStep)
        {
            isFoundFirst = true;
            ui->moldContentListWidget->item(i)->setSelected(true);
        }
        else if(isFoundFirst)
        {
            break;
        }
    }
    ui->moldContentListWidget->scrollToItem(ui->moldContentListWidget->item(i));
}

void ICMacroParamEditorFrame::OptionButtonClicked()
{
    QAbstractButton * optionButton = qobject_cast<QAbstractButton *>(sender());

    settingStackedLayout_->setCurrentWidget(optionButtonToPage_.value(optionButton));
}

void ICMacroParamEditorFrame::LoadAllRecordFileInfo()
{
    ui->programSelectedComboBox->clear();

//    ICStandardProgram* standardPrograms = ICStandardProgram::Instance();
    QDir recordDir(recordPath_);
    QFileInfoList fileInfoList = recordDir.entryInfoList(QDir::Files);
//    QFileInfoList standardProgramList;
    QFileInfoList userProgramList;
    foreach(const QFileInfo &fileInfo, fileInfoList)
    {
        /*if(standardPrograms->IsStandardProgram(fileInfo.fileName()))
        {
            standardProgramList.append(fileInfo);
        }
        else
        {*/
        if(fileInfo.fileName().right(3) != "fnc")
        {
            userProgramList.append(fileInfo);
        }
        //}
    }

    /*foreach(QFileInfo fileInfo, standardProgramList)
    {
        ui->programSelectedComboBox->addItem(fileInfo.fileName());
    }*/
    foreach(const QFileInfo &fileInfo, userProgramList)
    {
        ui->programSelectedComboBox->addItem(fileInfo.fileName());
    }
    ui->programSelectedComboBox->setCurrentIndex(-1);
    //    ui->instructTableWidget->clearContents();
    //    ui->instructTableWidget->setRowCount(0);
}

void ICMacroParamEditorFrame::InitInterface()
{
    ui->settingFrame->setLayout(settingStackedLayout_);

    actionPage_ = new ActionSettingFrame;
    optionButtonToPage_[ui->actionToolButton] = actionPage_;
    settingStackedLayout_->addWidget(actionPage_);

    injectionPage_ = new ICHCInjectionPage;
    optionButtonToPage_[ui->injectionToolButton] = injectionPage_;
    settingStackedLayout_->addWidget(injectionPage_);

    fixturePage_ = new ICHCFixturePage;
    optionButtonToPage_[ui->fixtureToolButton] = fixturePage_;
    settingStackedLayout_->addWidget(fixturePage_);

    conditionPage_ = new ICHCConditionPage;
    optionButtonToPage_[ui->conditionsToolButton] = conditionPage_;
    settingStackedLayout_->addWidget(conditionPage_);

    settingStackedLayout_->setCurrentWidget(actionPage_);
}

void ICMacroParamEditorFrame::InitSignal()
{
    connect(ui->actionToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->injectionToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->fixtureToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ui->conditionsToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(OptionButtonClicked()));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(NeedToGetTeach()),
            this,
            SLOT(GetTeachContent()));

//    connect(this,
//            SIGNAL(CurrentProgramChanged()),
//            ICInstructParam::Instance(),
//            SLOT(UpdateHostParam()));

//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StepChanged(int)),
//            this,
//            SLOT(SelectCurrentStep(int)));

//    connect(ICInstructParam::Instance(),
//            SIGNAL(HostParamChanged()),
//            this,
//            SLOT(UpdateHostParam()));
}

void ICMacroParamEditorFrame::on_programSelectedComboBox_activated(int index)
{
    int cmd = IC::CMD_TurntchSub0 + index;
    ICCommandProcessor* process = ICCommandProcessor::Instance();
    process->ExecuteHCCommand(cmd,
                              0,
                              ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
                              ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());

//    if(process->IsAlarmed())
//    {
//        return;
//    }
    if(currentMarcoNum != -1)
    {
        int ret = QMessageBox::warning(this, tr("Warning"),
                                       tr("Do you want to change current mold to ") \
                                       + ui->programSelectedComboBox->currentText(),
                                       QMessageBox::Yes | QMessageBox::Cancel,
                                       QMessageBox::Yes);

        if(ret == QMessageBox::Yes)
        {
            LoadMacro(index);
        }
        else
        {
            ui->programSelectedComboBox->setCurrentIndex(currentMarcoNum);
        }
    }
    else
    {
        LoadMacro(index);
    }

    currentMarcoNum = index;
}

void ICMacroParamEditorFrame::LoadMacro(int macro)
{
    ui->moldContentListWidget->clear();

//    ICMacroSubroutine::Instance()->SubRoutine(macro);
    QStringList macroParamList = ICInstructParam::Instance()->ContentParam(ICMacroSubroutine::Instance()->SubRoutine(macro));
    ui->moldContentListWidget->addItems(macroParamList);
}

void ICMacroParamEditorFrame::InitParameter()
{
}

//    void LoadMoldFile(const QString & moldName);

void ICMacroParamEditorFrame::on_insertToolButton_clicked()
{
    if(ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_INS))
    {
        currentAction_ = Insert;

        //do something
    }
}

void ICMacroParamEditorFrame::on_modifyToolButton_clicked()
{
    if(ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_MODIFY))
    {
        currentAction_ = Modify;
        //do something
    }
}

void ICMacroParamEditorFrame::on_deleteToolButton_clicked()
{
    if(ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_DEL))
    {
        //do something
//        ICMold::CurrentMold()->Delete(ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::Step).toInt());
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        if(host->CurrentStep() != ICMacroSubroutine::Instance()->LastStep(currentMarcoNum) && host->CurrentStep() != 0)
        {
            ICMacroSubroutine::Instance()->Delete(host->CurrentStep(), currentMarcoNum);
            LoadMacro(currentMarcoNum);
        }
    }
}

void ICMacroParamEditorFrame::on_nextStepToolButton_clicked()
{
    if(ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_F4))
    {
        //do something
    }
}

void ICMacroParamEditorFrame::on_priorToolButton_clicked()
{
    if(ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_F3))
    {
        //do something
    }
}

void ICMacroParamEditorFrame::GetTeachContent()
{
    QList<QList<uint> > moldStepContent = ICCommandProcessor::Instance()->TeachStep();
    QList<ICMoldItem> items;
    ICMoldItem item;
    for(int i = 0; i!= moldStepContent.size(); ++i)
    {
        item.SetValue(moldStepContent.at(i).at(0),
                      moldStepContent.at(i).at(1),
                      moldStepContent.at(i).at(2),
                      moldStepContent.at(i).at(3),
                      moldStepContent.at(i).at(4),
                      moldStepContent.at(i).at(5),
                      moldStepContent.at(i).at(6),
                      moldStepContent.at(i).at(7),
                      moldStepContent.at(i).at(8),
                      moldStepContent.at(i).at(9));
        items.append(item);
    }
    if(items.isEmpty())
    {
        currentAction_ = None;
        return;
    }
    switch(currentAction_)
    {
    case Insert:
        {
            if(ICVirtualHost::GlobalVirtualHost()->CurrentStep() == 0)
            {
                ICMacroSubroutine::Instance()->Modify(items, currentMarcoNum);
            }
            else
            {
                ICMacroSubroutine::Instance()->Insert(items, currentMarcoNum);
            }
        }
        break;
    case Modify:
        {
            if(ICVirtualHost::GlobalVirtualHost()->CurrentStep() == ICMacroSubroutine::Instance()->LastStep(currentMarcoNum))
            {
                ICMacroSubroutine::Instance()->Insert(items, currentMarcoNum);
            }
            else
            {
                ICMacroSubroutine::Instance()->Modify(items, currentMarcoNum);
            }
        }
        break;
    }
    currentAction_ = None;
//    QString receive;
//    QList<ICMoldItem> moldContent = ICMold::CurrentMold()->MoldContent();
//    for(int i = 0; i != moldContent.size(); ++i)
//    {
//        receive += moldContent.at(i).ToString() + "\n";
//    }
//    QMessageBox::information(this,
//                             "Modify",
//                             receive);

    LoadMacro(currentMarcoNum);
    if(!items.isEmpty())
    {
        SelectCurrentStep(items.at(0).Num());
    }
//    ICMold
}

//void ICMacroParamEditorFrame::CurrentMoldFileChanged(const QString & moldName)
//{
//}
