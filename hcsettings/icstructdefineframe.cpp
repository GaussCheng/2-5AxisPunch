#include "icstructdefineframe.h"
#include "ui_icstructdefineframe.h"

#include "iccommandprocessor.h"
#include "iccommands.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"

#include <QVector>
#include <QMessageBox>
#include <QDebug>
#include "mainframe.h"
#include "icactioncommand.h"
#include "icvirtualkey.h"
#include "icmodifyframe.h"
#include "icsystemconfig.h"

typedef union {
     struct {
        uint16_t xLimit : 2;
        uint16_t yLimit : 2;
        uint16_t sLimit : 2;
        uint16_t rLimit : 2;
        uint16_t tLimit : 2;
        uint16_t rev : 6;
    }b;
    uint16_t all;
}AxisLimitConfig;

typedef union{
    struct {
        uint16_t x : 2;
        uint16_t y : 2;
        uint16_t z : 2;
        uint16_t p : 2;
        uint16_t q : 2;
        uint16_t a : 2;
        uint16_t b : 2;
        uint16_t c : 2;
    }b;
    uint16_t combine;
}OriginStatus;

typedef union{
    struct {
        uint16_t a1 : 3;
        uint16_t a2 : 3;
        uint16_t a3 : 3;
        uint16_t a4 : 3;
        uint16_t a5 : 3;
        uint16_t resv : 1;
    }mode;
    uint16_t allMode;
}AxisMode;

