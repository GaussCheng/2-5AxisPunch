#include "icupdatesystempage.h"
#include "ui_icupdatesystempage.h"

#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QKeyEvent>

#include <QDebug>
#include "ickeyboard.h"
#include "iccommandprocessor.h"
#include "iccommands.h"
#include "icvirtualhost.h"
#include "icupdatelogodialog.h"
#include <QDateTime>
#include "icparameterssave.h"
#include <QFile>
#include <QTextStream>
#include "icparameterssave.h"
#include <QRegExp>
#include <icutility.h>
#include "ictipswidget.h"
#include "icpasswordmodifydialog.h"
//ICUpdateSystemPage *ICUpdateSystemPage = NULL;


static void BuildShiftMap(int beg, int* map)
{
    int index = 0;
    for(int i = beg; i < 10; ++i)
    {
        map[index++] = i;
    }
    for(int i = 0; i < beg; ++i)
    {
        map[index++] = i;
    }
}

ICUpdateSystemPage * ICUpdateSystemPage::instance_ = NULL;
ICUpdateSystemPage::ICUpdateSystemPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICUpdateSystemPage),
    updateIniPath_("/mnt/udisk/HCUpdate_5A/"),
    updateHostPath_("/mnt/udisk/HCUpdateHost_5A/"),
    updateSettings_(NULL),
    updateHostSettings_(NULL),
    status_(-1),
    updateDialog_(NULL)
//#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
//    fileName("./Update_SuperPassward")
//#else
//    fileName("/mnt/udisk/Update_SuperPassward")
//#endif
{
    ui->setupUi(this);
    InitInterface();
//    ICUpdateSystemPage = this;
//    hostStatusToStringMap_.insert(-1, "Connect Host Fail");
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(QueryStatus()));

//    refresh_restTimer = new QTimer();
//    refresh_restTimer->start(1000*60*60);
//    connect(refresh_restTimer,
//            SIGNAL(timeout()),
//            this,
//            SLOT(RefreshRestTime()));

    updateSystem_ = new ICUpdateSystem();
#ifdef Q_WS_X11
    model_ = new ICUpdatePackModel(QDir::homePath() + "/HCUpdate_UpdateTest");
    updateSystem_->SetPacksDir(QDir::homePath() + "/HCUpdate_UpdateTest");
#else
    model_ = new ICUpdatePackModel("/mnt/udisk/");
    updateSystem_->SetPacksDir("/mnt/udisk/");
#endif
    model_->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model_->setHeaderData(1, Qt::Horizontal, tr("Create Time"));
    ui->packetTable->setModel(model_);
}

ICUpdateSystemPage::~ICUpdateSystemPage()
{
    delete ui;
    if(updateSettings_ != NULL)
    {
        delete updateSettings_;
    }
    if(updateHostSettings_ != NULL)
    {
        delete updateHostSettings_;
    }
    delete updateSystem_;
    delete model_;
}

void ICUpdateSystemPage::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        model_->setHeaderData(0, Qt::Horizontal, tr("Name"));
        model_->setHeaderData(1, Qt::Horizontal, tr("Create Time"));
        break;
    default:
        break;
    }
}

void ICUpdateSystemPage::showEvent(QShowEvent *e)
{
//    timer_.start(1000);
//    if(ICParametersSave::Instance()->IsRegisterFunctinOn())
//    {
//        ui->registerContainer->show();
//    }
//    else
//    {
//        ui->registerContainer->hide();
//    }
//    ICVirtualHost::GlobalVirtualHost()->StopRefreshStatus();
    RefreshRestTime();

    QFrame::showEvent(e);
}

void ICUpdateSystemPage::hideEvent(QHideEvent *e)
{
    timer_.stop();
//    ui->updateToolButton->setEnabled(false);
//    ui->updatePasswardButton->setEnabled(false);
//    ui->updateHostButton->setEnabled(false);
//    ui->connectHostButton->setEnabled(false);
//    ui->writeHostButton->setEnabled(false);
//    ui->rebootButton->setEnabled(false);
    ICVirtualHost::GlobalVirtualHost()->RestartRefreshStatus();
    QFrame::hideEvent(e);
}


