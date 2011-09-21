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
    step_(0),
    currentAxis_(Hide),
    isStart_(true)
{
    ui->setupUi(this);

    InitInterface();
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(LevelChanged(int)));
    ui->currentAxisLabel->hide();
    ui->minPositionToolButton->hide();
    ui->maxPositionToolButton->hide();
    if(ICProgramHeadFrame::Instance()->CurrentLevel() == ICParametersSave::AdvanceAdmin)
    {
        LevelChanged(ICParametersSave::AdvanceAdmin);
    }
}

AxisSettingsFrame::~AxisSettingsFrame()
{
    delete ui;
}

void AxisSettingsFrame::hideEvent(QHideEvent *e)
{
    qDebug("Axis Setting hide");
    //    currentAxis_ = Hide;
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefresh()));
    QFrame::hideEvent(e);
}

void AxisSettingsFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    step_ = 0;
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefresh()));
}

void AxisSettingsFrame::SetCurrentAxis(QString currentAxisName, int axis)
{
    //    if(currentAxis_ != Hide)
    //    {
    //        QList<uint> status = GetCurrentStatus_();
    //        SetCurrentStatus_(status);
    //    }
    isStart_ = false;
    step_ = -1;
    ui->minPositionToolButton->setEnabled(true);
    ui->maxPositionToolButton->setEnabled(false);
    ui->currentAxisLabel->setText(currentAxisName);
    currentAxis_ = axis;
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICVirtualHost::ICSystemParameter machineLangth;
    ICVirtualHost::ICSystemParameter maxLangth;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    QString minText;
    QString maxText;

    double total = 0;
    if(currentAxis_ == XAxis)
    {
        machineLangth = ICVirtualHost::SYS_X_Length;
        maxLangth = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("X");
        minText = tr("Min pos inside mold");
        maxText = tr("Max pos inside mold");

    }
    else if(currentAxis_ == YAxis)
    {
        machineLangth = ICVirtualHost::SYS_Y_Length;
        maxLangth = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        total = ICParametersSave::Instance()->DistanceRotation("Y");
        minText = tr("Max standby pos");
        maxText = tr("Leave origin pos");
    }
    else if(currentAxis_ == ZAxis)
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
}

void AxisSettingsFrame::InitInterface()
{
    QIntValidator * intValidator = new QIntValidator(0, 32767, this);
    ui->mechanicalLengthLineEdit->SetDecimalPlaces(1);
    ui->mechanicalLengthLineEdit->setValidator(intValidator);
    ui->maximumDisplacementLineEdit->SetDecimalPlaces(1);
    ui->maximumDisplacementLineEdit->setValidator(intValidator);
    ui->internalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->internalSecurityZoneLineEdit->setValidator(intValidator);
    ui->externalSecurityZoneLineEdit->SetDecimalPlaces(1);
    ui->externalSecurityZoneLineEdit->setValidator(intValidator);
    ui->distanceRotationEdit->SetDecimalPlaces(2);
    ui->distanceRotationEdit->setValidator(intValidator);
}

QList<uint> AxisSettingsFrame::GetCurrentStatus_(bool isGuild) const
{
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
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
    //    if(isGuild)
    //    {
    //        if(currentAxis_ == XAxis)
    //        {
    //            totalH = host->HostStatus(ICVirtualHost::DbgX1).toUInt() & 0xFFFF;
    //            totalL = host->HostStatus(ICVirtualHost::DbgX0).toUInt() & 0xFFFF;
    //        }
    //        else if(currentAxis_ == YAxis)
    //        {
    //            totalH = host->HostStatus(ICVirtualHost::DbgY1).toUInt() & 0xFFFF;
    //            totalL = host->HostStatus(ICVirtualHost::DbgY0).toUInt() & 0xFFFF;
    //        }
    //        else if(currentAxis_ == ZAxis)
    //        {
    //            totalH = host->HostStatus(ICVirtualHost::DbgZ1).toUInt() & 0xFFFF;
    //            totalL = host->HostStatus(ICVirtualHost::DbgZ0).toUInt() & 0xFFFF;
    //        }
    //    }
    //    else
    //    {
    ////        host->SetSystemParameter(ICVirtualHost::SYS_Z_TotalL, );
    //        if(currentAxis_ == XAxis)
    //        {
    //            totalH = host->SystemParameter(ICVirtualHost::SYS_X_TotalH).toUInt() & 0xFFFF;
    //            totalL = host->SystemParameter(ICVirtualHost::SYS_X_TotalL).toUInt() & 0xFFFF;
    //        }
    //        else if(currentAxis_ == YAxis)
    //        {
    //            totalH = host->SystemParameter(ICVirtualHost::SYS_Y_TotalH).toUInt() & 0xFFFF;
    //            totalL = host->SystemParameter(ICVirtualHost::SYS_Y_TotalL).toUInt() & 0xFFFF;
    //        }
    //        else if(currentAxis_ == ZAxis)
    //        {
    //            totalH = host->SystemParameter(ICVirtualHost::SYS_Z_TotalH).toUInt() & 0xFFFF;
    //            totalL = host->SystemParameter(ICVirtualHost::SYS_Z_TotalL).toUInt() & 0xFFFF;
    //        }
    //    }
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
    if(currentAxis_ == XAxis)
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
    else if(currentAxis_ == YAxis)
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
    else if(currentAxis_ == ZAxis)
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

void AxisSettingsFrame::on_minPositionToolButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int subCmd;
    if(currentAxis_ == XAxis)
    {
        subCmd = IC::CMD_X1SubLmt;
    }
    else if(currentAxis_ == YAxis)
    {
        subCmd = IC::CMD_Y1SubLmt;
    }
    else if(currentAxis_ == ZAxis)
    {
        subCmd = IC::CMD_ZSubLmt;
    }
    processor->ExecuteHCCommand(subCmd, 0);

}