QString newStyle;
QString oldStyle;
ICStructDefineFrame::ICStructDefineFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICStructDefineFrame)
{
    ui->setupUi(this);

    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    /*axis define*/
    boxToAxis_.insert(ui->x1Box, ICVirtualHost::ICAxis_AxisX1);
    boxToAxis_.insert(ui->y1Box, ICVirtualHost::ICAxis_AxisY1);
    boxToAxis_.insert(ui->x2Box, ICVirtualHost::ICAxis_AxisX2);
    boxToAxis_.insert(ui->y2Box, ICVirtualHost::ICAxis_AxisY2);
    boxToAxis_.insert(ui->zBox, ICVirtualHost::ICAxis_AxisZ);
    boxToAxis_.insert(ui->aBox, ICVirtualHost::ICAxis_AxisA);
    boxToAxis_.insert(ui->bBox, ICVirtualHost::ICAxis_AxisB);
    boxToAxis_.insert(ui->cBox, ICVirtualHost::ICAxis_AxisC);

    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_None, 0);
    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_Pneumatic, 1);
    armDefineToIndex_.insert(ICVirtualHost::ICAxisDefine_Servo, 2);
    indexToArmDefine_.insert(0, ICVirtualHost::ICAxisDefine_None);
    indexToArmDefine_.insert(1, ICVirtualHost::ICAxisDefine_Pneumatic);
    indexToArmDefine_.insert(2, ICVirtualHost::ICAxisDefine_Servo);

    QList<QComboBox*> boxs = ui->armDefineBox->findChildren<QComboBox*>();
    axisDefine_ = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    for(int i = 0; i != boxs.size(); ++i)
    {
        boxs[i]->setCurrentIndex(armDefineToIndex_.value(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(boxs.at(i))))));
        connect(boxs[i],
                SIGNAL(currentIndexChanged(int)),
                SLOT(OnAxisDefineChanged(int)));
    }
    InitEscapeBox() ;

    punchButtons_<<ui->punch1<<ui->punch2<<ui->punch3<<ui->punch4<<ui->punch5<<ui->punch6
                   <<ui->punch7<<ui->punch8<<ui->punch9<<ui->punch10
                  << ui->punch11 << ui->punch12 << ui->punch13 << ui->punch14
                  << ui->punch15 << ui->punch16;

    inputBoxs << ui->inputBox_1 << ui->inputBox_2 << ui->inputBox_3 << ui->inputBox_4 << ui->inputBox_5 << ui->inputBox_6
              << ui->inputBox_7 << ui->inputBox_8 << ui->inputBox_9 << ui->inputBox_10 << ui->inputBox_11 << ui->inputBox_12
              << ui->inputBox_13 << ui->inputBox_14 << ui->inputBox_15 << ui->inputBox_16 << ui->inputBox_17 << ui->inputBox_18
              << ui->inputBox_19 << ui->inputBox_20 << ui->inputBox_21 << ui->inputBox_22 << ui->inputBox_23 << ui->inputBox_24
              << ui->inputBox_25 << ui->inputBox_26 << ui->inputBox_27 << ui->inputBox_28 << ui->inputBox_29 << ui->inputBox_30
              << ui->inputBox_31 << ui->inputBox_32;

    ouputBoxs << ui->outputBox_1 << ui->outputBox_2 << ui->outputBox_3 << ui->outputBox_4 << ui->outputBox_5 << ui->outputBox_6
              << ui->outputBox_7 << ui->outputBox_8 << ui->outputBox_9 << ui->outputBox_10 << ui->outputBox_11 << ui->outputBox_12
              << ui->outputBox_13 << ui->outputBox_14 << ui->outputBox_15 << ui->outputBox_16 << ui->outputBox_17 << ui->outputBox_18
              << ui->outputBox_19 << ui->outputBox_20 << ui->outputBox_21 << ui->outputBox_22 << ui->outputBox_23 << ui->outputBox_24
              << ui->outputBox_25 << ui->outputBox_26 << ui->outputBox_27 << ui->outputBox_28 << ui->outputBox_29 << ui->outputBox_30
              << ui->outputBox_31 << ui->outputBox_32;

    int machineCount = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Fixture).toInt();
    for(int i = 0; i != punchButtons_.size(); ++i)
    {
        if(machineCount & ((1 << (i))))
        {
            punchButtons_[i]->setChecked(true);
        }
    }

    CanConfig canConfig;
    canConfig.all = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv1).toInt();
    ui->canType->setCurrentIndex(canConfig.b.canType);
    ui->canID->SetThisIntToThisText(canConfig.b.canAddr);

    ReserveProgConfig progConfig;
    progConfig.all =  ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv2).toInt();
    ui->rP1->setCurrentIndex(progConfig.b.r1);
    ui->rP2->setCurrentIndex(progConfig.b.r2);
    ui->rP3->setCurrentIndex(progConfig.b.r3);
    ui->rP4->setCurrentIndex(progConfig.b.r4);
    ui->rP5->setCurrentIndex(progConfig.b.r5);
    ui->rP6->setCurrentIndex(progConfig.b.r6);
    ui->rP7->setCurrentIndex(progConfig.b.r7);
    ui->rP8->setCurrentIndex(progConfig.b.r8);

    AxisLimitConfig axisLimitconfig;
    axisLimitconfig.all = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Signal).toUInt();
    ui->xLimit->setChecked(axisLimitconfig.b.xLimit);
    ui->yLimit->setChecked(axisLimitconfig.b.yLimit);
    ui->sLimit->setChecked(axisLimitconfig.b.sLimit);
    ui->rLimit->setChecked(axisLimitconfig.b.rLimit);
    ui->tLimit->setChecked(axisLimitconfig.b.tLimit);


    ui->aBox->hide();
    ui->bBox->hide();
    ui->cBox->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_16->hide();

    AxisMode axisMode;
    axisMode.allMode = host->SystemParameter(ICVirtualHost::SYS_Config_Out).toInt();
