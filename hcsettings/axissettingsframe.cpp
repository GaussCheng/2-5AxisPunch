#include <QMessageBox>
#include "axissettingsframe.h"
#include "ui_axissettingsframe.h"

#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"

AxisSettingsFrame::AxisSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AxisSettingsFrame),
    currentAxis_(-1)
{
    ui->setupUi(this);
    ui->label_11->hide();
    ui->label_8->hide();
    ui->distanceRotationEdit->hide();
    ui->label->hide();
    ui->label_3->hide();
    ui->mechanicalLengthLineEdit->hide();

    InitInterface();
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(LevelChanged(int)));
//    if(ICProgramHeadFrame::Instance()->CurrentLevel() == ICParametersSave::AdvanceAdmin)
//    {
//        LevelChanged(ICParametersSave::AdvanceAdmin);
//    }
    LevelChanged(ICProgramHeadFrame::Instance()->CurrentLevel());
    connect(&refreshTimer_,
            SIGNAL(timeout()),
            SLOT(StatusRefresh()));
}

AxisSettingsFrame::~AxisSettingsFrame()
{
    delete ui;
}

void AxisSettingsFrame::hideEvent(QHideEvent *e)
{
    refreshTimer_.stop();
    QFrame::hideEvent(e);
}

void AxisSettingsFrame::showEvent(QShowEvent *e)
{
    refreshTimer_.start(20);
    QFrame::showEvent(e);
}

void AxisSettingsFrame::changeEvent(QEvent *e)
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

void AxisSettingsFrame::SetCurrentAxis(QString currentAxisName, int axis)
{
    Q_UNUSED(currentAxisName)
    currentAxis_ = axis;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    QString minText;
    QString maxText;

    double total = 0;
    ui->minUnitLabel->setText(tr("mm"));
    ui->maxUnitLabel->setText(tr("mm"));
    minSecValidator_->setBottom(0);
    maxSecValidator_->setBottom(0);
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("X");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");
        ui->label_2->show();
        ui->label_4->show();
        ui->maximumDisplacementLineEdit->show();

    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Y");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
        ui->label_2->show();
        ui->label_4->show();
        ui->maximumDisplacementLineEdit->show();
        minSecValidator_->setBottom(10);
        maxSecValidator_->setBottom(500);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        machineLangth = ICVirtualHost::SYS_Z_Length;
        maxLangth = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Z");
        minText = tr("Internal security zone");
        maxText = tr("External security zone");
        ui->label_2->show();
        ui->label_4->show();
        ui->maximumDisplacementLineEdit->show();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        machineLangth = ICVirtualHost::SYS_P_Length;
        maxLangth = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("P");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");
        ui->label_2->show();
        ui->label_4->show();
        ui->maximumDisplacementLineEdit->show();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        machineLangth = ICVirtualHost::SYS_Q_Length;
        maxLangth = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Q");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
        ui->label_2->show();
        ui->label_4->show();
        ui->maximumDisplacementLineEdit->show();
        minSecValidator_->setBottom(10);
        maxSecValidator_->setBottom(500);
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        machineLangth = ICVirtualHost::SYS_A_Length;
        maxLangth = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("A");
//        minText = tr("Internal security zone");
//        maxText = tr("External security zone");
        minText = tr("Transeve security zone(Less)");
        maxText = tr("Transeve security zone(Lagger)");
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_2->hide();
        ui->label_4->hide();
        ui->maximumDisplacementLineEdit->hide();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        machineLangth = ICVirtualHost::SYS_B_Length;
        maxLangth = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("B");
//        minText = tr("Internal security zone");
//        maxText = tr("External security zone");
        minText = tr("Transeve security zone(Less)");
        maxText = tr("Transeve security zone(Lagger)");
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_2->hide();
        ui->label_4->hide();
        ui->maximumDisplacementLineEdit->hide();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        machineLangth = ICVirtualHost::SYS_C_Length;
        maxLangth = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("C");
        minText = tr("Transeve security zone(Less)");
        maxText = tr("Transeve security zone(Lagger)");
        ui->minUnitLabel->setText(tr("degree"));
        ui->maxUnitLabel->setText(tr("degree"));
        ui->label_2->hide();
        ui->label_4->hide();
        ui->maximumDisplacementLineEdit->hide();
    }
    else
    {
        return;
    }
    ui->mechanicalLengthLineEdit->SetThisIntToThisText(host->SystemParameter(machineLangth).toInt());
    ui->maximumDisplacementLineEdit->SetThisIntToThisText(host->SystemParameter(maxLangth).toInt());
    ui->internalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(iSafe).toInt());
    ui->externalSecurityZoneLineEdit->SetThisIntToThisText(host->SystemParameter(oSafe).toInt());
    QString format = QString("%.%1f").arg(ui->distanceRotationEdit->DecimalPlaces());
    ui->distanceRotationEdit->setText(QString().sprintf(format.toAscii(), total));
    ui->minLabel->setText(minText);
    ui->maxLabel->setText(maxText);
    maxMoveValidator_->setTop(ui->mechanicalLengthLineEdit->TransThisTextToThisInt());
    maxSecValidator_->setTop(maxMoveValidator_->top());
    minSecValidator_->setTop(maxMoveValidator_->top());
//    ui->maximumDisplacementLineEdit->setValidator();
}

