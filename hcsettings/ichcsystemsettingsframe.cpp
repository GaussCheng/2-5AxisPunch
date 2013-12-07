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
#include "mainframe.h"

ICHCSystemSettingsFrame::ICHCSystemSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCSystemSettingsFrame)
{
    ui->setupUi(this);
    buttonGroup_ = new QButtonGroup();

    ui->languageButtonGroup->setId(ui->chineseBox,0);
    ui->languageButtonGroup->setId(ui->englishBox,1);
    InitParameter();
    ui->extentFunctionCheckBox->setChecked(ICParametersSave::Instance()->IsExtentFunctionUsed());
    ui->limitFunctionBox->setChecked(ICParametersSave::Instance()->IsRegisterFunctinOn());

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

    QList<QAbstractButton*> buttons_ = ui->languageButtonGroup->buttons();
    for(int i = 0; i != buttons_.size(); ++i)
    {
        buttons_[i]->setCheckable(true);
        connect(buttons_.at(i),
                SIGNAL(clicked()),
                this,
                SLOT(languageBoxChange()));
    }
#ifndef Q_WS_WIN32
    uname(&osInfo_);
#endif

    testvalue = FALSE;
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
    if(index == 0)
        ui->chineseBox->setChecked(true);
    else if(index == 1)
        ui->englishBox->setChecked(true);
    // ui->languageComboBox->setCurrentIndex(index);
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

void ICHCSystemSettingsFrame::languageBoxChange()
{
    ICParametersSave* paraSave = ICParametersSave::Instance();
    if(ui->languageButtonGroup->checkedId()== 0)
    {
        paraSave->SetCountry(QLocale::China);
    }
    else if(ui->languageButtonGroup->checkedId() == 1)
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
        if(index == 0)
            ui->chineseBox->setChecked(true);
        else if(index == 1)
            ui->englishBox->setChecked(true);
    //    ui->languageComboBox->setCurrentIndex(index);
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

        on_keyToneButton_toggled(ui->keyToneButton->isChecked());
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
    ui->limitFunctionBox->setEnabled(false);
    ui->limitFunctionLabel->setEnabled(false);
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
                                 tr("Please enter the old password and new password!"));
        return;
    }
    ICParametersSave* config = ICParametersSave::Instance();
    ICParametersSave::OperationLevel level;
    if(ui->machineAdminBox->isChecked())
    {
        level = ICParametersSave::MachineAdmin;
    }
    else if(ui->advanceAdminBox->isChecked())
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
        ui->limitFunctionLabel->setEnabled(true);
        ui->limitFunctionBox->setEnabled(true);
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

//void ICHCSystemSettingsFrame::on_backupMoldsButton_clicked()
//{
//    if(!CheckIsUsbAttached())
//    {
//        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
//        return;
//    }
//    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
//    tipsWidget.show();
//    qApp->processEvents();
//    ICBackupUtility backupUtility;
//    bool ret = backupUtility.BackupDir("/opt/Qt/bin/records",
//                                       "/mnt/udisk/HC5ABackup/records",
//                                       QStringList()<<"*.act"<<"*.fnc");
//    ret = ret && backupUtility.BackupDir("/opt/Qt/bin/subs",
//                                         "/mnt/udisk/HC5ABackup/subs",
//                                         QStringList()<<"sub[0-7].prg");
//    Information(ret);
//    //    system("rm -R /mnt/udisk/HC5ABackup/records");
//    //    system("rm -R /mnt/udisk/HC5ABackup/subs");
//    //    system("mkdir -p /mnt/udisk/HC5ABackup/");
//    //    //    bool isSuccess = QFile::copy("./records", "/mnt/udisk/HC5ABackup/");
//    //    //    isSuccess = isSuccess && QFile::copy("./subs", "/mnt/udisk/HC5ABackup/");
//    //    Information(system("cp -Rf /opt/Qt/bin/records/ /opt/Qt/bin/subs/ /mnt/udisk/HC5ABackup") >= 0);
//}

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