//    ui->waitTime->SetThisIntToThisText(recycleMode.split.time);
//    ui->waitTime->SetDecimalPlaces(1);
//    ui->waitTime->setValidator(new QIntValidator(0, 100, this));
    ui->os1->setCurrentIndex(axisMode.mode.a1);
    ui->os2->setCurrentIndex(axisMode.mode.a2);
    ui->os3->setCurrentIndex(axisMode.mode.a3);
    ui->os4->setCurrentIndex(axisMode.mode.a4);
    ui->os5->setCurrentIndex(axisMode.mode.a5);


    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)  ui->xLimit->show();
    else ui->xLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)  ui->yLimit->show();
    else ui->yLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)  ui->sLimit->show();
    else ui->sLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)  ui->rLimit->show();
    else ui->rLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)  ui->tLimit->show();
    else ui->tLimit->hide();
    oldStyle = ui->oStartBtn->styleSheet();
    newStyle = "border-style: outset;border-width: 2px;border-radius: 6px;border-color: gray;background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(0, 255, 0, 255), stop:1 rgba(255, 255, 255, 255));padding-right: 6px;padding-left:6px;";
    timerID_ = -1;

    ui->pressureMode->setCurrentIndex(host->PressureCheckMode());
    ui->originSpd->setValidator(new QIntValidator(0, 30, this));
    ui->originSpd->SetThisIntToThisText(host->SystemParameter(ICVirtualHost::SYS_OriginSpeed).toInt());
    ui->tryRunBox->setChecked(host->IsMidMoldCheck());
    ui->syncBox->setChecked(host->IsOrignSyncCheck());
    ui->downModeBox->setChecked(host->IsCloseMoldEn());
    ui->safeInfoBox->setChecked(host->IsSafeInfoModeEn());
    ui->fleeBox->setChecked(host->IsFleeEn());
    ui->originBox->setChecked(host->IsOriginModeEn());
    ui->autoBox->setChecked(host->IsAutoModeEn());
    ui->punchType->setCurrentIndex(host->PunchCheckMode());
    ui->waitMode->setCurrentIndex(host->IsWaitModeEn());



    quint32 check1,check2;
    check1 = ICMold::CurrentMold()->MoldParam(ICMold::check1);
    check2 = ICMold::CurrentMold()->MoldParam(ICMold::check2);

    ui->x37Box->setChecked(check2&(1 << (23 - 16)));
    ui->x40Box->setChecked(check2&(1 << (24 - 16)));
    ui->x22Box->setChecked(check1&(1 << 10));
    ui->x23Box->setChecked(check1&(1 << 11));

    InitEnfoce();

    editorToConfigIDs_.insert(ui->x1Box, ICConfigString::kCS_STRUCT_Axis_Define_X1);
    editorToConfigIDs_.insert(ui->y1Box, ICConfigString::kCS_STRUCT_Axis_Define_Y1);
    editorToConfigIDs_.insert(ui->zBox, ICConfigString::kCS_STRUCT_Axis_Define_Z);
    editorToConfigIDs_.insert(ui->x2Box, ICConfigString::kCS_STRUCT_Axis_Define_X2);
    editorToConfigIDs_.insert(ui->y2Box, ICConfigString::kCS_STRUCT_Axis_Define_Y2);
    editorToConfigIDs_.insert(ui->aBox, ICConfigString::kCS_STRUCT_Axis_Define_A);
    editorToConfigIDs_.insert(ui->bBox, ICConfigString::kCS_STRUCT_Axis_Define_B);
    editorToConfigIDs_.insert(ui->cBox, ICConfigString::kCS_STRUCT_Axis_Define_C);
    editorToConfigIDs_.insert(ui->xLimit, ICConfigString::kCS_STRUCT_Limit_Define_Arm_X_Limit);
    editorToConfigIDs_.insert(ui->yLimit, ICConfigString::kCS_STRUCT_Limit_Define_Arm_Y_Limit);
    editorToConfigIDs_.insert(ui->rLimit, ICConfigString::kCS_STRUCT_Limit_Define_Arm_R_Limit);
    editorToConfigIDs_.insert(ui->sLimit, ICConfigString::kCS_STRUCT_Limit_Define_Arm_S_Limit);
    editorToConfigIDs_.insert(ui->tLimit, ICConfigString::kCS_STRUCT_Limit_Define_Arm_T_Limit);
    editorToConfigIDs_.insert(ui->canType, ICConfigString::kCS_STRUCT_Config_CanType);
    editorToConfigIDs_.insert(ui->canID, ICConfigString::kCS_STRUCT_Config_CanId);
    editorToConfigIDs_.insert(ui->punchType, ICConfigString::kCS_STRUCT_Config_PunchType);
    editorToConfigIDs_.insert(ui->pressureMode, ICConfigString::kCS_STRUCT_Config_PressureCheck);
    editorToConfigIDs_.insert(ui->tryRunBox, ICConfigString::kCS_STRUCT_Config_TryRun);
    editorToConfigIDs_.insert(ui->downModeBox, ICConfigString::kCS_STRUCT_Config_DownMode);
    editorToConfigIDs_.insert(ui->fleeBox, ICConfigString::kCS_STRUCT_Config_Flee);
    editorToConfigIDs_.insert(ui->originBox, ICConfigString::kCS_STRUCT_Config_Origin);
    editorToConfigIDs_.insert(ui->autoBox, ICConfigString::kCS_STRUCT_Config_Auto);
    editorToConfigIDs_.insert(ui->os1, ICConfigString::kCS_STRUCT_Config_OriginX);
    editorToConfigIDs_.insert(ui->os2, ICConfigString::kCS_STRUCT_Config_OriginY);
    editorToConfigIDs_.insert(ui->os3, ICConfigString::kCS_STRUCT_Config_OriginS);
    editorToConfigIDs_.insert(ui->os4, ICConfigString::kCS_STRUCT_Config_OriginR);
    editorToConfigIDs_.insert(ui->os5, ICConfigString::kCS_STRUCT_Config_OriginT);
    editorToConfigIDs_.insert(ui->syncBox, ICConfigString::kCS_STRUCT_Config_SyncOrigin);
    editorToConfigIDs_.insert(ui->originSpd, ICConfigString::kCS_STRUCT_Config_OriginSpeed);
    editorToConfigIDs_.insert(ui->x37Box, ICConfigString::kCS_STRUCT_Config_Suck1);
    editorToConfigIDs_.insert(ui->x40Box, ICConfigString::kCS_STRUCT_Config_Suck2);
    editorToConfigIDs_.insert(ui->x22Box, ICConfigString::kCS_STRUCT_Config_Clip1);
    editorToConfigIDs_.insert(ui->x23Box, ICConfigString::kCS_STRUCT_Config_Clip2);

    ICLogInit



    ui->tabWidget->removeTab(1);
    ui->label_23->hide();
    ui->orignStatus->hide();
}

