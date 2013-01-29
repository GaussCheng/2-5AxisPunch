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
#include "icbackuputility.h"
#include "ictipswidget.h"
#include "icprogramformatchecker.h"
#include "icconfigformatchecker.h"
#include <QDebug>

ICHCSystemSettingsFrame::ICHCSystemSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCSystemSettingsFrame)
{
    ui->setupUi(this);
    buttonGroup_ = new QButtonGroup();

    InitParameter();
    ui->extentFunctionCheckBox->setChecked(ICParametersSave::Instance()->IsExtentFunctionUsed());

    ui->axisXToolButton->setText(tr("X1 Axis"));
    ui->axisYToolButton->setText(tr("Y1 Axis"));
    ui->axisZToolButton->setText(tr("Z Axis"));
    ui->axisX2ToolButton->setText(tr("X2 Axis"));
    ui->axisY2ToolButton->setText(tr("Y2 Axis"));
    ui->axisAToolButton->setText(tr("A Axis"));
    ui->axisBToolButton->setText(tr("B Axis"));
    ui->axisCToolButton->setText(tr("C Axis"));
    ui->structDefButton->setText(tr("Struct Define"));
    //    buttonGroup_->addButton(ui->basicSettingsToolButton);
    buttonGroup_->addButton(ui->axisXToolButton);
    buttonGroup_->addButton(ui->axisYToolButton);
    buttonGroup_->addButton(ui->axisZToolButton);
    buttonGroup_->addButton(ui->axisX2ToolButton);
    buttonGroup_->addButton(ui->axisY2ToolButton);
    buttonGroup_->addButton(ui->axisAToolButton);
    buttonGroup_->addButton(ui->axisBToolButton);
    buttonGroup_->addButton(ui->axisCToolButton);
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
    armValueToName_.insert(0, tr("Single Arm"));
    armValueToName_.insert(1, tr("Double Arm"));
    ui->backLightTimeEdit->setValidator(new QIntValidator(1, 60, this));
    ui->backLightTimeEdit->SetThisIntToThisText(ICParametersSave::Instance()->BackLightTime());
    ui->brightnessBar->setValue((9 - ICParametersSave::Instance()->Brightness()));
#ifndef Q_WS_WIN32
    uname(&osInfo_);
#endif
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
        ui->keyToneButton->setChecked(true);
    }
    else
    {
        ui->keyToneButton->setText(tr("Key Tone(OFF)"));
        ui->keyToneButton->setChecked(false);
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
        ui->axisXToolButton->setText(tr("X1 Axis"));
        ui->axisYToolButton->setText(tr("Y1 Axis"));
        ui->axisZToolButton->setText(tr("Z Axis"));
        ui->axisX2ToolButton->setText(tr("X2 Axis"));
        ui->axisY2ToolButton->setText(tr("Y2 Axis"));
        ui->axisAToolButton->setText(tr("A Axis"));
        ui->axisBToolButton->setText(tr("B Axis"));
        ui->axisCToolButton->setText(tr("C Axis"));
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
    if(!ui->machineConfigPage->isHidden())
    {
        UpdateConfigShow_();
    }
    //    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    //    ui->hmiMachienLenghtLabel->setText(host->SystemParameter(ICVirtualHost::SYS_));

    QFrame::showEvent(e);
}

void ICHCSystemSettingsFrame::on_saveButton_clicked()
{
    QString dateTimeCmd = "date -s " + ui->dateTimeEdit->dateTime().toString("yyyy.MM.dd-hh:mm:ss");
    dateTimeCmd += " && hwclock -w";
    ::system(dateTimeCmd.toAscii());
    ICParametersSave::Instance()->SetBackLightTime(ui->backLightTimeEdit->TransThisTextToThisInt());
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
    if(!dir.exists())
    {
        return false;
    }
    if(dir.entryList(QStringList(), QDir::Files).isEmpty())
    {
        return false;
    }
    QFile file("/proc/mounts");
    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }
    QString content = file.readAll();
    file.close();
    return content.contains(QRegExp("/dev/sd[a-z]*[1-9]*"));

}