void ICUpdateSystemPage::on_updateToolButton_clicked()
{
    SystemUpdateStart();
}

void ICUpdateSystemPage::SystemUpdateStart()
{
    ICTipsWidget tipWidget(tr("System Updating..."));
    tipWidget.show();
    qApp->processEvents();
    QModelIndex mI = ui->packetTable->currentIndex();
//    QString packName = model_->data(index.sibling(mI->row(), 0).toString();
    updateSystem_->StartUpdate(model_->data(mI.sibling(mI.row(), 0)).toString());
}

void ICUpdateSystemPage::RefreshUSBIniInfo()
{
    if(updateSettings_ != NULL)
    {
        delete updateSettings_;
        updateSettings_ = NULL;
    }

    if(updateHostSettings_ != NULL)
    {
        delete updateHostSettings_;
        updateHostSettings_ = NULL;
    }

    updateSettings_ = new QSettings(updateIniPath_ + "HCUpdate", QSettings::IniFormat);
    updateHostSettings_ = new QSettings(updateHostPath_ + "HCUpdateHost", QSettings::IniFormat);
//    ui->hmiVersionShowLabel->setText(updateSettings_->value("version", tr("No available HMI version")).toString());
//    ui->hostVersionShowLabel->setText(updateHostSettings_->value("version", tr("No available Host version")).toString());

    QString str;
    str = updateSettings_->value("superPassward").toString();
    if(str.isEmpty())
        str = QString(tr("No available New SuperPassward"));
    else
        str = QString(tr("New SuperPassward"));
//    ui->updatePasswardLabel->setText(str);
    if(!updateSettings_->value("version","").toString().isEmpty())
    {
//        ui->updateToolButton->setEnabled(true);
    }
    else
    {
//        ui->updateToolButton->setEnabled(false);
    }
//    if(!updateSettings_->value("superPassward","").toString().isEmpty())
//    {
//        ui->updatePasswardButton->setEnabled(true);
//    }
//    else
//    {
//        ui->updatePasswardButton->setEnabled(false);
//    }
    if(!updateHostSettings_->value("version", "").toString().isEmpty())
    {
//        ui->connectHostButton->setEnabled(true);
    }
    else
    {
//        ui->connectHostButton->setEnabled(false);
    }
    /*******************************************/
//        QStringList updateFileList = updateSettings_->childGroups();

//        if(updateFileList.count() > 0)
//        {
//            ui->copyFilesProgressBar->setRange(0, updateFileList.count());
//            ui->copyFilesProgressBar->setValue(0);
//        }
}

void ICUpdateSystemPage::RestartAndUpdateTheProgram()
{
    //    qApp->notify(qApp, new QCloseEvent());
    ICUtility::system("reboot");
}

void ICUpdateSystemPage::InitInterface()
{
//    ui->copyFilesProgressBar->setValue(0);

//    ui->versionLabel->setEnabled(false);
//    ui->versionShowLabel->setEnabled(false);
//    ui->copyFilesLabel->setEnabled(false);
//    ui->copyFilesShowLabel->setEnabled(false);
//    ui->copyFilesProgressBar->setEnabled(false);
//    ui->rebootLabel->setEnabled(false);
//    ui->rebootShowLabel->setEnabled(false);

//    ui->deviceShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
//    ui->versionShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
//    ui->copyFilesShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
//    ui->rebootShowLabel->setStyleSheet("border: 1px solid rgb(192,192,192);");
}

void ICUpdateSystemPage::keyPressEvent(QKeyEvent *e)
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