ICStructDefineFrame::~ICStructDefineFrame()
{
    delete ui;
}


void ICStructDefineFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {

    case QEvent::LanguageChange:{
        QList<QComboBox*> boxs = boxToAxis_.keys();
        for(int i=0;i<boxs.size();i++){
            boxs.at(i)->blockSignals(true);
        }
        ui->retranslateUi(this);
        for(int i=0;i<boxs.size();i++){
            boxs.at(i)->blockSignals(false);
        }
        retranslateUi_();
        InitCombobox();

    }

        break;
    default:
        break;
    }
}

void ICStructDefineFrame::hideEvent(QHideEvent *e)
{
    icMainFrame->BlockOrignShow(false);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    ui->tabWidget->setCurrentIndex(0);
    QWidget::hideEvent(e);
}

void ICStructDefineFrame::timerEvent(QTimerEvent *)
{
    if(ui->tabWidget->currentWidget() != ui->origin) return;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->CurrentStatus() == ICVirtualHost::Origin)
    {
        ui->oStartBtn->setStyleSheet(newStyle);
        ui->oStartBtn->setText(tr("Origining"));
    }
    else
    {
        ui->oStartBtn->setStyleSheet(oldStyle);
        ui->oX1Btn->setStyleSheet(oldStyle);
        ui->oX2Btn->setStyleSheet(oldStyle);
        ui->oY1Btn->setStyleSheet(oldStyle);
        ui->oY2Btn->setStyleSheet(oldStyle);
        ui->oZBtn->setStyleSheet(oldStyle);
        ui->oStartBtn->setText(tr("Start"));
        return;
    }

    OriginStatus os;
    os.combine = host->HostStatus(ICVirtualHost::DbgX1).toUInt();
    ui->orignStatus->setText(QString::number(host->HostStatus(ICVirtualHost::DbgX1).toInt(),16));
    if(os.b.x)
    {
        ui->oX1Btn->setStyleSheet(newStyle);
    }
    else
    {
        ui->oX1Btn->setStyleSheet(oldStyle);
    }

    if(os.b.y)
    {
        ui->oY1Btn->setStyleSheet(newStyle);
    }
    else
    {
        ui->oY1Btn->setStyleSheet(oldStyle);
    }

    if(os.b.z)
    {
        ui->oZBtn->setStyleSheet(newStyle);
    }
    else
    {
        ui->oZBtn->setStyleSheet(oldStyle);
    }

    if(os.b.p)
    {
        ui->oX2Btn->setStyleSheet(newStyle);
    }
    else
    {
        ui->oX2Btn->setStyleSheet(oldStyle);
    }

    if(os.b.q)
    {
        ui->oY2Btn->setStyleSheet(newStyle);
    }
    else
    {
        ui->oY2Btn->setStyleSheet(oldStyle);
    }

}