void ICHCSystemSettingsFrame::on_backupMachineButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
    tipsWidget.show();qApp->processEvents();
    ICBackupUtility backupUtility;
    Information(backupUtility.BackupDir("/opt/Qt/bin/sysconfig",
                                        "/mnt/udisk/HC5ABackup/sysconfig",
                                        QStringList()<<"param*.txt"<<"DistanceRotation"));
    //    system("rm /mnt/udisk/HC5ABackup/sysconfig/param*.txt");
    //    system("rm /mnt/udisk/HC5ABackup/sysconfig/DistanceRotation");
    //    system("mkdir -p /mnt/udisk/HC5ABackup/sysconfig");
    //    //    bool isSuccess = QFile::copy("./sysconfig/paramx.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramx.txt");
    //    //    isSuccess = isSuccess && QFile::copy("./sysconfig/paramy.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramy.txt");
    //    //    isSuccess = isSuccess && QFile::copy("./sysconfig/paramz.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramz.txt");
    //    Information(system("cp /opt/Qt/bin/sysconfig/param*.txt /opt/Qt/bin/sysconfig/DistanceRotation /mnt/udisk/HC5ABackup/sysconfig/ -f") >= 0 );
}

void ICHCSystemSettingsFrame::on_backupSystemButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
    tipsWidget.show();qApp->processEvents();
    ICBackupUtility backupUtility;
    Information(backupUtility.BackupDir("/opt/Qt/bin/sysconfig",
                                        "/mnt/udisk/HC5ABackup/sysconfig",
                                        QStringList()<<"system.txt"));
    //    system("rm /mnt/udisk/HC5ABackup/sysconfig/system.txt");
    //    system("mkdir -p /mnt/udisk/HC5ABackup/sysconfig");
    //    //    bool isSuccess = QFile::copy("./sysconfig/system.txt", "/mnt/udisk/HC5ABackup/sysconfig/system.txt");
    //    Information(system("cp /opt/Qt/bin/sysconfig/system.txt /mnt/udisk/HC5ABackup/sysconfig/system.txt -f") >= 0);
}

void ICHCSystemSettingsFrame::Information(bool isSuccess, const QString &msg)
{
    if(isSuccess)
    {
        QMessageBox::information(this, tr("Information"), tr("Operation finished!"));
    }
    else
    {
        QMessageBox::warning(this, tr("Information"), tr("Operation fail!") + msg);
    }
}

void ICHCSystemSettingsFrame::on_backupMoldsButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
    tipsWidget.show();qApp->processEvents();
    ICBackupUtility backupUtility;
    bool ret = backupUtility.BackupDir("/opt/Qt/bin/records",
                                       "/mnt/udisk/HC5ABackup/records",
                                       QStringList()<<"*.act"<<"*.fnc");
    ret = ret && backupUtility.BackupDir("/opt/Qt/bin/subs",
                                         "/mnt/udisk/HC5ABackup/subs",
                                         QStringList()<<"sub[0-7].prg");
    Information(ret);
    //    system("rm -R /mnt/udisk/HC5ABackup/records");
    //    system("rm -R /mnt/udisk/HC5ABackup/subs");
    //    system("mkdir -p /mnt/udisk/HC5ABackup/");
    //    //    bool isSuccess = QFile::copy("./records", "/mnt/udisk/HC5ABackup/");
    //    //    isSuccess = isSuccess && QFile::copy("./subs", "/mnt/udisk/HC5ABackup/");
    //    Information(system("cp -Rf /opt/Qt/bin/records/ /opt/Qt/bin/subs/ /mnt/udisk/HC5ABackup") >= 0);
}