void AxisSettingsFrame::InitInterface()
{
    QIntValidator * intValidator = new QIntValidator(0, 65530, this);
    maxSecValidator_ = new QIntValidator(0, 0, this);
    minSecValidator_ = new QIntValidator(0, 0, this);
    ui->mechanicalLengthLineEdit->SetDecimalPlaces(1);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
    ui->maximumDisplacementLineEdit->SetDecimalPlaces(1);
    maxMoveValidator_ = new QIntValidator(0, 65530, this);
    ui->maximumDisplacementLineEdit->setValidator(maxMoveValidator_);
    ui->internalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->internalSecurityZoneLineEdit->setValidator(minSecValidator_);
    ui->externalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->externalSecurityZoneLineEdit->setValidator(maxSecValidator_);
    ui->distanceRotationEdit->SetDecimalPlaces(2);
    ui->distanceRotationEdit->setValidator(intValidator);
}

QList<uint> AxisSettingsFrame::GetCurrentStatus_() const
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

bool AxisSettingsFrame::SetCurrentStatus_(const QList<uint> &status)
{
    Q_ASSERT_X(status.size() == 7, "AxisSettingsFrame::SetCurrentStatus", "status'size is less than 7!");
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
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
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
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
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
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
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
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        machineLangth = ICVirtualHost::SYS_P_Length;
        maxLangth = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        tH = ICVirtualHost::SYS_P_TotalH;
        tL = ICVirtualHost::SYS_P_TotalL;
        sum = ICVirtualHost::SYS_P_XorSum;
        axisName = "P";
        axis = 3;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        machineLangth = ICVirtualHost::SYS_Q_Length;
        maxLangth = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        tH = ICVirtualHost::SYS_Q_TotalH;
        tL = ICVirtualHost::SYS_Q_TotalL;
        sum = ICVirtualHost::SYS_Q_XorSum;
        axisName = "Q";
        axis = 4;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        machineLangth = ICVirtualHost::SYS_A_Length;
        maxLangth = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        tH = ICVirtualHost::SYS_A_TotalH;
        tL = ICVirtualHost::SYS_A_TotalL;
        sum = ICVirtualHost::SYS_A_XorSum;
        axisName = "A";
        axis = 5;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        machineLangth = ICVirtualHost::SYS_B_Length;
        maxLangth = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        tH = ICVirtualHost::SYS_B_TotalH;
        tL = ICVirtualHost::SYS_B_TotalL;
        sum = ICVirtualHost::SYS_B_XorSum;
        axisName = "B";
        axis = 6;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        machineLangth = ICVirtualHost::SYS_C_Length;
        maxLangth = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        tH = ICVirtualHost::SYS_C_TotalH;
        tL = ICVirtualHost::SYS_C_TotalL;
        sum = ICVirtualHost::SYS_C_XorSum;
        axisName = "C";
        axis = 7;
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

void AxisSettingsFrame::on_saveToolButton_clicked()
{
    QList<uint> status;
    status = GetCurrentStatus_();
    if(SetCurrentStatus_(status))
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        host->SaveAxisParam(currentAxis_);
//        host->ReConfigure();
        QMessageBox::information(this, tr("Information"), tr("Save Successfully!"));
    }
}

void AxisSettingsFrame::StatusRefresh()
{
    QString pos;
    QString feedbackPos;
    QString zSignal;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        pos = host->HostStatus(ICVirtualHost::XPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgX0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgX1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        pos = host->HostStatus(ICVirtualHost::YPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgY0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgY1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        pos = host->HostStatus(ICVirtualHost::ZPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgZ0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgZ1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        pos = host->HostStatus(ICVirtualHost::PPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgP0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgP1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        pos = host->HostStatus(ICVirtualHost::QPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgQ0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgQ1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        pos = host->HostStatus(ICVirtualHost::APos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgA0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgA1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        pos = host->HostStatus(ICVirtualHost::BPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgB0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgB1).toString();
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        pos = host->HostStatus(ICVirtualHost::CPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgC0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgC1).toString();
    }

    ui->testLineEdit->setText(pos);
    ui->feedbackEdit->setText(feedbackPos);
    ui->zSignalEdit->setText(zSignal);
}

void AxisSettingsFrame::on_testPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        addCmd = IC::CMD_TestX;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        addCmd = IC::CMD_TestY;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        addCmd = IC::CMD_TestZ;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        addCmd = IC::CMD_TestX2;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        addCmd = IC::CMD_TestY2;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        addCmd = IC::CMD_TestA;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        addCmd = IC::CMD_TestB;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        addCmd = IC::CMD_TestC;
    }
    processor->ExecuteHCCommand(addCmd, 0);

}


void AxisSettingsFrame::on_revTestPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == ICVirtualHost::ICAxis_AxisX1)
    {
        addCmd = IC::CMD_TestxRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY1)
    {
        addCmd = IC::CMD_TestyRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisZ)
    {
        addCmd = IC::CMD_TestzRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisX2)
    {
        addCmd = IC::CMD_TestX2Rev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisY2)
    {
        addCmd = IC::CMD_TestY2Rev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisA)
    {
        addCmd = IC::CMD_TestARev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisB)
    {
        addCmd = IC::CMD_TestBRev;
    }
    else if(currentAxis_ == ICVirtualHost::ICAxis_AxisC)
    {
        addCmd = IC::CMD_TestCRev;
    }
    else
    {
        return;
    }
    processor->ExecuteHCCommand(addCmd, 0);
}

void AxisSettingsFrame::on_pushButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TestClr, 0);
}

void AxisSettingsFrame::LevelChanged(int level)
{
    if(level >=  ICParametersSave::MachineAdmin)
    {
        if(level >= ICParametersSave::AdvanceAdmin)
        {
            ui->mechanicalLengthLineEdit->setEnabled(true);
        }
        else
        {
            ui->mechanicalLengthLineEdit->setEnabled(false);
        }
        ui->securityPointGroupBox->setEnabled(true);
        ui->saveToolButton->setEnabled(true);
    }
    else
    {
        ui->securityPointGroupBox->setEnabled(false);
        ui->saveToolButton->setEnabled(false);
    }
}