void ICStructDefineFrame::retranslateUi_()
{
    this->setWindowTitle(tr("Form"));
    ui->armDefineBox->setTitle(tr("Arm Define"));
//    ui->label_5->setText(tr("X1"));
    ui->x1Box->setItemText(0,tr("None"));
    ui->x1Box->setItemText(1,tr("Pneumatic"));
    ui->x1Box->setItemText(2,tr("Servo"));
//    ui->label_14->setText(tr("C"));
    ui->y1Box->setItemText(0,tr("None"));
    ui->y1Box->setItemText(1,tr("Pneumatic"));
    ui->y1Box->setItemText(2,tr("Servo"));
//    ui->label_13->setText(tr("Z"));
    ui->cBox->setItemText(0,tr("None"));
    ui->cBox->setItemText(1,tr("Pneumatic"));
    ui->cBox->setItemText(2,tr("Servo"));
//    ui->label_15->setText(tr("A"));
    ui->aBox->setItemText(0,tr("None"));
    ui->aBox->setItemText(1,tr("Pneumatic"));
    ui->aBox->setItemText(2,tr("Servo"));
//    ui->label_16->setText(tr("B"));
    ui->zBox->setItemText(0,tr("None"));
    ui->zBox->setItemText(1,tr("Pneumatic"));
    ui->zBox->setItemText(2,tr("Servo"));
//    ui->label_7->setText(tr("Y1"));
    ui->bBox->setItemText(0,tr("None"));
    ui->bBox->setItemText(1,tr("Pneumatic"));
    ui->bBox->setItemText(2,tr("Servo"));
//    ui->label_6->setText(tr("X2"));
    ui->x2Box->setItemText(0,tr("None"));
    ui->x2Box->setItemText(1,tr("Pneumatic"));
    ui->x2Box->setItemText(2,tr("Servo"));
//    ui->label_12->setText(tr("Y2"));
    ui->y2Box->setItemText(0,tr("None"));
    ui->y2Box->setItemText(1,tr("Pneumatic"));
    ui->y2Box->setItemText(2,tr("Servo"));
//    ui->outABox->setItemText(0,tr("Normal"));
//    ui->outABox->setItemText(1,tr("Extent"));
//    ui->label_8->setText(tr("5"));
//    ui->outEBox->setItemText(0,tr("Normal"));
//    ui->outEBox->setItemText(1,tr("Extent"));
//    ui->label_2->setText(tr("2"));
//    ui->outBBox->setItemText(0,tr("Normal"));
//    ui->outBBox->setItemText(1,tr("Extent"));
//    ui->label_9->setText(tr("6"));
//    ui->outFBox->setItemText(0,tr("Normal"));
//    ui->outFBox->setItemText(1,tr("Extent"));
//    ui->label_3->setText(tr("3"));
//    ui->outCBox->setItemText(0,tr("Normal"));
//    ui->outCBox->setItemText(1,tr("Extent"));
//    ui->label_11->setText(tr("7"));
//    ui->outGBox->setItemText(0,tr("Normal"));
//    ui->outGBox->setItemText(1,tr("Extent"));
//    ui->label_4->setText(tr("4"));
//    ui->outDBox->setItemText(0,tr("Normal"));
//    ui->outDBox->setItemText(1,tr("Extent"));
//    ui->label_10->setText(tr("8"));
//    ui->outHBox->setItemText(0,tr("Normal"));
//    ui->outHBox->setItemText(1,tr("Extent"));

    ui->fixtureDefineBox_2->setTitle(tr("Other Define"));
    ui->saveButton->setText(tr("Save"));
}

