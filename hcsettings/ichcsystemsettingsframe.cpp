#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include "ichcsystemsettingsframe.h"
#include "ui_ichcsystemsettingsframe.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"
#include "icadminpassworddialog.h"
#include "icvirtualhost.h"
#include "iccommands.h"
#include <QDebug>

ICHCSystemSettingsFrame::ICHCSystemSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCSystemSettingsFrame)
{
    ui->setupUi(this);
    buttonGroup_ = new QButtonGroup();

    InitParameter();
    ui->extentFunctionCheckBox->setChecked(ICParametersSave::Instance()->IsExtentFunctionUsed());

    ui->axisXToolButton->setText(tr("X Axis"));
    ui->axisYToolButton->setText(tr("Y Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
    ui->structDefButton->setText(tr("Struct Define"));
//    buttonGroup_->addButton(ui->basicSettingsToolButton);
    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->structDefButton);
    buttonGroup_->setExclusive(true);
    QAbstractButton* button;
    foreach(button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
    ui->axisXToolButton->click();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(OnLevelChanged(int)));
    armXStructValueToName_.insert(0, tr("None"));
    armXStructValueToName_.insert(1, tr("Only Backward Limit"));
    armXStructValueToName_.insert(2, tr("Only Forward Limit"));
    armXStructValueToName_.insert(3, tr("Both"));
    armYStructValueToName_.insert(1, tr("Only Up Limit"));
    armYStructValueToName_.insert(3, tr("Up and Donw Limit"));
}

ICHCSystemSettingsFrame::~ICHCSystemSettingsFrame()
{
    delete ui;
}

void ICHCSystemSettingsFrame::InitParameter()
{
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ICParametersSave* paraSave = ICParametersSave::Instance();
    int index = (paraSave->Country() == QLocale::China) ? 0 : 1;
    ui->languageComboBox->setCurrentIndex(index);
    if(paraSave->KeyTone())
    {
        ui->keyToneButton->setText(tr("Key Tone(ON)"));
    }
    else
    {
        ui->keyToneButton->setText(tr("Key Tone(OFF)"));
    }
}

void ICHCSystemSettingsFrame::on_languageComboBox_activated(int index)
{
    ICParametersSave* paraSave = ICParametersSave::Instance();
    if(index == 0)
    {
        paraSave->SetCountry(QLocale::China);
    }
    else if(index == 1)
    {
        paraSave->SetCountry(QLocale::UnitedStates);
    }
}

void ICHCSystemSettingsFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            ICParametersSave* paraSave = ICParametersSave::Instance();

            int index = (paraSave->Country() == QLocale::China) ? 0 : 1;
            ui->languageComboBox->setCurrentIndex(index);
            ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            ui->axisXToolButton->setText(tr("X Axis"));
            ui->axisYToolButton->setText(tr("Y Axis"));
            ui->axisZToolButton->setText(tr("Z Axis"));
            ui->structDefButton->setText(tr("Struct Define"));
        }
        break;
    default:
        break;
    }
}

void ICHCSystemSettingsFrame::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefresh()));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
////    if(ICProgramHeadFrame::Instance()->CurrentLevel() == ICParametersSave::AdvanceAdmin)
//    {
////        ui->adminSettingsPage->setEnabled(true);
////        ui->adminSettingsPage->show();
//    }
//    else
//    {
////        ui->adminSettingsPage->setEnabled(false);
////        ui->adminSettingsPage->hide();
//    }
    ui->extentLabel->setEnabled(false);
    ui->extentFunctionCheckBox->setEnabled(false);
    UpdateConfigShow_();
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    ui->hmiMachienLenghtLabel->setText(host->SystemParameter(ICVirtualHost::SYS_));

    QFrame::showEvent(e);
}

void ICHCSystemSettingsFrame::on_saveButton_clicked()
{
    QString dateTimeCmd = "date -s " + ui->dateTimeEdit->dateTime().toString("yyyy.MM.dd-hh:mm:ss");
    dateTimeCmd += " && hwclock -w";
    ::system(dateTimeCmd.toAscii());
}

void ICHCSystemSettingsFrame::on_keyToneButton_toggled(bool checked)
{
    if(checked)
    {
        ui->keyToneButton->setText(tr("Key Tone(ON)"));
    }
    else
    {
        ui->keyToneButton->setText(tr("Key Tone(OFF)"));
    }
    ICParametersSave::Instance()->SetKeyTone(checked);
}