//void ICHCSystemSettingsFrame::on_restoreMoldsButton_clicked()
//{
//    if(!CheckIsUsbAttached())
//    {
//        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
//        return;
//    }
//    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
//    tipsWidget.show();qApp->processEvents();
//    QDir dir("/mnt/udisk/HC5ABackup/records");
//    if(!dir.exists())
//    {
//        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
//        return;
//    }
//    QStringList acts = dir.entryList(QStringList()<<"*.act");
//    QStringList fncs = dir.entryList(QStringList()<<"*.fnc");
//    if(fncs.contains("Base.fnc"))
//    {
//        fncs.removeOne("Base.fnc");
//    }
//    if(acts.size() != fncs.size())
//    {
//        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
//        return;
//    }
//    for(int i = 0; i != fncs.size(); ++i)
//    {
//        fncs[i] = fncs.at(i).left(fncs.at(i).size() - 4);
//    }
//    for(int i = 0; i != acts.size(); ++i)
//    {
//        if(!fncs.contains(acts.at(i).left(acts.at(i).size() - 4)))
//        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
//            return;
//        }
//    }
//    QFile file;
//    QString actContent;
//    ICProgramFormatChecker programChecker;
//    for(int i = 0; i != acts.size(); ++i)
//    {
//        file.setFileName(dir.absoluteFilePath(acts.at(i)));
//        actContent.clear();
//        file.open(QFile::ReadOnly | QFile::Text);
//        actContent = file.readAll();
//        file.close();
//        if(!programChecker.Check(actContent))
//        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
//            return;
//        }
//    }
//    ICConfigFormatChecker configFormatChecker;
//    for(int i = 0; i != fncs.size(); ++i)
//    {
//        file.setFileName(dir.absoluteFilePath(fncs.at(i) + ".fnc"));
//        actContent.clear();
//        file.open(QFile::ReadOnly | QFile::Text);
//        actContent = file.readAll();
//        file.close();
//        if(!configFormatChecker.CheckRowCount(actContent, 58,ICDataFormatChecker::kCompareEqual))
//        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
//            return;
//        }
//        if(!configFormatChecker.Check(actContent))
//        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
//            return;
//        }
//    }
//    ICBackupUtility backupUtility;
//    bool ret = backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/records",
//                                        "/opt/Qt/bin/records",
//                                        QStringList()<<"*.act"<<"*.fnc");
//    dir.cdUp();
//    dir.cd("subs");
//    QStringList subs = dir.entryList(QStringList()<<"sub[0-7]");
//    for(int i = 0; i != subs.size(); ++i)
//    {
//        file.setFileName(dir.absoluteFilePath(subs.at(i)));
//        actContent.clear();
//        file.open(QFile::ReadOnly | QFile::Text);
//        actContent = file.readAll();
//        file.close();
//        if(!programChecker.Check(actContent))
//        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
//            return;
//        }
//    }
//    if(ret)
//    {
//        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/subs",
//                                              "/opt/Qt/bin/subs",
//                                              QStringList()<<"sub[0-7].prg");
//    }
//    Information(ret, tr("Backup files is broken!"));
//    //    system("cp /mnt/udisk/HC5ABackup/records/*.act /mnt/udisk/HC5ABackup/records/*.fnc /opt/Qt/bin/records -f");
//    //    dir.setPath("/mnt/udisk/HC5ABackup/subs");
//    //    if(!dir.exists())
//    //    {
//    //        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
//    //        return;
//    //    }
//    //    system("cp /mnt/udisk/HC5ABackup/subs/* /opt/Qt/bin/subs -f");
//    //    Information(true);
//    if(ret)
//    {
//        system("reboot");
//    }
//}

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
    QDir dir("/mnt/udisk/HC5ABackup/records");
    if(!dir.exists())
    {
        ret = false;
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
        ret = false;
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
            ret = false;
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
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
            ret = false;
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
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
            ret = false;
            return;
        }
        if(!configFormatChecker.Check(actContent))
        {
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong config format!"));
            ret = false;
            return;
        }
    }
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
//            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!"));
            ret = false;
            return;
        }
    }
    if(ret)
    {
        ICBackupUtility backupUtility;
        ret = backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                       "/opt/Qt/bin/sysconfig",
                                       QStringList()<<"param*.txt"<<"DistanceRotation");
        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/sysconfig",
                                              "/opt/Qt/bin/sysconfig",
                                              QStringList()<<"system.txt");

        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/records",
                                            "/opt/Qt/bin/records",
                                            QStringList()<<"*.act"<<"*.fnc");

        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/subs",
                                                  "/opt/Qt/bin/subs",
                                                  QStringList()<<"sub[0-7].prg");

        Information(ret);
        if(ret)
        {
            system("reboot");
        }

    }
    else
    {
        Information(false, tr("Backup files is broken!"));
    }

}

void ICHCSystemSettingsFrame::on_umountButton_clicked()
{
    system("umount /mnt/udisk");
}

void ICHCSystemSettingsFrame::OnLevelChanged(int level)
{
    if(level == ICParametersSave::AdvanceAdmin)
    {
        for(int i = 1; i != ui->systemConfigPages->count(); ++i)
        {
            ui->systemConfigPages->setTabEnabled(i, true);
        }
    }
    else
    {
        for(int i = 1; i != ui->systemConfigPages->count(); ++i)
        {
            ui->systemConfigPages->setTabEnabled(i, false);
        }
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
    ui->versionLabel->setText("Version: OS:" + os + QString("App %1;Libs:4.7.3; Host:").arg(SW_VERSION) + ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::Time).toString());
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
//            if(cols.at(0) != axisName.at(i))
//            {
//                QMessageBox::warning(this, tr("Warnning"), tr("Wrong Rotation config format!"));
//                return false;
//            }
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

void ICHCSystemSettingsFrame::on_limitFunctionBox_toggled(bool checked)
{
    ICParametersSave::Instance()->SetRegisterFunction(checked);
}