void ICStructDefineFrame::on_saveButton_clicked()
{
    ICSetAxisConfigsCommand command;
    ICCommandProcessor* process = ICCommandProcessor::Instance();
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int sum = 0;
    int machineCount = 0;
    for(int i = 0; i != punchButtons_.size(); ++i)
    {
        if(punchButtons_.at(i)->isChecked())
        {
            machineCount |= (1 << i);
        }
    }
    ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, machineCount);
    QVector<uint> dataBuffer(7, 0);
    AxisLimitConfig axisLimitConfig;
    axisLimitConfig.b.xLimit = ui->xLimit->isChecked();
    axisLimitConfig.b.yLimit = ui->yLimit->isChecked();
    axisLimitConfig.b.sLimit = ui->sLimit->isChecked();
    axisLimitConfig.b.rLimit = ui->rLimit->isChecked();
    axisLimitConfig.b.tLimit = ui->tLimit->isChecked();
    dataBuffer[0] = axisLimitConfig.all;
    dataBuffer[1] = axisDefine_;
    AxisMode axisMode;
    axisMode.mode.a1 = ui->os1->currentIndex();
    axisMode.mode.a2 = ui->os2->currentIndex();
    axisMode.mode.a3 = ui->os3->currentIndex();
    axisMode.mode.a4 = ui->os4->currentIndex();
    axisMode.mode.a5 = ui->os5->currentIndex();
    dataBuffer[2] = axisMode.allMode;
//    dataBuffer[3] = ICVirtualHost::GlobalVirtualHost()->FixtureDefineSwitch(ui->fixtureSelectBox->currentIndex());
    dataBuffer[3] = machineCount;
    CanConfig canConfig;
    canConfig.b.canType = ui->canType->currentIndex();
    canConfig.b.canAddr = ui->canID->TransThisTextToThisInt();
    ReserveProgConfig progConfig;
    progConfig.b.r1 = ui->rP1->currentIndex();
    progConfig.b.r2 = ui->rP2->currentIndex();
    progConfig.b.r3 = ui->rP3->currentIndex();
    progConfig.b.r4 = ui->rP4->currentIndex();
    progConfig.b.r5 = ui->rP5->currentIndex();
    progConfig.b.r6 = ui->rP6->currentIndex();
    progConfig.b.r7 = ui->rP7->currentIndex();
    progConfig.b.r8 = ui->rP8->currentIndex();
    dataBuffer[4] = canConfig.all;
    dataBuffer[5] = progConfig.all;
    for(int i = 0; i != 6; ++i)
    {
        sum += dataBuffer.at(i);
    }
    sum = (-sum & 0XFFFF);
    dataBuffer[6] = sum;
    command.SetSlave(process->SlaveID());
    command.SetDataBuffer(dataBuffer);
    command.SetAxis(8);
#ifndef Q_WS_X11
    if(process->ExecuteCommand(command).toBool())