void ICHCSystemSettingsFrame::on_changeButton_clicked()
{
    if(ui->oldPwdEdit->text().isEmpty() ||
            ui->newPwdEdit->text().isEmpty())
    {
        QMessageBox::information(this,
                                 tr("Information"),
                                 tr("Please enter the olp password and new password!"));
        return;
    }
    ICParametersSave* config = ICParametersSave::Instance();
    ICParametersSave::OperationLevel level;
    if(ui->levelComboBox->currentIndex() == 0)
    {
        level = ICParametersSave::MachineAdmin;
    }
    else
    {
        level = ICParametersSave::AdvanceAdmin;
    }
    int currentlevel = ICProgramHeadFrame::Instance()->CurrentLevel();

    if(currentlevel <= level && !config->VerifyPassword(level, ui->oldPwdEdit->text()))
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Old password is wrong!"));
        return;
    }
    ICParametersSave::Instance()->SetPassword(level, ui->newPwdEdit->text());
    ui->oldPwdEdit->clear();
    ui->newPwdEdit->clear();
    QMessageBox::information(this,
                             tr("Information"),
                             tr("Change password successfully!"));
}

void ICHCSystemSettingsFrame::on_extentFunctionCheckBox_toggled(bool checked)
{
//    if(passwordDialog_->exec() != QDialog::Accepted)
//    {
//        ui->extentFunctionCheckBox->setChecked(!checked);
//        return;
//    }
    ICParametersSave::Instance()->SetExtentFunctionUsed(checked);
}

void ICHCSystemSettingsFrame::on_verifyButton_clicked()
{
    if(ui->pwdEdit->text() == "szhc2a")
    {
        ui->extentLabel->setEnabled(true);
        ui->extentFunctionCheckBox->setEnabled(true);
    }
    ui->pwdEdit->clear();
}

bool ICHCSystemSettingsFrame::CheckIsUsbAttached() const
{
    QDir dir("/proc/scsi/usb-storage");
    return !dir.entryList().isEmpty();
}

void ICHCSystemSettingsFrame::on_backupMachineButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    system("mkdir -p /mnt/udisk/HC2ABackup/sysconfig");
//    bool isSuccess = QFile::copy("./sysconfig/paramx.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramx.txt");
//    isSuccess = isSuccess && QFile::copy("./sysconfig/paramy.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramy.txt");
//    isSuccess = isSuccess && QFile::copy("./sysconfig/paramz.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramz.txt");
    Information(system("cp /opt/Qt/bin/sysconfig/param*.txt /mnt/udisk/HC2ABackup/sysconfig/") >= 0 );
}

void ICHCSystemSettingsFrame::on_backupSystemButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    system("mkdir -p /mnt/udisk/HC2ABackup/sysconfig");
//    bool isSuccess = QFile::copy("./sysconfig/system.txt", "/mnt/udisk/HC2ABackup/sysconfig/system.txt");
    Information(system("cp /opt/Qt/bin/sysconfig/system.txt /mnt/udisk/HC2ABackup/sysconfig/system.txt") >= 0);
}

void ICHCSystemSettingsFrame::Information(bool isSuccess)
{
    if(isSuccess)
    {
        QMessageBox::information(this, tr("Information"), tr("Operation finished!"));
    }
    else
    {
        QMessageBox::warning(this, tr("Information"), tr("Operation fail!"));
    }
}

void ICHCSystemSettingsFrame::on_backupMoldsButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    system("mkdir -p /mnt/udisk/HC2ABackup/");
//    bool isSuccess = QFile::copy("./records", "/mnt/udisk/HC2ABackup/");
//    isSuccess = isSuccess && QFile::copy("./subs", "/mnt/udisk/HC2ABackup/");
    Information(system("cp -R /opt/Qt/bin/records/ /opt/Qt/bin/subs/ /mnt/udisk/HC2ABackup") >= 0);
}

