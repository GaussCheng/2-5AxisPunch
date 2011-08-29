#include "icupdatesystem.h"
#include "ui_icupdatesystem.h"

#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QKeyEvent>

#include <QDebug>
#include "ickeyboard.h"
ICUpdateSystem *icUpdateSystem = NULL;

ICUpdateSystem::ICUpdateSystem(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICUpdateSystem),
    updateIniPath_("/mnt/udisk/HCUpdate/"),
    updateSettings_(NULL)
{
    ui->setupUi(this);

    InitInterface();
    icUpdateSystem = this;

}

ICUpdateSystem::~ICUpdateSystem()
{
    delete ui;
    if(updateSettings_ != NULL)
    {
        delete updateSettings_;
    }
}

void ICUpdateSystem::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICUpdateSystem::on_refreshToolButton_clicked()
{
    RefreshUSBIniInfo();
}

void ICUpdateSystem::on_updateToolButton_clicked()
{
    SystemUpdateStart();
}

void ICUpdateSystem::SystemUpdateStart()
{
    if(updateSettings_ == NULL)
    {
        return;
    }

    QStringList updateFileList = updateSettings_->childGroups();

    if(updateFileList.isEmpty())
    {
        return;
    }

    QDir targetFileDir;
    for(int i = 0; i < updateFileList.count(); ++i)
    {
        updateSettings_->beginGroup(updateFileList.at(i));
        targetFileDir.setPath(updateSettings_->value("targetPath").toString());

        if(!targetFileDir.absolutePath().isEmpty())
        {
            targetFileDir.mkpath(targetFileDir.absolutePath());
            ui->copyFilesShowLabel->setText(updateFileList.at(i));
            QFile updateFile(targetFileDir.absolutePath() + '/' + updateFileList.at(i));
            if(updateFile.exists())
            {
                updateFile.remove();
            }

            QDir currentFileDir(updateIniPath_);
            currentFileDir.cd(updateSettings_->value("currentPath").toString());

            QFile::copy(currentFileDir.absolutePath() + '/' + updateFileList.at(i),
                        targetFileDir.absolutePath() + '/' + updateFileList.at(i));
            ui->copyFilesProgressBar->setValue(i + 1);
        }
        updateSettings_->endGroup();
    }
//    if(QFile::exists("/opt/Qt/bin/custom_step.sh"))
//    {
//        system("cd /opt/Qt/bin && chmod +x custom_step.sh && ./custom_step.sh && rm /opt/Qt/bin/custom_step.sh");
//    }
    int ret = QMessageBox::information(this, tr("Congratulations"),
                                       tr("Update finish\n"
                                          "You must restart this program\n"
                                          "Would you want to restart now?"),
                                       QMessageBox::Yes | QMessageBox::Cancel,
                                       QMessageBox::Yes);
    switch(ret)
    {
    case QMessageBox::Yes:
        RestartAndUpdateTheProgram();
        break;
    default:
        break;
    }
}

void ICUpdateSystem::RefreshUSBIniInfo()
{
    if(updateSettings_ != NULL)
    {
        delete updateSettings_;
        updateSettings_ = NULL;
    }

    updateSettings_ = new QSettings(updateIniPath_ + "HCUpdate", QSettings::IniFormat);
    ui->versionShowLabel->setText(updateSettings_->value("version", tr("No available version")).toString());
    QStringList updateFileList = updateSettings_->childGroups();

    if(updateFileList.count() > 0)
    {
        ui->copyFilesProgressBar->setRange(0, updateFileList.count());
        ui->copyFilesProgressBar->setValue(0);
    }
}

void ICUpdateSystem::RestartAndUpdateTheProgram()
{
//    qApp->notify(qApp, new QCloseEvent());
    if(QFile::exists("/opt/Qt/bin/Multi-axisManipulatorSystem.bfe"))
    {
        system("cd /opt/Qt/bin \
               && mv Multi-axisManipulatorSystem Multi-axisManipulatorSystemOld \
               && decrypt.sh Multi-axisManipulatorSystem.bfe \
               && rm Multi-axisManipulatorSystemOld");
    }
    if(QFile::exists("/opt/Qt/bin/custom_step.sh"))
    {
        system("cd /opt/Qt/bin && chmod +x custom_step.sh && ./custom_step.sh && rm /opt/Qt/bin/custom_step.sh");
    }
    system("reboot");
}

void ICUpdateSystem::InitInterface()
{
    ui->copyFilesProgressBar->setValue(0);

    ui->versionLabel->setEnabled(false);
    ui->versionShowLabel->setEnabled(false);
    ui->copyFilesLabel->setEnabled(false);
    ui->copyFilesShowLabel->setEnabled(false);
    ui->copyFilesProgressBar->setEnabled(false);
    ui->rebootLabel->setEnabled(false);
    ui->rebootShowLabel->setEnabled(false);

    ui->deviceShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
    ui->versionShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
    ui->copyFilesShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
    ui->rebootShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
}

void ICUpdateSystem::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case ICKeyboard::FB_Up:
        {
            RefreshUSBIniInfo();
        }
        break;
    case ICKeyboard::FB_Down:
        {
            SystemUpdateStart();
        }
        break;
    }
    QFrame::keyPressEvent(e);
}
