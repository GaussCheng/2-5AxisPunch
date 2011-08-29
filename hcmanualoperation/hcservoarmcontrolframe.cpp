#include "hcservoarmcontrolframe.h"
#include "ui_hcservoarmcontrolframe.h"
#include "ickeyboard.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"

HCServoArmControlFrame::HCServoArmControlFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCServoArmControlFrame)
{
    ui->setupUi(this);
}

HCServoArmControlFrame::~HCServoArmControlFrame()
{
    delete ui;
}

void HCServoArmControlFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
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

void HCServoArmControlFrame::SetCurrentAxis(const QString & axisName)
{
    ui->currentServoArmLabel->setText(axisName);
}

void HCServoArmControlFrame::RunMinus_()
{
    int key;
    const QString currentAxis = ui->currentServoArmLabel->text();
    if(currentAxis == tr("X"))
    {
        key = ICKeyboard::VFB_X1Sub;
    }
    else if(currentAxis == tr("Y"))
    {
        key = ICKeyboard::VFB_Y1Sub;
    }
    else if(currentAxis == tr("Z"))
    {
        key = ICKeyboard::VFB_ZSub;
    }
    ICKeyboard::Instace()->SetKeyValue(key);
    ICKeyboard::Instace()->SetPressed(true);
}

void HCServoArmControlFrame::RunPlus_()
{
    int key;
    const QString currentAxis = ui->currentServoArmLabel->text();
    if(currentAxis == tr("X"))
    {
        key = ICKeyboard::VFB_X1Add;
    }
    else if(currentAxis == tr("Y"))
    {
        key = ICKeyboard::VFB_Y1Add;
    }
    else if(currentAxis == tr("Z"))
    {
        key = ICKeyboard::VFB_ZAdd;
    }
    ICKeyboard::Instace()->SetKeyValue(key);
    ICKeyboard::Instace()->SetPressed(true);
}

void HCServoArmControlFrame::ChangeSpeed_(int dSpeed)
{

    int key = (dSpeed < 0)? IC::VKEY_DOWN : IC::VKEY_UP;
    const int times = qAbs(dSpeed) >> 1;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    for(int i = 0; i != times; ++i)
    {
        processor->ExecuteVirtualKeyCommand(key);
    }
}

void HCServoArmControlFrame::on_minusNormalButton_pressed()
{
    RunMinus_();
}

void HCServoArmControlFrame::on_minusNormalButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
}

void HCServoArmControlFrame::on_minusFastButton_pressed()
{
    ChangeSpeed_(10);
    RunMinus_();
}

void HCServoArmControlFrame::on_minusFastButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
    ChangeSpeed_(-10);
}

void HCServoArmControlFrame::on_minusHighSpeedButton_pressed()
{
    ChangeSpeed_(20);
    RunMinus_();
}

void HCServoArmControlFrame::on_minusHighSpeedButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
    ChangeSpeed_(-20);
}

void HCServoArmControlFrame::on_plusNormalButton_pressed()
{
    RunPlus_();
}

void HCServoArmControlFrame::on_plusNormalButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
}

void HCServoArmControlFrame::on_plusFastButton_pressed()
{
    ChangeSpeed_(10);
    RunPlus_();
}

void HCServoArmControlFrame::on_plusFastButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
    ChangeSpeed_(-10);
}

void HCServoArmControlFrame::on_plusHighSpeedButton_pressed()
{
    ChangeSpeed_(20);
    RunPlus_();
}

void HCServoArmControlFrame::on_plusHighSpeedButton_released()
{
    ICKeyboard::Instace()->SetPressed(false);
    ChangeSpeed_(-20);
}