void ICHCSystemSettingsFrame::on_backupAllButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    system("mkdir -p /mnt/udisk/HC2ABackup/sysconfig");
//    bool isSuccess = QFile::copy("./sysconfig/paramx.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramx.txt");
//    isSuccess = isSuccess && QFile::copy("./sysconfig/paramy.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramy.txt");
//    isSuccess = isSuccess && QFile::copy("./sysconfig/paramz.txt", "/mnt/udisk/HC2ABackup/sysconfig/paramz.txt");

//    isSuccess = isSuccess && QFile::copy("./sysconfig/system.txt", "/mnt/udisk/HC2ABackup/sysconfig/system.txt");

//    isSuccess = isSuccess && QFile::copy("./records", "/mnt/udisk/HC2ABackup/");
//    isSuccess = isSuccess && QFile::copy("./subs", "/mnt/udisk/HC2ABackup/");

    Information(system("cp /opt/Qt/bin/sysconfig/param*.txt /opt/Qt/bin/sysconfig/system.txt /mnt/udisk/HC2ABackup/sysconfig/ && cp -R /opt/Qt/bin/records/ /opt/Qt/bin/subs/ /mnt/udisk/HC2ABackup") >= 0);
}

void ICHCSystemSettingsFrame::on_restoreMachineButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    QDir dir("/mnt/udisk/HC2ABackup/sysconfig");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/sysconfig/param* /opt/Qt/bin/sysconfig -f");
    Information(true);
    system("reboot");
}

void ICHCSystemSettingsFrame::on_restoreSystemButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    QDir dir("/mnt/udisk/HC2ABackup/sysconfig");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/sysconfig/system.txt /opt/Qt/bin/sysconfig -f");
    Information(true);
    system("reboot");
}