void ICUpdateSystemPage::updateHostButton()
{
    if(updateHostSettings_ == NULL)
    {
        return;
    }
    QStringList updateFileList = updateHostSettings_->childGroups();
    if(updateFileList.isEmpty())
    {
        return;
    }
//    timer_.stop();
//    ui->copyFilesProgressBar->setValue(0);
    QFile file(updateHostPath_ + updateFileList.at(0));
    if(file.open(QFile::ReadOnly))
    {
        ICCommandProcessor* processor = ICCommandProcessor::Instance();
        ICUpdateHostRequestCommand reqCommand;
        if(processor->ExecuteCommand(reqCommand).toBool())
        {
            qDebug("req successful");
            ICUpdateHostTransferCommand tranCommand;
            QByteArray readySend(file.readAll());
            file.close();
            bool isTranSuccessful = true;
//            ui->copyFilesProgressBar->setRange(0, readySend.size() / 32);
            for(int addr = 0; addr != readySend.size() / 32; ++addr)
            {
                tranCommand.SetDataBuffer(readySend.mid(addr << 5, 32));
                tranCommand.SetStartAddr(addr);
                isTranSuccessful = isTranSuccessful && processor->ExecuteCommand(tranCommand).toBool();
                if(isTranSuccessful)
                {
//                    ui->copyFilesProgressBar->setValue(addr + 1);
                }
                else
                {
                    QMessageBox::warning(this, tr("Warning"), tr("Update Host fail!"));
                    return ;
                }
            }
            qDebug("tran successful");
//            QMessageBox::information(this, tr("Congratulations"),
//                                     tr("Send to  Host finished!"));
//            ui->writeHostButton->setEnabled(true);

//            timer_.start(200);
            return;
//            ICUpdateHostFinishCommand finishCommand;
//            if(processor->ExecuteCommand(finishCommand).toBool())
//            {
//                QMessageBox::information(this, tr("Congratulations"),
//                                         tr("Update Host finished!"));

//                return;
//            }

        }
    }
    QMessageBox::warning(this, tr("Warning"), tr("Update Host fail!"));
}

void ICUpdateSystemPage::QueryStatus()
{
    ICUpdateHostQueryCommand command;
    status_ = ICCommandProcessor::Instance()->ExecuteCommand(command).toInt();

    switch(status_)
    {
        case 255:     // update status
        updateHostButton();
        break;

    case 1:     // write status
        writeHostButton();
        break;

    case 5:
        rebootButton();
        connectHostFlag = FALSE;
        break;

    case -1:    // reboot done
        if(connectHostFlag == FALSE)
        {
            timer_.stop();
            QMessageBox::information(this, tr("tips"), tr("Update Host Successful!"));
        }
        break;

    default:
        break;
    }

    //    if(status_ == 1)
    //    {
    ////        ui->updateHostButton->setEnabled(true);
    ////        ui->rebootButton->setEnabled(true);
    //            updateHostButton();
    //    }

    //    else
    //    {
    ////        ui->updateHostButton->setEnabled(false);
    ////        ui->rebootButton->setEnabled(false);
    //        timer_.stop();
    //    }
    ui->statusLabel->setText(QString::number(status_));
}

void ICUpdateSystemPage::rebootButton()
{
    ICUpdateHostRestartCommand rebootCommand;
    ICCommandProcessor::Instance()->ExecuteCommand(rebootCommand);
//    while(!ICCommandProcessor::Instance()->ExecuteCommand(rebootCommand).toBool())
//    {
////        QMessageBox::information(this, tr("tips"), tr("Reboot Successful!"));
//        QString ss = ICCommandProcessor::Instance()->ExecuteCommand(rebootCommand).toString();
//        QMessageBox::information(this, tr("tips"),ss);
//    }
}

void ICUpdateSystemPage::on_connectHostButton_clicked()
{
    ICTipsWidget tipWidget(tr("System Updating..."));
    tipWidget.show();
    qApp->processEvents();
    QModelIndex mI = ui->packetTable->currentIndex();
//    QString packName = model_->data(index.sibling(mI->row(), 0).toString();
    updateSystem_->StartUpdate(model_->data(mI.sibling(mI.row(), 0)).toString());

}

