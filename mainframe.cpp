/* If win32 and no cygwin, suppose it's MinGW or any other native windows compiler. */
#if defined(WIN32) && !defined(__CYGWIN__)
#define NATIVE_WIN32
#endif /* win32 and no cygwin */

#ifndef NATIVE_WIN32
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#endif

#include <QBoxLayout>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRunnable>
#include <QStackedLayout>
#include <QThreadPool>
#include <QTranslator>
#include <QDir>


#include "ui_mainframe.h"

#include "icactioncommand.h"
#include "icalarmdescriptiondialog.h"
#include "icalarmframe.h"
#include "iccycletimeandfinishedframe.h"
#include "ichcinstructionpageframe.h"
#include "ichcmanualoperationpageframe.h"
#include "ichcprogrammonitorframe.h"
//#include "ichctimeframe.h"
#include "icinitialframe.h"
#include "iciomonitor.h"
#include "ickeyboard.h"
#include "icmonitorpageframe.h"
#include "icorigindialog.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"
#include "icreturnpage.h"
#include "icsystemstatusframe.h"
#include "icvirtualhost.h"
#include "mainframe.h"
#include "moldinformation.h"
#include "icactiondialog.h"
#include "ictimerpool.h"
#include "ichostcomparepage.h"
#include "icupdatesystem.h"
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
#include "simulateknob.h"
#endif

#include <QDebug>