void ICHCSystemSettingsFrame::on_backupAllButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
    tipsWidget.show();qApp->processEvents();
    ICBackupUtility backupUtility;
    bool ret = backupUtility.BackupDir("/opt/Qt/bin/sysconfig",
                                       "/mnt/udisk/HC5ABackup/sysconfig",
                                       QStringList()<<"param*.txt"<<"DistanceRotation");
    ret = ret && backupUtility.BackupDir("/opt/Qt/bin/sysconfig",
                                         "/mnt/udisk/HC5ABackup/sysconfig",
                                         QStringList()<<"system.txt");
    ret = ret && backupUtility.BackupDir("/opt/Qt/bin/records/",
                                         "/mnt/udisk/HC5ABackup/records/",
                                         QStringList()<<"*.act"<<"*.fnc");
    ret = ret && backupUtility.BackupDir("/opt/Qt/bin/subs",
                                         "/mnt/udisk/HC5ABackup/subs",
                                         QStringList()<<"sub[0-7].prg");
    Information(ret);

    //    system("rm -R /mnt/udisk/HC5ABackup");
    //    system("mkdir -p /mnt/udisk/HC5ABackup/sysconfig");
    //    //    bool isSuccess = QFile::copy("./sysconfig/paramx.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramx.txt");
    //    //    isSuccess = isSuccess && QFile::copy("./sysconfig/paramy.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramy.txt");
    //    //    isSuccess = isSuccess && QFile::copy("./sysconfig/paramz.txt", "/mnt/udisk/HC5ABackup/sysconfig/paramz.txt");

    //    //    isSuccess = isSuccess && QFile::copy("./sysconfig/system.txt", "/mnt/udisk/HC5ABackup/sysconfig/system.txt");

    //    //    isSuccess = isSuccess && QFile::copy("./records", "/mnt/udisk/HC5ABackup/");
    //    //    isSuccess = isSuccess && QFile::copy("./subs", "/mnt/udisk/HC5ABackup/");

    //    Information(system("cp /opt/Qt/bin/sysconfig/param*.txt /opt/Qt/bin/sysconfig/system.txt /opt/Qt/bin/sysconfig/DistanceRotation /mnt/udisk/HC5ABackup/sysconfig/ -f && cp -Rf /opt/Qt/bin/records/ /opt/Qt/bin/subs/ /mnt/udisk/HC5ABackup") >= 0);
}

void ICHCSystemSettingsFrame::on_restoreMachineButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
    tipsWidget.show();qApp->processEvents();

    if(CheckRestoreMachineFiles_())
    {

        ICBackupUtility backupUtility;
        bool ret = (backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                             "/opt/Qt/bin/sysconfig",
                                             QStringList()<<"param*.txt"<<"DistanceRotation"));
        Information(ret, tr("Backup files is broken!"));
        //    QDir dir("/mnt/udisk/HC5ABackup/sysconfig");
        //    if(!dir.exists())
        //    {
        //        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        //        return;
        //    }
        //    system("cp /mnt/udisk/HC5ABackup/sysconfig/param* /opt/Qt/bin/sysconfig -f");
        //    system("cp /mnt/udisk/HC5ABackup/sysconfig/DistanceRotation /opt/Qt/bin/sysconfig -f");
        //    Information(true);
        if(ret)
        {
            system("reboot");
        }
    }
}

void ICHCSystemSettingsFrame::on_restoreSystemButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
    tipsWidget.show();qApp->processEvents();
    if(CheckRestoreSystemFiles_())
    {
        ICBackupUtility backupUtility;
        bool ret = (backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                             "/opt/Qt/bin/sysconfig",
                                             QStringList()<<"system.txt"));
        Information(ret, tr("Backup files is broken!"));
        //    QDir dir("/mnt/udisk/HC5ABackup/sysconfig");
        //    if(!dir.exists())
        //    {
        //        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        //        return;
        //    }
        //    system("cp /mnt/udisk/HC5ABackup/sysconfig/system.txt /opt/Qt/bin/sysconfig -f");
        //    Information(true);
        if(ret)
        {
            system("reboot");
        }
    }
}