void ICUpdateSystemPage::writeHostButton()
{
    ICUpdateHostFinishCommand finishCommand;
    ICCommandProcessor::Instance()->ExecuteCommand(finishCommand);
//    if(ICCommandProcessor::Instance()->ExecuteCommand(finishCommand).toBool())
//    {
////        if(!timer_.isActive())
////        {
////            timer_.start(100);
////        }
////        QMessageBox::information(this, tr("Congratulations"),
////                                 tr("Update Host finished!"));
//        return;
//    }
}

void ICUpdateSystemPage::on_updateLogoButton_clicked()
{
 #ifndef Q_WS_X11
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this,tr("warning"),tr("USB is not exist!"));
        return;
    }
#endif
    if(updateDialog_ == NULL)
    {
        updateDialog_ = new ICUpdateLogoDialog(this);
    }
    updateDialog_->exec();
}


void ICUpdateSystemPage::on_generateBtn_clicked()
{
    QString ret;
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int i = 0; i != 10; ++i)
    {
        ret.append(QString::number(qrand() % 10));
    }
    ui->machineCode->setText(ret);
}

int ICUpdateSystemPage::Register(const QString& code, const QString& machineCode)
{
//    int pMap[10];
//    for(int i = 0; i != 10; ++i)
//    {
//        pMap[i] = i;
//    }
//    int sortMap[16];
//    sortMap[0] = 1;
//    sortMap[1] = 2;
//    sortMap[2] = 3;
//    sortMap[3] = 5;
//    sortMap[4] = 7;
//    sortMap[5] = 11;
//    sortMap[6] = 13;
//    sortMap[7] = 0;
//    sortMap[8] = 4;
//    sortMap[9] = 6;
//    sortMap[10] = 8;
//    sortMap[11] = 10;
//    sortMap[12] = 12;
//    sortMap[13] = 14;
//    sortMap[14] = 9;
//    sortMap[15] = 15;
//    QString rC = code;
//    QString sortRet(16, '0');
//    int beg;
//    if(rC.size() != 16)
//    {
//        return - 1;
//    }
//    for(int i = 0; i != sortRet.size(); ++i)
//    {
//        sortRet[sortMap[i]] = rC.at(i);
//    }
//    qDebug()<<sortRet;
//    QString mCode = machineCode.simplified();
//    mCode = mCode.remove(" ");
//    beg = 0;
//    for(int i = 0; i != mCode.size(); ++i)
//    {
//        if(!mCode.at(i).isDigit())
//        {
//            return -1;
//        }
//        beg += mCode.at(i).digitValue();
//    }
//    beg /= mCode.size();
//    beg %= 10;
//    for(int i = 0; i != sortRet.size(); ++i)
//    {
//        for(int j = 0; j != 10; ++j)
//        {
//            if(((pMap[j] + beg) % 10) == sortRet.at(i).digitValue())
//            {
//                sortRet[i] = QString::number(pMap[j]).at(0);
//                break;
//            }
//        }
//    }
//    if(sortRet.left(10) != mCode)
//    {
//        return -1;
//    }
//    return sortRet.right(6).toInt();
//    return sortRet.right(6).toInt() *24*7;

    int sortMap[16];
    sortMap[0] = 1;
    sortMap[1] = 2;
    sortMap[2] = 3;
    sortMap[3] = 5;
    sortMap[4] = 7;
    sortMap[5] = 11;
    sortMap[6] = 13;
    sortMap[7] = 0;
    sortMap[8] = 4;
    sortMap[9] = 6;
    sortMap[10] = 8;
    sortMap[11] = 10;
    sortMap[12] = 12;
    sortMap[13] = 14;
    sortMap[14] = 9;
    sortMap[15] = 15;
    QString rC = code;
    QString sortRet(16, '0');
    int beg;
    if(rC.size() != 16)
    {
        return - 1;
    }
    for(int i = 0; i != sortRet.size(); ++i)
    {
        sortRet[sortMap[i]] = rC.at(i);
    }
    QString mCode = machineCode.simplified();
    mCode = mCode.remove(" ");
    beg = 0;
    for(int i = 0; i != mCode.size(); ++i)
    {
        if(!mCode.at(i).isDigit())
        {
            return -1;
        }
        beg += mCode.at(i).digitValue();
    }
    beg /= mCode.size();
    beg %= 10;
    int pMap[10];
    BuildShiftMap(beg, pMap);
    for(int i = 0; i != sortRet.size(); ++i)
    {
        for(int j = 0; j != 10; ++j)
        {
            if(j == sortRet.at(i).digitValue())
            {
                sortRet[i] = QString::number(pMap[j]).at(0);
                break;
            }
        }
    }
    if(sortRet.left(10) != mCode)
    {
        return -1;
    }
//    return sortRet.right(6).toInt();
    return sortRet.right(6).toInt() * 24 * 7;
}