#endif
    {
        host->SetAxisDefine(axisDefine_);
        host->SetPeripheryOutput(0);
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Fixture, dataBuffer.at(3));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv1, dataBuffer.at(4));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Resv2, dataBuffer.at(5));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Xorsum, dataBuffer.at(6));
        host->SetSystemParameter(ICVirtualHost::SYS_ARM_CONFIG, 0);
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Signal, dataBuffer.at(0));
        host->SetSystemParameter(ICVirtualHost::SYS_Config_Out, dataBuffer.at(2));
        host->SetPressureCheckMode(ui->pressureMode->currentIndex());
        host->SetSystemParameter(ICVirtualHost::SYS_OriginSpeed, ui->originSpd->TransThisTextToThisInt());
        host->SetSystemParameter(ICVirtualHost::Sys_EnforceInput0, enforceInputs() & 0xFFFF);
        host->SetSystemParameter(ICVirtualHost::Sys_EnforceInput1, (enforceInputs() >> 16) & 0xFFFF);
        host->SetSystemParameter(ICVirtualHost::Sys_EnforceOutput0, enforceOutputs() & 0xFFFF);
        host->SetSystemParameter(ICVirtualHost::Sys_EnforceOutput1, (enforceOutputs() >> 16) & 0xFFFF);
        host->SetMidMoldCheck(ui->tryRunBox->isChecked());
        host->SetCloseMoldEn(ui->downModeBox->isChecked());
        host->SetOrignSyncCheck(ui->syncBox->isChecked());
        host->SetPunchCheckMode(ui->punchType->currentIndex());
        host->SetFleeEn(ui->fleeBox->isChecked());
        host->SetOriginModeEn(ui->originBox->isChecked());
        host->SetAutoModeEn(ui->autoBox->isChecked());
        host->SetWaitModeEn(ui->waitMode->currentIndex());
        host->SetSafeInfoModeEn(ui->safeInfoBox->isChecked());

        if(ui->canType->currentIndex() == 0){
            host->SetEjectionLink(0);
        }
        //        host->SystemParameter(ICVirtualHost::SYS_Function);
        host->SaveSystemConfig();
        QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
        emit StructChanged();
        icMainFrame->UpdateAxisDefine_();
        ICModifyFrame::Instance()->OnActionTriggered(ICConfigString::kCS_STRUCT_Config_Save, tr("Save"), "");
    }

    qDebug()<<"Arm Define"<<axisDefine_;
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)  ui->xLimit->show();
    else ui->xLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)  ui->yLimit->show();
    else ui->yLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)  ui->sLimit->show();
    else ui->sLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)  ui->rLimit->show();
    else ui->rLimit->hide();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)  ui->tLimit->show();
    else ui->tLimit->hide();

    quint32 check1,check2;
    check1 = ICMold::CurrentMold()->MoldParam(ICMold::check1);
    check2 = ICMold::CurrentMold()->MoldParam(ICMold::check2);

    if(ui->x22Box->isChecked()){
        check1 |= (1 << 10);
    }
    else{
        check1 &= ~(1 << 10);
    }
    if(ui->x23Box->isChecked()){
        check1 |= (1 << 11);
    }
    else{
        check1 &= ~(1 << 11);
    }
    if(ui->x37Box->isChecked()){
        check2 |= (1 << (23 - 16));
    }
    else{
        check2 &= ~(1 << (23 - 16));
    }
    if(ui->x40Box->isChecked()){
        check2 |= (1 << (24 - 16));
    }
    else{
        check2 &= ~(1 << (24 - 16));
    }

    ICMold::CurrentMold()->SetMoldParam(ICMold::check1,check1);
    ICMold::CurrentMold()->SetMoldParam(ICMold::check2,check2);

    ICMold::CurrentMold()->SaveMoldParamsFile();

}



void ICStructDefineFrame::OnAxisDefineChanged(int index)
{
    QComboBox* box = qobject_cast<QComboBox*>(sender());
    ICVirtualHost::GlobalVirtualHost()->CalAxisDefine(axisDefine_,
                                                      static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(box)),
                                                      static_cast<ICVirtualHost::ICAxisDefine>(indexToArmDefine_.value(index)));
}


void ICStructDefineFrame::escapeBoxChange()
{
    ICVirtualHost::GlobalVirtualHost()->SetEscapeWay(buttongroup_->checkedId());
}

void ICStructDefineFrame::InitEscapeBox()
{
    buttongroup_ = new QButtonGroup();
    QList<QAbstractButton*> buttons = buttongroup_->buttons();
    for(int i = 0; i != buttons.size(); ++i)
    {
        buttons[i]->setCheckable(true);
        connect(buttons.at(i),
                SIGNAL(clicked()),
                this,
                SLOT(escapeBoxChange()));
    }
    buttongroup_->setExclusive(true);
}