MainFrame *icMainFrame = NULL;
MainFrame::MainFrame(QSplashScreen *splashScreen, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrame),
    monitorPage_(NULL),
    centerStackedLayout_(new QStackedLayout),
    ledFlags_(0),
    errCode_(0),
    oldRunnigStatus_(0),
    oldXPos_(0),
    oldYPos_(0),
    oldZPos_(0),
    isOriginShown_(false),
    isReturnShown_(false),
    oldFinishCount_(0),
    cycleTime_(0),
    oldCycleTime_(0),
    oldStep_(0),
    screenSaver_(new ICScreenSaver()),
    isBackLightOff_(false),
    isOrigined_(false),
    isDoAction_(false),
    isXPosChanged_(false),
    isYPosChanged_(false),
    isZPosChanged_(false),
    axisDefine_(-1),
    registe_timer(new QTimer),
    reboot_timer(new QTimer)
  #ifdef HC_SK_8_SC
  ,
    oldSw_(-1),
    oldKey_(-1)
  #endif
{
    connect(this,
            SIGNAL(LoadMessage(QString)),
            splashScreen,
            SLOT(showMessage(QString)));
    connect(registe_timer,
            SIGNAL(timeout()),
            SLOT(CountRestTime()));
    emit LoadMessage("Connected");
    ui->setupUi(this);
    ICProgramHeadFrame::Instance(ui->headerContainer);

    QDir configDir("./sysconfig");
    configDir.setFilter(QDir::Files);
    QStringList backupFiles = configDir.entryList(QStringList()<<"*~");
    if(!backupFiles.isEmpty())
    {
        for(int i = 0; i != backupFiles.size(); ++i)
        {
            configDir.remove(backupFiles.at(i).left(backupFiles.at(i).size() - 1));
            configDir.rename(backupFiles.at(i), backupFiles.at(i).left(backupFiles.at(i).size() - 1));
        }
        //        QMessageBox::critical(this, tr("Warning"), tr("System Configs has been recover, please check the configs first!"));
    }
    configDir.cd("../records/");
    backupFiles = configDir.entryList(QStringList()<<"*~");
    if(!backupFiles.isEmpty())
    {
        for(int i = 0; i != backupFiles.size(); ++i)
        {
            configDir.remove(backupFiles.at(i).left(backupFiles.at(i).size() - 1));
            configDir.rename(backupFiles.at(i), backupFiles.at(i).left(backupFiles.at(i).size() - 1));
        }
        //        QMessageBox::critical(this, tr("Warning"), tr("Record has been recover, please check the record first!"));
    }
    configDir.cd("../subs/");
    backupFiles = configDir.entryList(QStringList()<<"*~");
    if(!backupFiles.isEmpty())
    {
        for(int i = 0; i != backupFiles.size(); ++i)
        {
            configDir.remove(backupFiles.at(i).left(backupFiles.at(i).size() - 1));
            configDir.rename(backupFiles.at(i), backupFiles.at(i).left(backupFiles.at(i).size() - 1));
        }
        //        QMessageBox::critical(this, tr("Warning"), tr("Sub has been recover, please check the sub first!"));
    }
    icMainFrame = this;
    screenSaver_->hide();
    buttonGroup_ = new QButtonGroup();
    nullButton_ = new QPushButton();
    nullButton_->hide();
    buttonGroup_->addButton(ui->teachButton);
    buttonGroup_->addButton(ui->ioMonitorButton);
    buttonGroup_->addButton(ui->alarmButton);
    buttonGroup_->addButton(ui->settingsButton);
    buttonGroup_->addButton(nullButton_);
    buttonGroup_->setExclusive(true);
    foreach(QAbstractButton* button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
    emit LoadMessage("MainFrame UI Loaded");
#ifndef Q_WS_X11
#ifndef Q_WS_WIN32
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
#endif
    emit LoadMessage("Reset the window hint");
    //    connect(ICVirtualHost::GlobalVirtualHost(),
    //            SIGNAL(StatusRefreshed()),
    //            this,
    //            SLOT(StatusRefreshed()));
    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
    emit LoadMessage("Ready to Refresh");
    InitCategoryPage();
    InitInterface();
    UpdateTranslate();
    emit LoadMessage("Translation Loaded");
    InitSignal();
    emit LoadMessage("Signals is ready");
#ifndef Q_WS_WIN32
    ledFD_ = open("/dev/szhc_leds", O_WRONLY);
#else
    ledFD_ = 0;
#endif

    //    QThreadPool::globalInstance()->start(new ICScreenSaverMonitor(this));
    //    connect(screenSaver_.data(),
    //            SIGNAL(Unlock()),
    //            this,
    //            SLOT(SetBackLightOn()));

    //    ui->xPosLabel->hide();
    //    ui->label_3->hide();
    //    ui->label_5->hide();
    actionDialog_ = new ICActionDialog(this);
//    axisWidgets_.append(QList<QWidget*>()<<ui->x1Label<<ui->x1mmLabel<<ui->xPosLabel);
//    axisWidgets_.append(QList<QWidget*>()<<ui->y1Label<<ui->y1mmLabel<<ui->yPosLabel);
//    axisWidgets_.append(QList<QWidget*>()<<ui->zLabel<<ui->zmmLabel<<ui->zPosLabel);
    compareAlarmNums_<<210<<211<<212<<213<<214<<215<<216<<217<<218;
    hostCompareDialog_ = new ICHostComparePage(this);
    UpdateAxisDefine_();
    ICKeyboard::Instace()->Receive();
    QTimer::singleShot(ICParametersSave::Instance()->BackLightTime() * 60000, this, SLOT(CheckedInput()));
    QTimer::singleShot(1000, this, SLOT(ClearPosColor()));
    oldFinishCount_ = ICVirtualHost::GlobalVirtualHost()->FinishProductCount();
    finishCount_ = oldFinishCount_;
    ui->cycleTimeAndFinistWidget->SetFinished(oldFinishCount_);

    //    QTimer::singleShot(100, this, SLOT(InitHeavyPage()));
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    simulateKnob_ = new SimulateKnob();
    simulateKnob_->show();
    connect(simulateKnob_,
            SIGNAL(manualButtonClicked()),
            SLOT(ShowManualPage()));
    connect(simulateKnob_,
            SIGNAL(stopButtonClicked()),
            SLOT(ShowStandbyPage()));
    connect(simulateKnob_,
            SIGNAL(autoButtonClicked()),
            SLOT(ShowAutoPage()));
//    this->setFixedSize(640, 480);

#endif
#ifdef Q_WS_X11
//            ShowInstructPage();
    //       ShowManualPage();
//         ShowAutoPage();
#endif
    QString moldName = ICParametersSave::Instance()->MoldName("Test.act");
    qDebug()<<"Last mold:"<<moldName;
    ICProgramHeadFrame::Instance()->SetCurrentMoldName(moldName);
    int restTime = ICParametersSave::Instance()->RestTime(0);
    if(restTime != 0)
    {
        QDateTime last = ICParametersSave::Instance()->BootDatetime();
        int overTime = QDateTime::currentDateTime().daysTo(last) * 24;
        restTime -= qAbs(overTime);
    }
    ICParametersSave::Instance()->SetBootDatetime(QDateTime::currentDateTime());
    isOverTime_ = (restTime == 1);
    registe_timer->start(3600000);

}


MainFrame::~MainFrame()
{
    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
    delete nullButton_;
    delete buttonGroup_;
    delete ui;

}


void MainFrame::closeEvent(QCloseEvent *e)
{
#ifdef Q_WS_WIN32
    simulateKnob_->close();
    delete simulateKnob_;
#endif
    QWidget::closeEvent(e);
}

void MainFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        UpdateTranslate();
    }
        break;
    default:
        break;
    }
}