void AxisSettingsFrame::on_maxPositionToolButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == XAxis)
    {
        addCmd = IC::CMD_X1AddLmt;
    }
    else if(currentAxis_ == YAxis)
    {
        addCmd = IC::CMD_Y1AddLmt;
    }
    else if(currentAxis_ == ZAxis)
    {
        addCmd = IC::CMD_ZAddLmt;
    }
    processor->ExecuteHCCommand(addCmd, 0);
}

void AxisSettingsFrame::on_saveToolButton_clicked()
{
    QList<uint> status;
    if(isStart_ && step_ == 4)
    {
        status = GetCurrentStatus_(true);
    }
    else
    {
        status = GetCurrentStatus_(false);
    }
    if(SetCurrentStatus_(status))
    {
        ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
        host->SaveAxisParam();
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
    if(currentAxis_ == XAxis)
    {
        pos = host->HostStatus(ICVirtualHost::XPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgX0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgX1).toString();
    }
    else if(currentAxis_ == YAxis)
    {
        pos = host->HostStatus(ICVirtualHost::YPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgY0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgY1).toString();
    }
    else if(currentAxis_ == ZAxis)
    {
        pos = host->HostStatus(ICVirtualHost::ZPos).toString();
        feedbackPos = host->HostStatus(ICVirtualHost::DbgZ0).toString();
        zSignal = host->HostStatus(ICVirtualHost::DbgZ1).toString();
    }

    ui->testLineEdit->setText(pos);
    ui->feedbackEdit->setText(feedbackPos);
    ui->zSignalEdit->setText(zSignal);

    //    step_ = host->CurrentStep();
    //    if(!isStart_)
    //    {
    //        ui->minPositionToolButton->setEnabled(true);
    //        ui->maxPositionToolButton->setEnabled(false);
    //        isStart_ = true;
    //        ui->guildLabel->setText(tr("Please press Min button"));
    //    }
    //    else if(step_ == 1 )
    //    {
    //        ui->guildLabel->setText(tr("When alarmed ,please press stop button"));
    //    }
    //    else if(step_ == 2)
    //    {
    //        ui->guildLabel->setText(tr("Prease press Max button"));
    //        ui->minPositionToolButton->setEnabled(false);
    //        ui->maxPositionToolButton->setEnabled(true);
    //    }
    //    else if(step_ == 3)
    //    {
    //        ui->guildLabel->setText(tr("When alarmed, please press stop button"));
    //        ui->minPositionToolButton->setEnabled(true);
    //        ui->maxPositionToolButton->setEnabled(false);
    //    }
    //    else if(step_ == 4)
    //    {
    //        ui->guildLabel->setText(tr("Set the machine configure and save"));
    //        ui->minPositionToolButton->setEnabled(true);
    //        ui->maxPositionToolButton->setEnabled(false);
    //    }
}

void AxisSettingsFrame::on_testPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == XAxis)
    {
        addCmd = IC::CMD_TestX;
    }
    else if(currentAxis_ == YAxis)
    {
        addCmd = IC::CMD_TestY;
    }
    else if(currentAxis_ == ZAxis)
    {
        addCmd = IC::CMD_TestZ;
    }
    processor->ExecuteHCCommand(addCmd, 0);

}


void AxisSettingsFrame::on_revTestPushButton_clicked()
{
    ICCommandProcessor *processor = ICCommandProcessor::Instance();
    int addCmd;
    if(currentAxis_ == XAxis)
    {
        addCmd = IC::CMD_TestxRev;
    }
    else if(currentAxis_ == YAxis)
    {
        addCmd = IC::CMD_TestyRev;
    }
    else if(currentAxis_ == ZAxis)
    {
        addCmd = IC::CMD_TestzRev;
    }
    processor->ExecuteHCCommand(addCmd, 0);
}

void AxisSettingsFrame::on_pushButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TestClr, 0);
}

void AxisSettingsFrame::LevelChanged(int level)
{
    if(level == ICParametersSave::AdvanceAdmin)
    {
        ui->securityPointGroupBox->setEnabled(true);
        ui->saveToolButton->setEnabled(true);
    }
    else
    {
        ui->securityPointGroupBox->setEnabled(false);
        ui->saveToolButton->setEnabled(false);
    }
}