void ICHCSystemSettingsFrame::on_restoreMoldsButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
    tipsWidget.show();qApp->processEvents();
    QDir dir("/mnt/udisk/HC5ABackup/records");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    QStringList acts = dir.entryList(QStringList()<<"*.act");
    QStringList fncs = dir.entryList(QStringList()<<"*.fnc");
    if(fncs.contains("Base.fnc"))
    {
        fncs.removeOne("Base.fnc");
    }
    if(acts.size() != fncs.size())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
        return;
    }
    for(int i = 0; i != fncs.size(); ++i)
    {
        fncs[i] = fncs.at(i).left(fncs.at(i).size() - 4);
    }
    for(int i = 0; i != acts.size(); ++i)
    {
        if(!fncs.contains(acts.at(i).left(acts.at(i).size() - 4)))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
            return;
        }
    }
    QFile file;
    QString actContent;
    ICProgramFormatChecker programChecker;
    for(int i = 0; i != acts.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(acts.at(i)));
        actContent.clear();
        file.open(QFile::ReadOnly | QFile::Text);
        actContent = file.readAll();
        file.close();
        if(!programChecker.Check(actContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
            return;
        }
    }
    ICConfigFormatChecker configFormatChecker;
    for(int i = 0; i != fncs.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(fncs.at(i) + ".fnc"));
        actContent.clear();
        file.open(QFile::ReadOnly | QFile::Text);
        actContent = file.readAll();
        file.close();
        if(!configFormatChecker.CheckRowCount(actContent, 58,ICDataFormatChecker::kCompareEqual))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
            return;
        }
        if(!configFormatChecker.Check(actContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
            return;
        }
    }
    ICBackupUtility backupUtility;
    bool ret = backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/records",
                                        "/opt/Qt/bin/records",
                                        QStringList()<<"*.act"<<"*.fnc");
    dir.cdUp();
    dir.cd("subs");
    QStringList subs = dir.entryList(QStringList()<<"sub[0-7]");
    for(int i = 0; i != subs.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(subs.at(i)));
        actContent.clear();
        file.open(QFile::ReadOnly | QFile::Text);
        actContent = file.readAll();
        file.close();
        if(!programChecker.Check(actContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
            return;
        }
    }
    if(ret)
    {
        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/subs",
                                              "/opt/Qt/bin/subs",
                                              QStringList()<<"sub[0-7].prg");
    }
    Information(ret, tr("Backup files is broken!"));
    //    system("cp /mnt/udisk/HC5ABackup/records/*.act /mnt/udisk/HC5ABackup/records/*.fnc /opt/Qt/bin/records -f");
    //    dir.setPath("/mnt/udisk/HC5ABackup/subs");
    //    if(!dir.exists())
    //    {
    //        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
    //        return;
    //    }
    //    system("cp /mnt/udisk/HC5ABackup/subs/* /opt/Qt/bin/subs -f");
    //    Information(true);
    if(ret)
    {
        system("reboot");
    }
}

void ICHCSystemSettingsFrame::on_restoreAllButton_clicked()
{
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
    tipsWidget.show();qApp->processEvents();
    bool ret = CheckRestoreSystemFiles_();
    ret = ret && CheckRestoreMachineFiles_();
    if(ret)
    {
        ICBackupUtility backupUtility;
        ret = backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                       "/opt/Qt/bin/sysconfig",
                                       QStringList()<<"param*.txt"<<"DistanceRotation");
        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                              "/opt/Qt/bin/sysconfig",
                                              QStringList()<<"system.txt");
        if(ret)
        {
            on_restoreMoldsButton_clicked();

            system("reboot");
        }

    }
    Information(false, tr("Backup files is broken!"));

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
        for(int i = 1; i != ui->systemConfigPages->count(); ++i)
        {
            ui->systemConfigPages->setTabEnabled(i, true);
        }
    }
    else
    {
        ui->hmiGroupBox->setEnabled(false);
        ui->hostGroupBox->setEnabled(false);
        for(int i = 1; i != ui->systemConfigPages->count(); ++i)
        {
            ui->systemConfigPages->setTabEnabled(i, false);
        }
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

void ICHCSystemSettingsFrame::on_axisX2ToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 3;
    SetConfig(ICVirtualHost::SYS_P_Length,
              ICVirtualHost::SYS_P_Maxium,
              ICVirtualHost::SYS_P_InSafe,
              ICVirtualHost::SYS_P_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "P");
}

void ICHCSystemSettingsFrame::on_axisY2ToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 4;
    SetConfig(ICVirtualHost::SYS_Q_Length,
              ICVirtualHost::SYS_Q_Maxium,
              ICVirtualHost::SYS_Q_InSafe,
              ICVirtualHost::SYS_Q_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "Q");
}