void MainFrame::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case ICKeyboard::FB_F1:
    {
        ui->teachButton->click();
    }
        break;
    case ICKeyboard::FB_F2:
    {
        ui->ioMonitorButton->click();
    }
        break;
    case ICKeyboard::FB_F3:
    {
        ui->alarmButton->click();
    }
        break;
    case ICKeyboard::FB_F4:
    {
        ui->settingsButton->click();
    }
        break;
    case ICKeyboard::FB_F5:
    {
        ui->returnButton->click();
    }
        break;
    default:
    {
        QWidget::keyPressEvent(e);
    }
    }
}

void MainFrame::InitCategoryPage()
{
    emit LoadMessage("Start to Initialize category pages");
    ui->centerPageFrame->setLayout(centerStackedLayout_);
    emit LoadMessage("center page layout has been setted");

    emit LoadMessage("Start to Initialize initial pages");
    initialPage_ = new ICInitialFrame();
    centerStackedLayout_->addWidget(initialPage_);

    emit LoadMessage("Start to Initialize manual pages");
    manualPage_ = new ICHCManualOperationPageFrame();
    centerStackedLayout_->addWidget(manualPage_);

    emit LoadMessage("Start to Initialize auto pages");
    autoPage_ = new ICHCProgramMonitorFrame();
    centerStackedLayout_->addWidget(autoPage_);

    emit LoadMessage("Start to Initialize settings pages");
    settingsPage_ = new ICSettingsFrame();
    functionButtonToPage_.insert(ui->settingsButton, settingsPage_);
    centerStackedLayout_->addWidget(settingsPage_);

    emit LoadMessage("Start to Initialize alarm pages");
    alarmPage_ = ICAlarmFrame::Instance();
    functionButtonToPage_.insert(ui->alarmButton, alarmPage_);
    centerStackedLayout_->addWidget(alarmPage_);


    emit LoadMessage("Start to Initialize monitor pages");
    monitorPage_ = new ICMonitorPageFrame();
    functionButtonToPage_.insert(ui->ioMonitorButton, monitorPage_);
    centerStackedLayout_->addWidget(monitorPage_);


    emit LoadMessage("Start to Initialize instruct pages");
    instructPage_ = new ICHCInstructionPageFrame();
    functionButtonToPage_.insert(ui->teachButton, instructPage_);
    centerStackedLayout_->addWidget(instructPage_);

    emit LoadMessage("Start to Initialize origin pages");
    originExecutingPage_ = new ICOriginDialog();
    emit LoadMessage("end to Initialize  pages");

}

void MainFrame::InitInterface()
{
}

void MainFrame::InitSignal()
{
    connect(ui->settingsButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->ioMonitorButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->alarmButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->teachButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->returnButton,
            SIGNAL(clicked()),
            SLOT(ReturnButtonClicked()));
}

void MainFrame::UpdateTranslate()
{
}

void MainFrame::CategoryButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(functionButtonToPage_.contains(clickedButton))
    {
        if(clickedButton == ui->settingsButton ||
                clickedButton == ui->teachButton)
        {
            if(ICVirtualHost::GlobalVirtualHost()->DoseControled())
            {
                QMessageBox::warning(this,
                                     tr("Warning"),
                                     tr("Controlled, Can't modify!"));
                return;
            }
        }
        centerStackedLayout_->setCurrentWidget(functionButtonToPage_.value(clickedButton));
    }
    //    else if(clickedButton == ui->monitorPageButton)
    //    {
    //        monitorPage_ = new ICMonitorPageFrame();
    //        functionButtonToPage_.insert(ui->monitorPageButton, monitorPage_);
    //        centerStackedLayout_->addWidget(monitorPage_);
    //    }
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(clickedButton->text());
}

void MainFrame::StatusRefreshed()
{

    static ICAlarmString* alarmString = ICAlarmString::Instance();
    static ICVirtualHost* virtualHost = ICVirtualHost::GlobalVirtualHost();
    if(isOverTime_)
    {
        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
        errCode_ = 4000;
        alarmString->SetPriorAlarmNum(errCode_);
        ui->cycleTimeAndFinistWidget->SetAlarmInfo("Err" + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));
        return;
    }

