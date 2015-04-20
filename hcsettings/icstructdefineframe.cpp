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

typedef union {
     struct {
        u_int16_t xLimit : 2;
        u_int16_t yLimit : 2;
        u_int16_t sLimit : 2;
        u_int16_t rLimit : 2;
        u_int16_t tLimit : 2;
        u_int16_t rev : 6;
    }b;
    u_int16_t all;
}AxisLimitConfig;

typedef union{
    struct {
        u_int16_t x : 2;
        u_int16_t y : 2;
        u_int16_t z : 2;
        u_int16_t p : 2;
        u_int16_t q : 2;
        u_int16_t a : 2;
        u_int16_t b : 2;
        u_int16_t c : 2;
    }b;
    u_int16_t combine;
}OriginStatus;

typedef union{
    struct {
        u_int16_t a1 : 3;
        u_int16_t a2 : 3;
        u_int16_t a3 : 3;
        u_int16_t a4 : 3;
        u_int16_t a5 : 3;
        u_int16_t resv : 1;
    }mode;
    u_int16_t allMode;
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


//    outputDefineToNumber_.insert(ui->outABox, 0);
//    outputDefineToNumber_.insert(ui->outBBox, 1);
//    outputDefineToNumber_.insert(ui->outCBox, 2);
//    outputDefineToNumber_.insert(ui->outDBox, 3);
//    outputDefineToNumber_.insert(ui->outEBox, 4);
//    outputDefineToNumber_.insert(ui->outFBox, 5);
//    outputDefineToNumber_.insert(ui->outGBox, 6);
//    outputDefineToNumber_.insert(ui->outHBox, 7);

//    numberToOutputDefine_.insert(0, ui->outABox);
//    numberToOutputDefine_.insert(1, ui->outBBox);
//    numberToOutputDefine_.insert(2, ui->outCBox);
//    numberToOutputDefine_.insert(3, ui->outDBox);
//    numberToOutputDefine_.insert(4, ui->outEBox);
//    numberToOutputDefine_.insert(5, ui->outFBox);
//    numberToOutputDefine_.insert(6, ui->outGBox);
//    numberToOutputDefine_.insert(7, ui->outHBox);

//    ui->fixtureSelectBox->setCurrentIndex(host->FixtureDefine());
  //  ui->escapeComboBox->setCurrentIndex(host->EscapeWay());

    punchButtons_<<ui->punch1<<ui->punch2<<ui->punch3<<ui->punch4<<ui->punch5<<ui->punch6
                   <<ui->punch7<<ui->punch8<<ui->punch9<<ui->punch10;

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
    ui->originSpd->setValidator(new QIntValidator(0, 10, this));
    ui->originSpd->SetThisIntToThisText(host->SystemParameter(ICVirtualHost::SYS_OriginSpeed).toInt());
    ui->tryRunBox->setChecked(host->IsMidMoldCheck());

    ui->tabWidget->removeTab(1);
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
    if(ui->tabWidget->currentIndex() != 3) return;
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
        host->SetMidMoldCheck(ui->tryRunBox->isChecked());
        //        host->SystemParameter(ICVirtualHost::SYS_Function);
        host->SaveSystemConfig();
        QMessageBox::information(this, tr("Tips"), tr("Save Sucessfully!"));
        emit StructChanged();
        icMainFrame->UpdateAxisDefine_();
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

}

//void ICStructDefineFrame::on_adjUse_toggled(bool checked)
//{
//    ICParametersSave::Instance()->SetAdjustFunction(checked);
//}

void ICStructDefineFrame::on_tabWidget_currentChanged(int index)
{
    if(index == 3)
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