void ICHCSystemSettingsFrame::on_axisAToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 5;
    SetConfig(ICVirtualHost::SYS_A_Length,
              ICVirtualHost::SYS_A_Maxium,
              ICVirtualHost::SYS_A_InSafe,
              ICVirtualHost::SYS_A_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "A");
}

void ICHCSystemSettingsFrame::on_axisBToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 6;
    SetConfig(ICVirtualHost::SYS_B_Length,
              ICVirtualHost::SYS_B_Maxium,
              ICVirtualHost::SYS_B_InSafe,
              ICVirtualHost::SYS_B_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "B");
}

void ICHCSystemSettingsFrame::on_axisCToolButton_clicked()
{
    ui->hostGroupBox->show();
    ui->hmiGroupBox->show();
    ui->selectHostButton->show();
    ui->hostStructDefGroup->hide();
    ui->hmiStructDefGroup->hide();
    ui->structSelectHostButton->hide();
    currentAxis_ = 7;
    SetConfig(ICVirtualHost::SYS_C_Length,
              ICVirtualHost::SYS_C_Maxium,
              ICVirtualHost::SYS_C_InSafe,
              ICVirtualHost::SYS_C_OutSafe,
              tr("Internal security zone"),
              tr("External security zone"),
              "C");
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
    armStruct_ = host->SystemParameter(ICVirtualHost::SYS_Config_Signal).toUInt();
    ui->hmiX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
    ui->hmiY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
    ui->hmiX2->setText(armXStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
    ui->hmiY2->setText(armYStructValueToName_.value((armStruct_ & 0x0100) >> 8));
    ui->hmiArm->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toUInt()));
    ui->hmiSignal->setText(QString::number(armStruct_));
    ui->hmiOut->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Out).toUInt()));
    ui->hmiPos->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Fixture).toUInt()));
    ui->hmiRev1->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Resv1).toUInt()));
    ui->hmiRev2->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Resv2).toUInt()));
    ui->hmiSum->setText(QString::number(host->SystemParameter(ICVirtualHost::SYS_Config_Xorsum).toUInt()));
    //    ui->hmiArm->setText(armValueToName_.value((armStruct_ & 0x0300) >> 8));
    ICGetAxisConfigsCommand command;
    ICCommandProcessor* processor = ICCommandProcessor::Instance();
    command.SetAxis(8);
    command.SetSlave(processor->SlaveID());
    ICCommunicationCommandBase::ResultVector ret = processor->ExecuteCommand(command).value<ICCommunicationCommandBase::ResultVector>();
    if(!ret.isEmpty())
    {
        armStruct_ = ret.at(0);
        ui->hostX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
        ui->hostY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
        ui->hostX2->setText(armXStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
        ui->hostY2->setText(armYStructValueToName_.value((armStruct_ & 0x0300) >> 8));
        ui->hostSignal->setText(QString::number(armStruct_));
        ui->hostArm->setText(QString::number(ret.at(1)));
        ui->hostOut->setText(QString::number(ret.at(2)));
        ui->hostPos->setText(QString::number(ret.at(3)));
        ui->hostRev1->setText(QString::number(ret.at(4)));
        ui->hostRev2->setText(QString::number(ret.at(5)));
        ui->hostSum->setText(QString::number(ret.at(6)));
        //        ui->hostArm->setText(armValueToName_.value((armStruct_ & 0x0300) >> 8));
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
    else if(axisName == "P")
    {
        command.SetAxis(3);
    }
    else if(axisName == "Q")
    {
        command.SetAxis(4);
    }
    else if(axisName == "A")
    {
        command.SetAxis(5);
    }
    else if(axisName == "B")
    {
        command.SetAxis(6);
    }
    else if(axisName == "C")
    {
        command.SetAxis(7);
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

//void ICHCSystemSettingsFrame::on_seletHmiButton_clicked()
//{
//    ICCommandProcessor* process = ICCommandProcessor::Instance();
//    ICSelecteConfigCommand command;
//    command.SetSlave(process->SlaveID());
//    command.SetSelected(currentAxis_);
//    process->ExecuteCommand(command);
//    UpdateConfigShow_();
//}

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
    else if(currentAxis_ == 3)
    {
        machineL = ICVirtualHost::SYS_P_Length;
        maxL = ICVirtualHost::SYS_P_Maxium;
        iSafe = ICVirtualHost::SYS_P_InSafe;
        oSafe = ICVirtualHost::SYS_P_OutSafe;
        totalL = ICVirtualHost::SYS_P_TotalL;
        totalH = ICVirtualHost::SYS_P_TotalH;
        xorSum = ICVirtualHost::SYS_P_XorSum;
        axis = "P";
    }
    else if(currentAxis_ == 4)
    {
        machineL = ICVirtualHost::SYS_Q_Length;
        maxL = ICVirtualHost::SYS_Q_Maxium;
        iSafe = ICVirtualHost::SYS_Q_InSafe;
        oSafe = ICVirtualHost::SYS_Q_OutSafe;
        totalL = ICVirtualHost::SYS_Q_TotalL;
        totalH = ICVirtualHost::SYS_Q_TotalH;
        xorSum = ICVirtualHost::SYS_Q_XorSum;
        axis = "Q";
    }
    else if(currentAxis_ == 5)
    {
        machineL = ICVirtualHost::SYS_A_Length;
        maxL = ICVirtualHost::SYS_A_Maxium;
        iSafe = ICVirtualHost::SYS_A_InSafe;
        oSafe = ICVirtualHost::SYS_A_OutSafe;
        totalL = ICVirtualHost::SYS_A_TotalL;
        totalH = ICVirtualHost::SYS_A_TotalH;
        xorSum = ICVirtualHost::SYS_A_XorSum;
        axis = "A";
    }
    else if(currentAxis_ == 6)
    {
        machineL = ICVirtualHost::SYS_B_Length;
        maxL = ICVirtualHost::SYS_B_Maxium;
        iSafe = ICVirtualHost::SYS_B_InSafe;
        oSafe = ICVirtualHost::SYS_B_OutSafe;
        totalL = ICVirtualHost::SYS_B_TotalL;
        totalH = ICVirtualHost::SYS_B_TotalH;
        xorSum = ICVirtualHost::SYS_B_XorSum;
        axis = "B";
    }
    else if(currentAxis_ == 7)
    {
        machineL = ICVirtualHost::SYS_C_Length;
        maxL = ICVirtualHost::SYS_C_Maxium;
        iSafe = ICVirtualHost::SYS_C_InSafe;
        oSafe = ICVirtualHost::SYS_C_OutSafe;
        totalL = ICVirtualHost::SYS_C_TotalL;
        totalH = ICVirtualHost::SYS_C_TotalH;
        xorSum = ICVirtualHost::SYS_C_XorSum;
        axis = "C";
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
    ICVirtualHost::GlobalVirtualHost()->SaveAxisParam(currentAxis_);
    UpdateConfigShow_();
}

//void ICHCSystemSettingsFrame::on_saveParaButton_clicked()
//{
//    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
//}

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
    else if(currentAxis_ == 3)
    {
        ui->axisX2ToolButton->click();
    }
    else if(currentAxis_ == 4)
    {
        ui->axisY2ToolButton->click();
    }
    else if(currentAxis_ == 5)
    {
        ui->axisAToolButton->click();
    }
    else if(currentAxis_ == 6)
    {
        ui->axisBToolButton->click();
    }
    else if(currentAxis_ == 7)
    {
        ui->axisCToolButton->click();
    }
}

void ICHCSystemSettingsFrame::StatusRefresh()
{
#ifdef Q_WS_WIN32
    QString os("1.0");
#else
    QString os(osInfo_.release);
#endif
    os += "; ";
    ui->versionLabel->setText("Version: OS:" + os + "App 3.1.5;Libs:4.7.3; Host:" + ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::Time).toString());
}

void ICHCSystemSettingsFrame::on_structSelectHostButton_clicked()
{
    ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(ICVirtualHost::SYS_ARM_CONFIG, armStruct_);
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    ui->hmiX1->setText(armXStructValueToName_.value(armStruct_ & 0x0003));
    ui->hmiY1->setText(armYStructValueToName_.value((armStruct_ & 0x000C) >> 2));
    ui->hmiX2->setText(armXStructValueToName_.value((armStruct_ & 0x0030) >> 4));
    ui->hmiY2->setText(armYStructValueToName_.value((armStruct_ & 0x00C0) >> 6));
    ui->hmiArm->setText(armValueToName_.value((armStruct_ & 0x0300) >> 8));
}

void ICHCSystemSettingsFrame::on_calibrationBtn_clicked()
{
    if(QMessageBox::warning(this,
                            tr("Warning"),
                            tr("The system will be reboot to calibrate! Do you want to continue?"),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
        ::system("cd /home/szhc && echo recal >>recal && reboot");
    }

}

void ICHCSystemSettingsFrame::on_brightMinus_clicked()
{
    uint brightness = ui->brightnessBar->value();
    if(brightness == 0)
    {
        return;
    }
    ui->brightnessBar->setValue((--brightness));
    ICParametersSave::Instance()->SetBrightness(9 - brightness);
}

void ICHCSystemSettingsFrame::on_brightPlus_clicked()
{
    uint brightness = ui->brightnessBar->value();
    if(brightness == (uint)ui->brightnessBar->maximum())
    {
        return;
    }
    ui->brightnessBar->setValue((++brightness));
    ICParametersSave::Instance()->SetBrightness(9 -brightness);
}

bool ICHCSystemSettingsFrame::CheckRestoreSystemFiles_()
{

    ICConfigFormatChecker configChecker;
    QFile file("/mnt/udisk/HC5ABackup/sysconfig/system.txt");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QString sysContent = file.readAll();
        file.close();
        if(!configChecker.CheckRowCount(sysContent, 162, ICDataFormatChecker::kCompareEqual))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong system config format!"));
            return false;
        }
        if(!configChecker.Check(sysContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong system config format!"));
            return false;
        }
    }
    return true;
}

bool ICHCSystemSettingsFrame::CheckRestoreMachineFiles_()
{
    QFile file;
    QString configContent;
    QDir dir("/mnt/udisk/HC5ABackup/sysconfig");
    QStringList configs = dir.entryList( QStringList()<<"param*.txt");
    ICConfigFormatChecker configChecker;
    for(int i = 0; i != configs.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(configs.at(i)));
        file.open(QFile::ReadOnly | QFile::Text);
        configContent = file.readAll();
        file.close();
        if(!configChecker.CheckRowCount(configContent, 7, ICDataFormatChecker::kCompareEqual))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong Axis config format!"));
            return false;
        }
        if(!configChecker.Check(configContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong Axis config format!"));
            return false;
        }
    }
    file.setFileName(dir.absoluteFilePath("DistanceRotation"));
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        configContent = file.readAll();
        file.close();
        QStringList rows = configContent.split("\n", QString::SkipEmptyParts);
        if(rows.size() != 8)
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong Rotation config format!"));
            return false;
        }
        QStringList cols;
        QStringList axisName;
        axisName<<"X"<<"Y"<<"Z"<<"P"<<"Q"<<"A"<<"B"<<"C";
        for(int i = 0; i != rows.size(); ++i)
        {
            cols = rows.at(i).split(" ", QString::SkipEmptyParts);
            if(cols.size() != 2)
            {
                QMessageBox::warning(this, tr("Warnning"), tr("Wrong Rotation config format!"));
                return false;
            }
            if(cols.at(0) != axisName.at(i))
            {
                QMessageBox::warning(this, tr("Warnning"), tr("Wrong Rotation config format!"));
                return false;
            }
            for(int j = 0; j != cols.at(1).size(); ++j)
            {
                if(!cols.at(1).at(j).isDigit())
                {
                    QMessageBox::warning(this, tr("Warnning"), tr("Wrong Rotation config format!"));
                    return false;
                }
            }

        }
    }
    return true;

}

void ICHCSystemSettingsFrame::on_clearButton_clicked()
{
    ui->newPwdEdit->clear();
    ui->oldPwdEdit->clear();
}