//    uint axisLast = virtualHost->HostStatus(ICVirtualHost::AxisLastPos1).toUInt() |
//            (virtualHost->HostStatus(ICVirtualHost::AxisLastPos2).toUInt() << 16);
////    int pos = virtualHost->HostStatus(ICVirtualHost::XPos).toInt() * 10 + (axisLast1 & 0xF);
//    int pos = virtualHost->GetActualPos(ICVirtualHost::ICAxis_AxisX1, 0);
//    if(pos != oldXPos_)
//    {
//        oldXPos_ = pos ;
//        ui->teachButton->setText(QString().sprintf("%.1f", oldXPos_ / 10.0));
////        ui->xPosLabel->setText(QString().sprintf("%.2f", oldXPos_ / 100.0));
////        ui->xPosLabel->setStyleSheet("color: rgb(0, 0, 127);background-color: rgb(85, 255, 127);");
//        isXPosChanged_ = true;
//    }
//    pos = virtualHost->GetActualPos(ICVirtualHost::ICAxis_AxisY1, axisLast);
//    if(pos != oldYPos_)
//    {
//        oldYPos_ = pos;
//        ui->yPosLabel->setText(QString().sprintf("%.2f", oldYPos_ / 100.0));
//        ui->yPosLabel->setStyleSheet("color: rgb(0, 0, 127);background-color: rgb(85, 255, 127);");
//        isYPosChanged_ = true;
//    }

//    pos = virtualHost->GetActualPos(ICVirtualHost::ICAxis_AxisZ, axisLast);
//    if(pos != oldZPos_)
//    {
//        oldZPos_ = pos;
//        ui->zPosLabel->setText(QString().sprintf("%.2f", oldZPos_ / 100.0));
//        ui->zPosLabel->setStyleSheet("color: rgb(0, 0, 127);background-color: rgb(85, 255, 127);");
//        isZPosChanged_ = true;
//    }

    bool isControled = virtualHost->DoseControled();
    ICProgramHeadFrame::Instance()->ChangeControlStatus(isControled);
    newLedFlags_ = 0;
    newLedFlags_ |= (virtualHost->IsInputOn(72)? 8 : 0);
    newLedFlags_ |= (virtualHost->IsInputOn(64)? 4 : 0);
    newLedFlags_ |= (virtualHost->IsOutputOn(64)? 2 : 0);
    newLedFlags_ |= (virtualHost->IsOutputOn(65)? 1 : 0);
    if(newLedFlags_ != ledFlags_)
    {
        ledFlags_ = newLedFlags_;

#ifndef Q_WS_WIN32
#ifdef HC_ARMV6
        ioctl(ledFD_, 0, ledFlags_);
#else
//        ioctl(ledFD_, 2, ledFlags_);
        ioctl(ledFD_, 2, ledFlags_);
#endif
#endif
    }
    errCode_ = virtualHost->AlarmNum();
    if(compareAlarmNums_.indexOf(errCode_) != -1)
    {
        hostCompareDialog_->show();
    }
    ICProgramHeadFrame::Instance()->ChangePunchOrigin(virtualHost->IsInputOn(72));
    int hintCode = virtualHost->HintNum();
    if(alarmString->PriorAlarmNum() != static_cast<int>(errCode_) || hintCode != oldHintCode_)
    {
        oldHintCode_ = hintCode;
        qDebug()<<"hint code"<<hintCode<<alarmString->HintInfo(hintCode);
        alarmString->SetPriorAlarmNum(errCode_);
        if(errCode_ != 0)
        {
            if(hintCode != 5)
            {
                ui->cycleTimeAndFinistWidget->SetAlarmInfo("Err" + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));
            }
            else
            {
                ui->cycleTimeAndFinistWidget->SetHintInfo(tr("Hint") + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));

            }
            QTimer::singleShot(5000, this, SLOT(checkAlarmModify()));
        }
        else if(hintCode != 0)
        {
            ui->cycleTimeAndFinistWidget->SetHintInfo(tr("Hint") + QString::number(hintCode) + ":" + alarmString->HintInfo(hintCode));
        }
        else
        {
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("");
        }
    }