void ICStructDefineFrame::InitCombobox()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    QList<QComboBox*> boxs = ui->armDefineBox->findChildren<QComboBox*>();
    for(int i = 0; i != boxs.size(); ++i)
    {
        boxs[i]->blockSignals(true);
        boxs[i]->setCurrentIndex(armDefineToIndex_.value(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(boxToAxis_.value(boxs.at(i))))));
        boxs[i]->blockSignals(false);
    }
    CanConfig canConfig;
    canConfig.all = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv1).toInt();
    ui->canType->blockSignals(true);
    ui->canType->setCurrentIndex(canConfig.b.canType);
    ui->canType->blockSignals(false);

    ui->pressureMode->blockSignals(true);
    ui->pressureMode->setCurrentIndex(host->PressureCheckMode());
    ui->pressureMode->blockSignals(false);

    AxisMode axisMode;
    axisMode.allMode = host->SystemParameter(ICVirtualHost::SYS_Config_Out).toInt();

    ui->os1->blockSignals(true);
    ui->os2->blockSignals(true);
    ui->os3->blockSignals(true);
    ui->os4->blockSignals(true);
    ui->os5->blockSignals(true);

    ui->os1->setCurrentIndex(axisMode.mode.a1);
    ui->os2->setCurrentIndex(axisMode.mode.a2);
    ui->os3->setCurrentIndex(axisMode.mode.a3);
    ui->os4->setCurrentIndex(axisMode.mode.a4);
    ui->os5->setCurrentIndex(axisMode.mode.a5);

    ui->os1->blockSignals(false);
    ui->os2->blockSignals(false);
    ui->os3->blockSignals(false);
    ui->os4->blockSignals(false);
    ui->os5->blockSignals(false);

    ui->tryRunBox->setChecked(host->IsMidMoldCheck());
    ui->downModeBox->setChecked(host->IsCloseMoldEn());
    ui->safeInfoBox->setChecked(host->IsSafeInfoModeEn());
    ui->syncBox->setChecked(host->IsOrignSyncCheck());
    ui->fleeBox->setChecked(host->IsFleeEn());
    ui->originBox->setChecked(host->IsOriginModeEn());
    ui->autoBox->setChecked(host->IsAutoModeEn());
    ui->punchType->setCurrentIndex(host->PunchCheckMode());
    ui->waitMode->setCurrentIndex(host->IsWaitModeEn());

}

void ICStructDefineFrame::InitEnfoce()
{
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    for(int i=0;i < 32;i++){
        QString input = config->XInfo(i).GetLocaleName("zh");
        QString ouput = config->YInfo(i).GetLocaleName("zh");
        inputBoxs.at(i)->setText(input);
        ouputBoxs.at(i)->setText(ouput);
    }
    InitEnforceInput();
    InitEnforceOutput();

}

void ICStructDefineFrame::InitEnforceInput()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    quint32 inputs = host->SystemParameter(ICVirtualHost::Sys_EnforceInput0).toInt();
    inputs |= ( host->SystemParameter(ICVirtualHost::Sys_EnforceInput1).toInt() << 16);
    for(int i =0;i < inputBoxs.size();i++){
        inputBoxs.at(i)->setChecked( inputs & ( 1 << i));
    }
}

void ICStructDefineFrame::InitEnforceOutput()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    quint32 outputs = host->SystemParameter(ICVirtualHost::Sys_EnforceOutput0).toInt();
    outputs |= ( host->SystemParameter(ICVirtualHost::Sys_EnforceOutput1).toInt() << 16);
    for(int i =0;i < ouputBoxs.size();i++){
        ouputBoxs.at(i)->setChecked( outputs & ( 1 << i));
    }
}

quint32 ICStructDefineFrame::enforceInputs()
{
    qint32 rs = 0;
    foreach(QCheckBox *box,inputBoxs){
        int index = inputBoxs.indexOf(box);
        int v = box->isChecked();
        rs  |= ( v << index);
    }
    return rs;
}

quint32 ICStructDefineFrame::enforceOutputs()
{
    qint32 rs = 0;
    foreach(QCheckBox *box,ouputBoxs){
        int index = ouputBoxs.indexOf(box);
        int v = box->isChecked();
        rs  |= ( v << index);
    }
    return rs;
}

//void ICStructDefineFrame::on_adjUse_toggled(bool checked)
//{
//    ICParametersSave::Instance()->SetAdjustFunction(checked);
//}

void ICStructDefineFrame::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->currentWidget() == ui->origin)
    {
        timerID_ = startTimer(50);
    }
    else
    {
        if(timerID_ >=0)
        {
            killTimer(timerID_);
        }
    }
}

void ICStructDefineFrame::on_oStartBtn_clicked()
{
    icMainFrame->BlockOrignShow(true);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnZero, 0);
}

void ICStructDefineFrame::on_oX1Btn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_ORIGIN_X1);
}

void ICStructDefineFrame::on_oY1Btn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_ORIGIN_Y1);
}

void ICStructDefineFrame::on_oZBtn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_ORIGIN_Z);
}

void ICStructDefineFrame::on_oX2Btn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_ORIGIN_X2);
}

void ICStructDefineFrame::on_oY2Btn_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_ORIGIN_Y2);
}

void ICStructDefineFrame::on_setOrigin_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_SET_ORIGIN);
}



ICLogFunctions(ICStructDefineFrame)
