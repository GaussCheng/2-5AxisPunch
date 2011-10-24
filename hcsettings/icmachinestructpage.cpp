#include "icmachinestructpage.h"
#include "ui_icmachinestructpage.h"

#include <QButtonGroup>
#include <QMessageBox>

#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icstructdefineframe.h"
#include "ichctimeframe.h"

ICMachineStructPage::ICMachineStructPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineStructPage),
    structPage_(NULL),
    timePage_(NULL)
{
    ui->setupUi(this);
    InitInterface();
    buttonGroup_ = new QButtonGroup();

    ui->axisXToolButton->setText(tr("X Axis"));
    ui->axisYToolButton->setText(tr("Y Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
    ui->structDefButton->setText(tr("Struct Define"));
    ui->timeLimitButton->setText(tr("Time"));
    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->structDefButton);
    buttonGroup_->addButton(ui->timeLimitButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
    ui->axisXToolButton->click();
}

ICMachineStructPage::~ICMachineStructPage()
{
    delete buttonGroup_;
    delete ui;
}

void ICMachineStructPage::changeEvent(QEvent *e)
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

void ICMachineStructPage::hideEvent(QHideEvent *e)
{
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    QWidget::hideEvent(e);
}

void ICMachineStructPage::on_axisXToolButton_clicked()
{
    SetCurrentAxis(AxisX);
}

void ICMachineStructPage::on_axisYToolButton_clicked()
{
    SetCurrentAxis(AxisY);
}

void ICMachineStructPage::on_axisZToolButton_clicked()
{
    SetCurrentAxis(AxisZ);
}

void ICMachineStructPage::on_structDefButton_clicked()
{
    if(structPage_ == NULL)
    {
        structPage_ = new ICStructDefineFrame();
        ui->content->addWidget(structPage_);
    }
    ui->content->setCurrentWidget(structPage_);
}

void ICMachineStructPage::on_timeLimitButton_clicked()
{
    if(timePage_ == NULL)
    {
        timePage_ = new ICHCTimeFrame();
        ui->content->addWidget(timePage_);
    }
    ui->content->setCurrentWidget(timePage_);
}

void ICMachineStructPage::SetCurrentAxis(AxisSelect axis)
{
    ui->content->setCurrentIndex(0);
    currentAxis_ = axis;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    QString minText;
    QString maxText;

    double total = 0;
    if(currentAxis_ == AxisX)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("X");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");

    }
    else if(currentAxis_ == AxisY)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Y");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
    }
    else if(currentAxis_ == AxisZ)
    {
        machineLangth = ICVirtualHost::SYS_Z_Length;
        maxLangth = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Z");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
    }
    else
    {
        return;
    }
    ui->mechanicalLengthLineEdit->SetThisIntToThisText(host->SystemParameter(machineLangth).toInt());
    ui->maximumDisplacementLineEdit->SetThisIntToThisText(host->SystemParameter(maxLangth).toInt());
    ui->internalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(iSafe).toInt());
    ui->externalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(oSafe).toInt());
    ui->distanceRotationEdit->setText(QString().sprintf("%.2f", total));
    ui->minLabel->setText(minText);
    ui->maxLabel->setText(maxText);
    maxMoveValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
}

QList<uint> ICMachineStructPage::GetCurrentStatus_() const
{
    QList<uint> ret;
    double machineLength = ui->mechanicalLengthLineEdit->text().toDouble();
    ret.append(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    ret.append(ui->maximumDisplacementLineEdit->TransThisTextToThisInt());
    ret.append(ui->internalSecurityZoneLineEdit->TransThisTextToThisInt());
    ret.append(ui->externalSecurityZoneLineEdit->TransThisTextToThisInt());
    uint totalH;
    uint totalL;
    uint total = machineLength * 10000 / ui->distanceRotationEdit->text().toDouble();
    totalH = (total >> 16) & 0XFFFF;
    totalL = total & 0XFFFF;
    ret.append(totalL);
    ret.append(totalH);
    int sum = 0;
    for(int i = 0; i != ret.size(); ++i)
    {
        sum += ret.at(i);
    }
    sum = (-sum & 0xFFFF);
    ret.append(sum);
    return ret;
    //    ret.append();
}

bool ICMachineStructPage::SetCurrentStatus_(const QList<uint> &status)
{
    Q_ASSERT_X(status.size() == 7, "ICMachineStructPage::SetCurrentStatus", "status'size is less than 7!");
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    ICVirtualHost::ICSystemParameter tH;
    ICVirtualHost::ICSystemParameter tL;
    ICVirtualHost::ICSystemParameter sum;
    QString axisName;
    int axis;
    if(currentAxis_ == AxisX)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        tH = ICVirtualHost::SYS_X_TotalH;
        tL = ICVirtualHost::SYS_X_TotalL;
        sum = ICVirtualHost::SYS_X_XorSum;
        axisName = "X";
        axis = 0;
    }
    else if(currentAxis_ == AxisY)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        tH = ICVirtualHost::SYS_Y_TotalH;
        tL = ICVirtualHost::SYS_Y_TotalL;
        sum = ICVirtualHost::SYS_Y_XorSum;
        axisName = "Y";
        axis = 1;
    }
    else if(currentAxis_ == AxisZ)
    {
        machineLangth = ICVirtualHost::SYS_Z_Length;
        maxLangth = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        tH = ICVirtualHost::SYS_Z_TotalH;
        tL = ICVirtualHost::SYS_Z_TotalL;
        sum = ICVirtualHost::SYS_Z_XorSum;
        axisName = "Z";
        axis = 2;
    }
    else
    {
        return false;
    }
    ICSetAxisConfigsCommand command;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    command.SetSlave(processor->SlaveID());
    command.SetAxis(axis);
    command.SetDataBuffer(status.toVector());
    if(processor->ExecuteCommand(command).toBool())
    {
        host->SetSystemParameter(machineLangth, status.at(0));
        host->SetSystemParameter(maxLangth, status.at(1));
        host->SetSystemParameter(iSafe, status.at(2));
        host->SetSystemParameter(oSafe, status.at(3));
        host->SetSystemParameter(tL, status.at(4));
        host->SetSystemParameter(tH, status.at(5));
        host->SetSystemParameter(sum, status.at(6));
        ICParametersSave::Instance()->SetDistanceRotation(axisName, ui->distanceRotationEdit->text().toDouble());
        qDebug()<<ICParametersSave::Instance()->DistanceRotation(axisName);
        return true;
    }
    return false;
}

void ICMachineStructPage::on_saveToolButton_clicked()
{
    QList<uint> status;
    status = GetCurrentStatus_();
    if(SetCurrentStatus_(status))
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        host->SaveAxisParam();
//        host->ReConfigure();
        QMessageBox::information(this, tr("Information"), tr("Save Successfully!"));
    }
}

void ICMachineStructPage::InitInterface()
{
    QIntValidator * intValidator = new QIntValidator(0, 32767, this);
    ui->mechanicalLengthLineEdit->SetDecimalPlaces(1);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
    ui->maximumDisplacementLineEdit->SetDecimalPlaces(1);
    maxMoveValidator_ = new QIntValidator(0, 32767, this);
    ui->maximumDisplacementLineEdit->setValidator(maxMoveValidator_);
    ui->internalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->internalSecurityZoneLineEdit->setValidator(intValidator);
    ui->externalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->externalSecurityZoneLineEdit->setValidator(intValidator);
    ui->distanceRotationEdit->SetDecimalPlaces(2);
    ui->distanceRotationEdit->setValidator(intValidator);
}