//    finishCount_ = virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt();
//    if(finishCount_ != oldFinishCount_)
//    {
//        ui->cycleTimeAndFinistWidget->SetFinished(virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt());
//        oldFinishCount_ = finishCount_;
//    }
    cycleTime_ = virtualHost->HostStatus(ICVirtualHost::Time).toUInt();
    if(cycleTime_ != oldCycleTime_)
    {
        ui->cycleTimeAndFinistWidget->SetCycleTime(QString().sprintf("%.2f", cycleTime_ / qreal(100)));
        oldCycleTime_ = cycleTime_;
    }

    ICProgramHeadFrame::Instance()->ChangeRobotOrigin(IsOrigined());
    runningStatus_ = virtualHost->CurrentStatus();

    if(runningStatus_ == ICVirtualHost::Manual)
    {
        speed_ = virtualHost->HostStatus(ICVirtualHost::DbgX0).toString();
        //        statusStr_ = tr("Manual");
    }
    else if(runningStatus_ == ICVirtualHost::Stop)
    {
        speed_ = "0";
        //        statusStr_ = tr("Stop");
//<<<<<<< HEAD
#ifdef Q_WS_X11
        finishCount_ = virtualHost->FinishProductCount();
        if(finishCount_ != oldFinishCount_)
        {
            ui->cycleTimeAndFinistWidget->SetFinished(finishCount_);
            oldFinishCount_ = finishCount_;
        }
#endif
//=======
//        ui->cycleTimeAndFinistWidget->SetFinished(oldFinishCount_);
//        ui->systemStatusFrame->SetProgramStatus(StatusLabel::ONSTATUS);
//>>>>>>> 7eb8511... 1.版本升级为4.1.8_SC
    }
    else if(runningStatus_ == ICVirtualHost::Auto)
    {
        if(hintCode == 15)
        {
            if(actionDialog_->isHidden())
            {
                actionDialog_->show();
            }
        }
        else
        {
            actionDialog_->hide();
        }
        finishCount_ = virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt();
//        finishCount_ = virtualHost->FinishProductCount();
        if(finishCount_ != oldFinishCount_)
        {
            ui->cycleTimeAndFinistWidget->SetFinished(finishCount_);
            virtualHost->SetFinishProductCount(finishCount_);
            oldFinishCount_ = finishCount_;
            virtualHost->SaveSystemConfig();
        }
        int speedVal =  virtualHost->GlobalSpeed();
        speed_ = QString::number(speedVal);
        if(virtualHost->HostStatus(ICVirtualHost::DbgX0) == ICVirtualHost::AutoReady)
        {
            runningStatus_ = ICVirtualHost::AutoReady;
        }
        else if(virtualHost->HostStatus(ICVirtualHost::DbgX0) == ICVirtualHost::AutoRunning)
        {
            runningStatus_ = ICVirtualHost::AutoRunning;
        }
        else if(virtualHost->HostStatus(ICVirtualHost::DbgX0) == ICVirtualHost::AutoSingleCycle)
        {
            runningStatus_ = ICVirtualHost::AutoSingleCycle;
        }
        else if(virtualHost->HostStatus(ICVirtualHost::DbgX0) == ICVirtualHost::AutoStopping)
        {
            runningStatus_ = ICVirtualHost::AutoStopping;
        }
        else if(virtualHost->HostStatus(ICVirtualHost::DbgX0) == ICVirtualHost::AutoOneCycle)
        {
            runningStatus_ = ICVirtualHost::AutoOneCycle;
        }
        runningStatus_ = ICVirtualHost::AutoRunning;
        //        statusStr_ = tr("Auto");
    }
    else if(runningStatus_ == ICVirtualHost::Teach)
    {
        speed_ = virtualHost->HostStatus(ICVirtualHost::DbgZ1).toString();
        //        statusStr_ = tr("Teach");
    }
    else
    {
        speed_ = "0";
    }