void ICUpdateSystemPage::on_registerBtn_clicked()
{
    if(ui->machineCode->text().isNull())
    {
        ui->tipLabel->setText(tr("Wrong Register Code!"));
        return;
    }
    int hour = Register(ui->registerCode->text(), ui->machineCode->text());
    if(hour == -1)
    {
        ui->tipLabel->setText(tr("Wrong Register Code!"));
    }
    else
    {
//        ICAppSettings().SetUsedTime(hour);
        ICParametersSave::Instance()->SetRestTime(hour);
        ui->tipLabel->setText(tr("Register Success!"));
        if(hour == 0)
        {
            ui->restTime->setText(tr("No Limit"));
        }
        else
        {
            ui->restTime->setText(QString::number(hour) + tr("hour"));
        }
        emit RegisterSucceed();
        ui->machineCode->clear();
        ui->registerCode->clear();

//        ICDALHelper::UpdateConfigValue(ICAddr_System_OtherUsedTime, hour);
    }
//    ICProgramHeadFrame::Instance()->ReashRestTime();
}

void ICUpdateSystemPage::RefreshRestTime()
{
    int rest_time = ICParametersSave::Instance()->RestTime(0);
    if(rest_time == 0){
        ui->restTime->setText(tr("No Limit"));
    }

    else if(rest_time > 0)
        ui->restTime->setText(QString::number(rest_time) + tr("hour"));
    else
        ui->restTime->setText(tr("No Register!"));
}

//int pMap_[10];
//int sortMap_[16] = {1,2,3,5,7,11,13,0,4,6,8,10,12,14,9,15};

bool ICUpdateSystemPage::CheckIsUsbAttached() const
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

void ICUpdateSystemPage::on_updatePasswardButton_clicked()
{
    ICPasswordModifyDialog pmD(this);
    int ret = pmD.exec();
    if(ret == QDialog::Accepted)
    {
        if(pmD.OldPassword() == ICParametersSave::Instance()->SuperPassward() ||
                pmD.OldPassword() == "szhcrobot")
        {
            ICParametersSave::Instance()->SetSuperPassward(pmD.NewPassword());
        }
        else
        {
            QMessageBox::warning(this,
                                 tr("Warning"),
                                 tr("Old password is wrong"));
        }
    }
}

void ICUpdateSystemPage::on_scanPanel_clicked()
{
    ICTipsWidget tipWidget(tr("Scanning..."));
    tipWidget.show();
    qApp->processEvents();
    model_->SetScanPattern(UPDATE_PREFIX"Panel*.bfe");
    model_->fetchMore(model_->index(0, 0));
    ui->packetTable->resizeColumnsToContents();
    ui->packetTable->setCurrentIndex(QModelIndex());
}

void ICUpdateSystemPage::on_scanHost_clicked()
{
    ICTipsWidget tipWidget(tr("Scanning..."));
    tipWidget.show();
    qApp->processEvents();
    model_->SetScanPattern(UPDATE_PREFIX"Host*.bfe");
    model_->fetchMore(model_->index(0, 0));
    ui->packetTable->resizeColumnsToContents();
    ui->packetTable->setCurrentIndex(QModelIndex());
}