void ICHCSystemSettingsFrame::on_restoreMoldsButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    QDir dir("/mnt/udisk/HC2ABackup/records");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/records/* /opt/Qt/bin/records -f");
    dir.setPath("/mnt/udisk/HC2ABackup/subs");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/subs/* /opt/Qt/bin/subs -f");
    Information(true);
    system("reboot");
}

void ICHCSystemSettingsFrame::on_restoreAllButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    QDir dir("/mnt/udisk/HC2ABackup/sysconfig");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/sysconfig/param* /opt/Qt/bin/sysconfig -f");

    dir.setPath("/mnt/udisk/HC2ABackup/sysconfig");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    system("cp /mnt/udisk/HC2ABackup/sysconfig/system.txt /opt/Qt/bin/sysconfig -f");
    on_restoreMoldsButton_clicked();

}

void ICHCSystemSettingsFrame::on_umountButton_clicked()
{
    system("umount /mnt/udisk");
}

void ICHCSystemSettingsFrame::OnLevelChanged(int level)
{
    if(level == ICParametersSave::AdvanceAdmin)
    {
        ui->hmiGroupBox->setEnabled(true);
        ui->hostGroupBox->setEnabled(true);
    }
    else
    {
        ui->hmiGroupBox->setEnabled(false);
        ui->hostGroupBox->setEnabled(false);
    }
}

void ICHCSystemSettingsFrame::on_axisXToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 0;
    SetConfig(ICVirtualHost::SYS_X_Length,
              ICVirtualHost::SYS_X_Maxium,
              ICVirtualHost::SYS_X_InSafe,
              ICVirtualHost::SYS_X_OutSafe,
              tr("Min pos inside mold"),
              tr("Max pos inside mold"),
              "X");
}

void ICHCSystemSettingsFrame::on_axisYToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 1;
    SetConfig(ICVirtualHost::SYS_Y_Length,
              ICVirtualHost::SYS_Y_Maxium,
              ICVirtualHost::SYS_Y_InSafe,
              ICVirtualHost::SYS_Y_OutSafe,
              tr("Max standby pos"),
              tr("Leave origin pos"),
              "Y");
}

void ICHCSystemSettingsFrame::on_axisZToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 2;
    SetConfig(ICVirtualHost::SYS_Z_Length,
              ICVirtualHost::SYS_Z_Maxium,
              ICVirtualHost::SYS_Z_InSafe,
              ICVirtualHost::SYS_Z_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "Z");
}

void ICHCSystemSettingsFrame::on_structDefButton_clicked()
{
    ui->hostGroupBox->hide();
    ui->hmiGroupBox->hide();
    ui->selectHostButton->hide();
    ui->hostStructDefGroup->show();
    ui->hmiStructDefGroup->show();
    ui->structSelectHostButton->show();
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    armStruct_ = host->SystemParameter(ICVirtualHost::SYS_ARM_CONFIG).toUInt();
    ui->hmiX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
    ui->hmiY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
    ui->hmiX2->setText(armXStructValueToName_.value((armStruct_ & 0x0030) >> 4));
    ui->hmiY2->setText(armYStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
    ICGetAxisConfigsCommand command;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    command.SetAxis(3);
    command.SetSlave(processor->SlaveID());
    ICCommunicationCommandBase::ResultVector ret = processor->ExecuteCommand(command).value<ICCommunicationCommandBase::ResultVector>();
    if(!ret.isEmpty())
    {
        armStruct_ = ret.at(0);
        ui->hostX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
        ui->hostY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
        ui->hostX2->setText(armXStructValueToName_.value((armStruct_ & 0x0030) >> 4));
        ui->hostY2->setText(armYStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
    }
}

void ICHCSystemSettingsFrame::SetConfig(int machineLenght,
                                        int maxLength,
                                        int iSafe,
                                        int outsafe,
                                        QString minText,
                                        QString maxText,
                                        QString axisName)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ui->hmiMachienLenghtLabel->setText(QString().sprintf("%.1f", host->SystemParameter((ICVirtualHost::ICSystemParameter)(machineLenght)).toInt() / (qreal)10));
    ui->hmiMaxLenthLabel->setText(QString().sprintf("%.1f", host->SystemParameter((ICVirtualHost::ICSystemParameter)(maxLength)).toInt() / (qreal)10));
    ui->hmiInnerSecurityLabel->setText(QString().sprintf("%.1f", host->SystemParameter((ICVirtualHost::ICSystemParameter)(iSafe)).toInt() / (qreal)10));
    ui->hmiOuterSecurityLabel->setText(QString().sprintf("%.1f", host->SystemParameter((ICVirtualHost::ICSystemParameter)(outsafe)).toInt() / (qreal)10));
    ui->hmiISafeName->setText(minText);
    ui->hostISafeLabel->setText(minText);
    ui->hmiOSafeName->setText(minText);
    ui->hostOSafeLabel->setText(maxText);
    ui->hmiDRLabel->setText(QString().sprintf("%.2f", ICParametersSave::Instance()->DistanceRotation(axisName)));

    ICCommandProcessor* process = ICCommandProcessor::Instance();
    ICGetAxisConfigsCommand command;
    command.SetSlave(process->SlaveID());
    if(axisName == "X")
    {
        command.SetAxis(0);
    }
    else if(axisName == "Y")
    {
        command.SetAxis(1);
    }
    else if(axisName == "Z")
    {
        command.SetAxis(2);
    }
    else
    {
        command.SetAxis(0);
    }
    ICCommunicationCommandBase::ResultVector ret = process->ExecuteCommand(command).value<ICCommunicationCommandBase::ResultVector>();
    if(!ret.isEmpty())
    {
        ui->hostMachineLengthLabel->setText(QString().sprintf("%.1f", ret.at(0) / (qreal)10));
        ui->hostMaxLengthLabel->setText(QString().sprintf("%.1f", ret.at(1) / (qreal)10));
        ui->hostInnerSecurityLabel->setText(QString().sprintf("%.1f", ret.at(2) / (qreal)10));
        ui->hostOutterSecurityLabel->setText(QString().sprintf("%.1f", ret.at(3) / (qreal)10));
        ui->hostDRLabel->setText(QString().sprintf("%.2f",(qreal)(ret.at(0) * 1000) / (uint)((ret.at(5) << 16) | ret.at(4))));
        totalL_ = ret.at(4);
        totalH_ = ret.at(5);
        xorSum_ = ret.at(6);
//        ui->label_7->setText(QString::number(ret.at(4)));
//        ui->label_8->setText(QString::number(ret.at(5)));
//        ui->label_11->setText(QString::number((uint)((ret.at(5) << 16) | ret.at(4))));
//        ui->label_21->setText(QString::number((qreal)(ret.at(1) * 1000)));
    }
}

void ICHCSystemSettingsFrame::on_seletHmiButton_clicked()
{
    ICCommandProcessor* process = ICCommandProcessor::Instance();
    ICSelecteConfigCommand command;
    command.SetSlave(process->SlaveID());
    command.SetSelected(currentAxis_);
    process->ExecuteCommand(command);
    UpdateConfigShow_();
}

void ICHCSystemSettingsFrame::on_selectHostButton_clicked()
{
    ICVirtualHost::ICSystemParameter machineL;
    ICVirtualHost::ICSystemParameter maxL;
    ICVirtualHost::ICSystemParameter iSafe;
    ICVirtualHost::ICSystemParameter oSafe;
    ICVirtualHost::ICSystemParameter totalL;
    ICVirtualHost::ICSystemParameter totalH;
    ICVirtualHost::ICSystemParameter xorSum;
    QString axis;
    if(currentAxis_ == 0)
    {
        machineL = ICVirtualHost::SYS_X_Length;
        maxL = ICVirtualHost::SYS_X_Maxium;
        iSafe = ICVirtualHost::SYS_X_InSafe;
        oSafe = ICVirtualHost::SYS_X_OutSafe;
        totalL = ICVirtualHost::SYS_X_TotalL;
        totalH = ICVirtualHost::SYS_X_TotalH;
        xorSum = ICVirtualHost::SYS_X_XorSum;
        axis = "X";
    }
    else if(currentAxis_ == 1)
    {
        machineL = ICVirtualHost::SYS_Y_Length;
        maxL = ICVirtualHost::SYS_Y_Maxium;
        iSafe = ICVirtualHost::SYS_Y_InSafe;
        oSafe = ICVirtualHost::SYS_Y_OutSafe;
        totalL = ICVirtualHost::SYS_Y_TotalL;
        totalH = ICVirtualHost::SYS_Y_TotalH;
        xorSum = ICVirtualHost::SYS_Y_XorSum;
        axis = "Y";
    }
    else if(currentAxis_ == 2)
    {
        machineL = ICVirtualHost::SYS_Z_Length;
        maxL = ICVirtualHost::SYS_Z_Maxium;
        iSafe = ICVirtualHost::SYS_Z_InSafe;
        oSafe = ICVirtualHost::SYS_Z_OutSafe;
        totalL = ICVirtualHost::SYS_Z_TotalL;
        totalH = ICVirtualHost::SYS_Z_TotalH;
        xorSum = ICVirtualHost::SYS_Z_XorSum;
        axis = "Z";
    }
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    ICParametersSave::Instance()->SetDistanceRotation(axis, ui->hostDRLabel->text().toDouble());
    host->SetSystemParameter(machineL, ui->hostMachineLengthLabel->text().toDouble() * (qreal)10);
    host->SetSystemParameter(maxL, ui->hostMaxLengthLabel->text().toDouble() * (qreal)10);
    host->SetSystemParameter(iSafe, ui->hostInnerSecurityLabel->text().toDouble() * (qreal)10);
    host->SetSystemParameter(oSafe, ui->hostOutterSecurityLabel->text().toDouble() * (qreal)10);
    host->SetSystemParameter(totalL, totalL_);
    host->SetSystemParameter(totalH, totalH_);
    host->SetSystemParameter(xorSum, xorSum_);
    ICVirtualHost::GlobalVirtualHost()->SaveAxisParam();
    UpdateConfigShow_();
}

void ICHCSystemSettingsFrame::on_saveParaButton_clicked()
{
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
}

void ICHCSystemSettingsFrame::UpdateConfigShow_()
{
    if(currentAxis_ == 0)
    {
        ui->axisXToolButton->click();
    }
    else if(currentAxis_ == 1)
    {
        ui->axisYToolButton->click();
    }
    else if(currentAxis_ == 2)
    {
        ui->axisZToolButton->click();
    }
}

void ICHCSystemSettingsFrame::StatusRefresh()
{
    ui->versionLabel->setText("Version: App 1.7.7-master; OS 1.0; Libs:4.7.3; Host:" + ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::Time).toString());
}

void ICHCSystemSettingsFrame::on_structSelectHostButton_clicked()
{
    ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_ARM_CONFIG, armStruct_);
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    ui->hmiX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
    ui->hmiY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
    ui->hmiX2->setText(armXStructValueToName_.value((armStruct_ & 0x0030) >> 4));
    ui->hmiY2->setText(armYStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
}