//    if(speed_ != ui->systemStatusFrame->CurrentSpeed())
//    {
//        //        ui->speedLabel->setText(speed_);
//        ui->systemStatusFrame->SetCurrentSpeed(speed_);
//    }
    if(runningStatus_ != oldRunnigStatus_)
    {
        //        ui->statusLabel->setText(statusStr_);
        oldRunnigStatus_ = runningStatus_; // must put here because the stop status want to use the new value;
        if(runningStatus_ == ICVirtualHost::Manual)
        {
            ICProgramHeadFrame::Instance()->ChangeCurrentStatus(1);
//            ui->settingsButton->setEnabled(false);
//            ui->teachButton->setEnabled(true);
//            ui->systemStatusFrame->SetManualStatus(StatusLabel::ONSTATUS);
            //            LevelChanged(ICProgramHeadFrame::Instance()->CurrentLevel());
            //            ui->functionPageButton->setEnabled(false);
            //            ui->recordPageButton->setText(tr("Instru            ui->recordPageButton->setText(tr("Instruct"));ct"));
            //            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoRunning)
        {
            ICProgramHeadFrame::Instance()->ChangeCurrentStatus(2);
//            ui->settingsButton->setEnabled(false);
//            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Running);
            //            ui->functionPageButton->setEnabled(false);
            //            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoReady)
        {
            ICProgramHeadFrame::Instance()->ChangeCurrentStatus(2);
//            ui->settingsButton->setEnabled(false);
//            ui->teachButton->setEnabled(false);
//            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Ready);
            //            ui->functionPageButton->setEnabled(false);
            //            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoSingleCycle)
        {
//            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::SingleCycle);
            //            ui->functionPageButton->setEnabled(false);
            //            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoStopping)
        {
            ICProgramHeadFrame::Instance()->ChangeCurrentStatus(2);
//            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Stopping);
            //            ui->functionPageButton->setEnabled(false);
            //            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoOneCycle)
        {
//            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::OneCycle);
        }
        //        else if(runningStatus_ == ICVirtualHost::Teach)
        //        {
        //            ui->systemStatusFrame->SetProgramStatus(StatusLabel::ONSTATUS);
        //        }
        else if(runningStatus_ == ICVirtualHost::Stop)
        {
            ICProgramHeadFrame::Instance()->ChangeCurrentStatus(0);
//            ui->settingsButton->setEnabled(true);
//            ui->teachButton->setEnabled(true);
            //            ui->systemStatusFrame->SetProgramStatus(virtualHost->IsCloseMoldPermit() ? StatusLabel::ONSTATUS : StatusLabel::OFFSTATUS);
            //            ui->systemStatusFrame->SetSystemStop();
            //            ui->recordPageButton->setText(tr("Records"));
            //            LevelChanged(ICProgramHeadFrame::Instance()->CurrentLevel());
            //            ui->functionPageButton->setEnabled(true);
            //            ui->recordPageButton->setEnabled(true);
            //            HideOrigin();
            HideReturn();
        }
        else if(runningStatus_ == ICVirtualHost::Origin)
        {
            ShowOrigin();
        }
        else if(runningStatus_ == ICVirtualHost::Return)
        {
//            ShowReturn();
        }
    }
    LevelChanged(ICProgramHeadFrame::Instance()->CurrentLevel());
    if(mousePoint_ != QCursor::pos())
    {
        mousePoint_ = QCursor::pos();
        SetHasInput(true);
    }

#ifdef HC_SK_8_SC
    int currentSw = -1;
    int currentKey = -1;
    if(virtualHost->IsInputOn(33/*manual*/))
    {
        currentSw = ICKeyboard::KS_ManualStatu;
    }
    else if(virtualHost->IsInputOn(37/*auto*/))
    {
        currentSw = ICKeyboard::KS_AutoStatu;
    }
    else if(virtualHost->IsInputOn(45/*stop*/))
    {
        currentSw = ICKeyboard::KS_StopStatu;
    }
    else if(virtualHost->IsInputOn(46/*start*/))
    {
        currentKey = ICKeyboard::VFB_Run;
    }
    else if(virtualHost->IsInputOn(36/*origin*/))
    {
        currentKey = ICKeyboard::FB_Origin;
    }
    if(oldSw_ != currentSw)
    {
        oldSw_ = currentSw;
        ICKeyboard::Instace()->SetSwitchValue(oldSw_);
    }
    if(oldKey_ != currentKey)
    {
        oldKey_ = currentKey;
        ICKeyboard::Instace()->SetKeyValue(oldKey_);
    }
#endif
}

void MainFrame::ShowManualPage()
{
//    functionPage_->ShowFunctionSelectPage();
    centerStackedLayout_->setCurrentWidget(manualPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Manual"));
    nullButton_->click();
    //    if(!IsOrigined())
    //    {
    //        QMessageBox::warning(this, tr("Warning"), tr("Need to origin!"));
    //    }
//    ui->recordPageButton->setText(tr("Instruct"));
    //    ui->recordPageButton->setEnabled(true);
}

void MainFrame::ShowAutoPage()
{
    centerStackedLayout_->setCurrentWidget(autoPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Auto")) ;
    nullButton_->click();
    //    if(!IsOrigined())
    //    {
    //        QMessageBox::warning(this, tr("Warning"), tr("Need to origin!"));
    //    }
}

void MainFrame::ShowInstructPage()
{
    centerStackedLayout_->setCurrentWidget(instructPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Instruct"));
    nullButton_->click();
}

void MainFrame::ShowStandbyPage()
{
    centerStackedLayout_->setCurrentWidget(initialPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Standby"));
    nullButton_->click();
//    ui->recordPageButton->setText(tr("Records"));
    //    qApp->processEvents();
}

void MainFrame::ShowFunctionPage()
{
    if(ICVirtualHost::GlobalVirtualHost()->DoseControled())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Controlled, Can't modify!"));
        return;
    }
    centerStackedLayout_->setCurrentWidget(settingsPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(ui->functionPageButton->text());
}

void MainFrame::ShowOrigin()
{
    if(!originExecutingPage_->isVisible())
    {
        //        ui->systemStatusFrame->SetOriginStatus(StatusLabel::ONSTATUS);
        //        isOriginShown_ = true;
        originExecutingPage_->open();
    }
}

void MainFrame::HideOrigin()
{
    //    if(isOriginShown_)
    //    {
    //        ui->systemStatusFrame->SetSystemStop();
    //        isOriginShown_ = false;
    if(originExecutingPage_->isVisible())
    {
        originExecutingPage_->accept();
        //        oldRunnigStatus_ = ICVirtualHost::Stop;
    }
}

void MainFrame::ShowReturn()
{
    if(!isReturnShown_)
    {
        isReturnShown_ = true;
        returnExecutingPage_->open();
    }
}

void MainFrame::HideReturn()
{
    if(isReturnShown_)
    {
        isReturnShown_ = false;
        returnExecutingPage_->accept();
    }
}

void MainFrame::ReturnButtonClicked()
{
    int status = ICKeyboard::Instace()->CurrentSwitchStatus();
    if(status == ICKeyboard::KS_ManualStatu)
    {
        ShowManualPage();
    }
    else if(status == ICKeyboard::KS_AutoStatu)
    {
        if(centerStackedLayout_->currentWidget() != autoPage_)
        {
            ShowAutoPage();
        }
    }
    else
    {
        ShowStandbyPage();
    }

}

void MainFrame::RecordButtonClicked()
{
    //    int status = ICVirtualHost::GlobalVirtualHost()->CurrentStatus();
    if(ICVirtualHost::GlobalVirtualHost()->DoseControled())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Controlled, Can't modify!"));
        return;
    }
#if !defined(Q_WS_WIN32) &&  !defined(Q_WS_X11)
    if(ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_ManualStatu)
#else
    if(!manualPage_->isHidden())
#endif
    {
        centerStackedLayout_->setCurrentWidget(instructPage_);
    }
    else
    {
        centerStackedLayout_->setCurrentWidget(recordPage_);
    }
}

void MainFrame::LevelChanged(int level)
{

    switch(level)
    {
    case ICParametersSave::MachineOperator:
    {
//        ui->functionPageButton->setEnabled(false);
//        ui->recordPageButton->setEnabled(false);
        ui->settingsButton->setEnabled(false);
        ui->teachButton->setEnabled(false);
#ifndef Q_WS_X11
//        if(!functionPage_->isHidden() ||
//                !instructPage_->isHidden() ||
//                !recordPage_->isHidden())
//        {
//            ui->returnPageButton->click();
//        }
#endif
    }
        break;
    case ICParametersSave::MachineAdmin:
    {
        if(ICKeyboard::Instace()->CurrentSwitchStatus() != ICKeyboard::KS_AutoStatu)
        {
            ui->teachButton->setEnabled(true);
            ui->settingsButton->setEnabled(true);
            settingsPage_->SetToShowAll(false);
        }
        else
        {
            ui->teachButton->setEnabled(false);
        }

    }
        break;
    case ICParametersSave::AdvanceAdmin:
    {
        ui->settingsButton->setEnabled(true);
        ui->teachButton->setEnabled(true);
        if(ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_StopStatu)
        {
            ui->settingsButton->setEnabled(true);
            settingsPage_->SetToShowAll(true);
//            ui->functionPageButton->setEnabled(true);
        }
        else
        {
            ui->settingsButton->setEnabled(false);
//            ui->functionPageButton->setEnabled(false);
        }
        if(ICKeyboard::Instace()->CurrentSwitchStatus() != ICKeyboard::KS_AutoStatu)
        {
            ui->teachButton->setEnabled(true);
//            ui->recordPageButton->setEnabled(true);
        }
        else
        {
            ui->teachButton->setEnabled(false);
//            ui->recordPageButton->setEnabled(false);
        }
    }
        break;
    default:
    {
//        ui->functionPageButton->setEnabled(false);
//        ui->recordPageButton->setEnabled(false);
    }
    }
//    if()
}
void MainFrame::StepChanged(int step)
{
//    ui->stepLabel->setText(QString::number(step));
    oldStep_ = step;
}


bool MainFrame::IsOrigined() const
{
    return ICVirtualHost::GlobalVirtualHost()->IsOrigined();
}

void MainFrame::ShowScreenSaver()
{
    //    screenSaver_->show();
    ICProgramHeadFrame::Instance()->SetCurrentLevel(ICParametersSave::MachineOperator);
}

bool MainFrame::IsInput() const
{
    return isDoAction_;
}

void MainFrame::SetHasInput(bool isInput)
{
    isDoAction_ = isInput;
    if(isInput && IsBackLightOff())
    {
        //        system("BackLight on");
        ICParametersSave::Instance()->SetBrightness(ICParametersSave::Instance()->Brightness());
        SetBackLightOff(false);
    }
}

bool MainFrame::IsBackLightOff() const
{
    return isBackLightOff_;
}

void MainFrame::SetBackLightOff(bool isOff)
{
    isBackLightOff_ = isOff;
}

void MainFrame::CheckedInput()
{
    if(!IsInput())
    {
        ShowScreenSaver();
        system("BackLight off");
        SetBackLightOff(true);
    }
    SetHasInput(false);
    QTimer::singleShot(ICParametersSave::Instance()->BackLightTime() * 60000, this, SLOT(CheckedInput()));
}

void MainFrame::ShowWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->show();
    }
}

void MainFrame::HideWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->hide();
    }
}

void MainFrame::UpdateAxisDefine_()
{
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    int currentAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
//    if(axisDefine_ != currentAxis)
//    {
//        axisDefine_ = currentAxis;
//        for(int i = 0 ; i != axisWidgets_.size(); ++i)
//        {
//            HideWidgets_(axisWidgets_[i]);
//        }


//        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) != ICVirtualHost::ICAxisDefine_Servo)
//        {
//            HideWidgets_(axisWidgets_[0]);
//        }
//        else
//        {
//            ShowWidgets_(axisWidgets_[0]);
//        }

//        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) != ICVirtualHost::ICAxisDefine_Servo)
//        {
//            HideWidgets_(axisWidgets_[1]);
//        }
//        else
//        {
//            ShowWidgets_(axisWidgets_[1]);
//        }

//        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) != ICVirtualHost::ICAxisDefine_Servo)
//        {
//            HideWidgets_(axisWidgets_[2]);
//        }
//        else
//        {
//            ShowWidgets_(axisWidgets_[2]);
//        }
//    }
}

void MainFrame::KeyToInstructEditor(int key)
{
    Q_UNUSED(key);
    instructPage_->ShowServoAction(key);
}

void MainFrame::ClearPosColor()
{
//    if(isXPosChanged_)
//    {
//        ui->xPosLabel->setStyleSheet("color: rgb(0, 0, 127);");
//        isXPosChanged_ = false;
//    }
//    if(isYPosChanged_)
//    {
//        ui->yPosLabel->setStyleSheet("color: rgb(0, 0, 127);");
//        isYPosChanged_ = false;
//    }
//    if(isZPosChanged_)
//    {
//        ui->zPosLabel->setStyleSheet("color: rgb(0, 0, 127);");
//        isZPosChanged_ = false;
//    }
//    QTimer::singleShot(1000, this, SLOT(ClearPosColor()));
}

void MainFrame::checkAlarmModify()
{
    if(errCode_ == 0)
    {
        ICAlarmFrame::Instance()->AlarmModifyTime();
    }
    else
    {
        QTimer::singleShot(5000, this, SLOT(checkAlarmModify()));
    }
}

void MainFrame::CountRestTime()
{
    int restTime = ICParametersSave::Instance()->RestTime(0);
    if(restTime < 1)
    {
        return;
    }
    if(restTime == 1)
    {
        isOverTime_ = true;
        return;
    }
    isOverTime_ = false;
    --restTime;
    ICParametersSave::Instance()->SetRestTime(restTime);
    ICParametersSave::Instance()->SetBootDatetime(QDateTime::currentDateTime());
}
