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


#include "ui_mainframe.h"

#include "icactioncommand.h"
#include "icalarmdescriptiondialog.h"
#include "icalarmframe.h"
#include "iccycletimeandfinishedframe.h"
#include "icfunctionpagebackframe.h"
#include "icfunctionpageframe.h"
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


#include <QDebug>

//class ICScreenSaverMonitor: public QRunnable
//{
//public:
//    ICScreenSaverMonitor(MainFrame* mainFrame) { mainFrame_ = mainFrame;}
//    void run()
//    {
//        int fd = open("/dev/screensaver", O_RDONLY);
//        if(fd < 0)
//        {
//            perror("Open screensaver fail \n");
//            return;
//        }
//        char readStr[30];
//        while(1)
//        {
//            memset(readStr, '\0', 30);
//            read(fd, readStr, 30);
//            qDebug(readStr);
//            if(strcmp(readStr, "BackLight Off") == 0)
//            {
//                mainFrame_->SetBackLightOff();
//                qDebug("set backLightOFF");
//            }
//        }
//    }
//private:
//    MainFrame* mainFrame_;
//};


MainFrame *icMainFrame = NULL;
MainFrame::MainFrame(QSplashScreen *splashScreen, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrame),
    monitorPage_(NULL),
    centerStackedLayout_(new QStackedLayout),
    ledFlags_(0),
    errCode_(0),
    oldRunnigStatus_(0),
    isOriginShown_(false),
    isReturnShown_(false),
    oldFinishCount_(0),
    oldCycleTime_(0),
    screenSaver_(new ICScreenSaver()),
    isBackLightOff_(false),
    isOrigined_(false),
    isDoAction_(false)
{
    connect(this,
            SIGNAL(LoadMessage(QString)),
            splashScreen,
            SLOT(showMessage(QString)));
    emit LoadMessage("Connected");
    ui->setupUi(this);
    icMainFrame = this;
    screenSaver_->hide();
    buttonGroup_ = new QButtonGroup();
    nullButton_ = new ICPageSwitch();
    nullButton_->hide();
    buttonGroup_->addButton(ui->functionPageButton);
    buttonGroup_->addButton(ui->monitorPageButton);
    buttonGroup_->addButton(ui->alarmPageButton);
    buttonGroup_->addButton(ui->recordPageButton);
    buttonGroup_->addButton(nullButton_);
    buttonGroup_->setExclusive(true);
    foreach(QAbstractButton* button, buttonGroup_->buttons())
    {
        button->setCheckable(true);
    }
    emit LoadMessage("MainFrame UI Loaded");
    this->setWindowFlags(Qt::FramelessWindowHint);
    emit LoadMessage("Reset the window hint");
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    emit LoadMessage("Ready to Refresh");
    InitCategoryPage();
    InitInterface();
    UpdateTranslate();
    emit LoadMessage("Translation Loaded");
    InitSignal();
    emit LoadMessage("Signals is ready");
    ledFD_ = open("/dev/szhc_leds", O_WRONLY);

//    QThreadPool::globalInstance()->start(new ICScreenSaverMonitor(this));
//    connect(screenSaver_.data(),
//            SIGNAL(Unlock()),
//            this,
//            SLOT(SetBackLightOn()));

//    ui->xPosLabel->hide();
//    ui->label_3->hide();
//    ui->label_5->hide();
    QTimer::singleShot(ICParametersSave::Instance()->BackLightTime() * 60000, this, SLOT(CheckedInput()));

    //    QTimer::singleShot(100, this, SLOT(InitHeavyPage()));
//    ShowInstructPage();
//    ShowManualPage();
//    ShowAutoPage();

}

MainFrame::~MainFrame()
{
    delete nullButton_;
    delete buttonGroup_;
    delete ui;
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
//        if(ui->functionPageButton->isEnabled())
//        {
//            ICFunctionPageBackFrame* page = qobject_cast<ICFunctionPageBackFrame*>(functionPage_);
//            if(page != NULL)
//            {
//                page->ShowFunctionSelectPage();
//            }
//            centerStackedLayout_->setCurrentWidget(functionPage_);
//            ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Function"));
//            ui->functionPageButton->setChecked(true);
//        }
        ui->functionPageButton->click();
    }
        break;
    case ICKeyboard::FB_F2:
    {
//        centerStackedLayout_->setCurrentWidget(monitorPage_);
//        ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Monitor"));
//        ui->monitorPageButton->setChecked(true);
        ui->monitorPageButton->click();
    }
        break;
    case ICKeyboard::FB_F3:
    {
//        if(ui->recordPageButton->isEnabled())
//        {
//            centerStackedLayout_->setCurrentWidget(recordPage_);
//            ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Records"));
//            ui->recordPageButton->setChecked(true);
//        }
        ui->recordPageButton->click();
    }
        break;
    case ICKeyboard::FB_F4:
    {
//        centerStackedLayout_->setCurrentWidget(alarmPage_);
//        ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Alarm History"));
//        ui->alarmPageButton->setChecked(true);
        ui->alarmPageButton->click();
    }
        break;
    case ICKeyboard::FB_F5:
    {
//        centerStackedLayout_->setCurrentWidget(initialPage_);
//        ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Standby"));
//        ReturnButtonClicked();
        ui->returnPageButton->click();
    }
        break;
    default:
    {
        QWidget::keyPressEvent(e);
    }
    }
    ui->childPageNameLabel->setText("");
}

void MainFrame::InitCategoryPage()
{
    emit LoadMessage("Start to Initialize category pages");
    ui->centerPageFrame->setLayout(centerStackedLayout_);
    emit LoadMessage("center page layout has been setted");

    initialPage_ = new ICInitialFrame;
    functionButtonToPage_.insert(ui->returnPageButton, initialPage_);
    centerStackedLayout_->addWidget(initialPage_);
    emit LoadMessage("Standby page has been loaded");

    alarmPage_ = ICAlarmFrame::Instance();
    functionButtonToPage_.insert(ui->alarmPageButton, alarmPage_);
    centerStackedLayout_->addWidget(alarmPage_);
    emit LoadMessage("Alarm history page has been loaded");

    functionPage_ = new ICFunctionPageBackFrame;
    functionButtonToPage_.insert(ui->functionPageButton, functionPage_);
    centerStackedLayout_->addWidget(functionPage_);
    emit LoadMessage("Function page has been loaded");

    instructPage_ = new ICHCInstructionPageFrame();
    centerStackedLayout_->addWidget(instructPage_);
    autoPage_ = new ICHCProgramMonitorFrame();
    centerStackedLayout_->addWidget(autoPage_);

    recordPage_ = MoldInformation::Instance();
    qDebug()<<"recordPage Loaded";
//    functionButtonToPage_.insert(ui->recordPageButton, recordPage_);
    qDebug()<<"recordPage Loaded 1";
    centerStackedLayout_->addWidget(recordPage_);
    emit LoadMessage("Records page has been loaded");

    manualPage_ = new ICHCManualOperationPageFrame();
    centerStackedLayout_->addWidget(manualPage_);
    emit LoadMessage("Manual page has been loaded");

    monitorPage_ = new ICMonitorPageFrame();
    functionButtonToPage_.insert(ui->monitorPageButton, monitorPage_);
    centerStackedLayout_->addWidget(monitorPage_);
    emit LoadMessage("Monitor page has been loaded");

    originExecutingPage_ = new ICOriginDialog();
    returnExecutingPage_ = new ICReturnPage();
//    centerStackedLayout_->addWidget(originExecutingPage_);
    emit LoadMessage("Origin page has been loaded");

//    centerStackedLayout_->setCurrentWidget(initialPage_);
    emit LoadMessage("End of initialzing category pages");
//    centerStackedLayout_->setCurrentWidget(instructPage_);
}

void MainFrame::InitInterface()
{
    emit LoadMessage("Start to load main frame interface");
    QLayout * programHeadLayout = new QHBoxLayout;
    programHeadLayout->setContentsMargins(0,0,0,0);
    ui->programHeadWidget->setLayout(programHeadLayout);
    programHeadLayout->addWidget(ICProgramHeadFrame::Instance());
    ICProgramHeadFrame::Instance()->SetCurrentMoldName(ICParametersSave::Instance()->MoldName("TEST.act"));
    emit LoadMessage("Header has been shown");

    //    QLayout * currentchildPageNameLayout = new QHBoxLayout;
    //    currentchildPageNameLayout->setContentsMargins(0,0,0,0);
    //    ui->currentChildPageNameWidget->setLayout(currentchildPageNameLayout);
    //    currentchildPageNameLayout->addWidget(ICCurrentChildPageNameFrame::Instance());
    emit LoadMessage("Child page name section has been shown");

    ui->functionPageButton->setIcon(QPixmap(":/resource/settings.png"));
    ui->monitorPageButton->setIcon(QPixmap(":/resource/monitor.png"));
    ui->recordPageButton->setIcon(QPixmap(":/resource/records.png"));
    ui->alarmPageButton->setIcon(QPixmap(":/resource/warning.png"));
    ui->returnPageButton->setIcon(QPixmap(":resource/return.png"));
    emit LoadMessage("page switchers pixmap has been shown");

    //    this->setStyleSheet("QFrame { border: none; }");
    //    ui->centerPageFrame->setStyleSheet("QFrame#centerPageFrame{border-top: 1px solid  #000080; border-bottom: 1px solid #000080;}");
    emit LoadMessage("End of loading main frame interface");
}

void MainFrame::InitSignal()
{
    connect(ui->functionPageButton,
            SIGNAL(clicked()),
            functionPage_,
            SLOT(ShowFunctionSelectPage()));

    connect(ui->functionPageButton,
            SIGNAL(clicked()),
            this,
            SLOT(CategoryButtonClicked()));
    connect(ui->alarmPageButton,
            SIGNAL(clicked()),
            this,
            SLOT(CategoryButtonClicked()));
    connect(ui->monitorPageButton,
            SIGNAL(clicked()),
            this,
            SLOT(CategoryButtonClicked()));
    //    connect(ui->returnPageButton,
    //            SIGNAL(clicked()),
    //            this,
    //            SLOT(CategoryButtonClicked()));
//    connect(ui->recordPageButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(CategoryButtonClicked()));
    connect(ui->recordPageButton,
            SIGNAL(clicked()),
            this,
            SLOT(RecordButtonClicked()));
    connect(ui->returnPageButton,
            SIGNAL(clicked()),
            this,
            SLOT(ReturnButtonClicked()));

//    connect(recordPage_,
//            SIGNAL(LoadFileInfoButtonClicked(QString)),
//            this,
//            SLOT(ShowFunctionPage()));
    connect(functionPage_,
            SIGNAL(SettingPageShown(QString)),
            this,
            SLOT(SetChildPageName(QString)));
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(LevelChanged(int)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StepChanged(int)),
            this,
            SLOT(StepChanged(int)));
//    connect(originExecutingPage_,
//            SIGNAL(OriginStatusChanged(bool)),
//            this,
//            SLOT(SetOrigined(bool)));
}

void MainFrame::UpdateTranslate()
{
    ui->functionPageButton->setText(tr("Function"));
    ui->monitorPageButton->setText(tr("Monitor"));
    ui->alarmPageButton->setText(tr("Alarm"));
    ui->recordPageButton->setText(tr("Record"));
    ui->returnPageButton-> setText(tr("Return"));
    ui->cycleTimeAndFinistWidget->SetAlarmInfo(ICAlarmString::Instance()->AlarmInfo(errCode_));
    ui->cycleTimeAndFinistWidget->SetCycleTime(QString().sprintf("%.1f", cycleTime_ / 200.0));
    ui->cycleTimeAndFinistWidget->SetFinished(oldFinishCount_);
    ui->xPosLabel->setText(QString().sprintf("%.1f", oldXPos_ / 10.0));
    ui->yPosLabel->setText(QString().sprintf("%.1f", oldYPos_ / 10.0));
    ui->zPosLabel->setText(QString().sprintf("%.1f", oldZPos_ / 10.0));
    ui->stepLabel->setText(QString::number(oldStep_));
}

void MainFrame::CategoryButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(functionButtonToPage_.contains(clickedButton))
    {
        centerStackedLayout_->setCurrentWidget(functionButtonToPage_.value(clickedButton));
    }
    //    else if(clickedButton == ui->monitorPageButton)
    //    {
    //        monitorPage_ = new ICMonitorPageFrame();
    //        functionButtonToPage_.insert(ui->monitorPageButton, monitorPage_);
    //        centerStackedLayout_->addWidget(monitorPage_);
    //    }
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(clickedButton->text());
    ui->childPageNameLabel->setText("");
}

void MainFrame::StatusRefreshed()
{
    static ICAlarmString* alarmString = ICAlarmString::Instance();
    static ICVirtualHost* virtualHost = ICVirtualHost::GlobalVirtualHost();
    int pos = virtualHost->HostStatus(ICVirtualHost::XPos).toInt();
    if(pos != oldXPos_)
    {
        oldXPos_ = pos;
        ui->xPosLabel->setText(QString().sprintf("%.1f", pos / 10.0));
    }
    pos = virtualHost->HostStatus(ICVirtualHost::YPos).toInt();
    if(pos != oldYPos_)
    {
        oldYPos_ = pos;
        ui->yPosLabel->setText(QString().sprintf("%.1f", pos / 10.0));
    }

    pos = virtualHost->HostStatus(ICVirtualHost::ZPos).toInt();
    if(pos != oldZPos_)
    {
        oldZPos_ = pos;
        ui->zPosLabel->setText(QString().sprintf("%.1f", pos / 10.0));
    }

    newLedFlags_ = 0;
    newLedFlags_ |= (virtualHost->IsInputOn(35)? 8 : 0);
    newLedFlags_ |= (virtualHost->IsInputOn(32)? 4 : 0);
    newLedFlags_ |= (virtualHost->IsOutputOn(32)? 2 : 0);
    newLedFlags_ |= (virtualHost->IsOutputOn(37)? 1 : 0);
    if(newLedFlags_ != ledFlags_)
    {
        ledFlags_ = newLedFlags_;
        ioctl(ledFD_, 2, ledFlags_);
    }
    errCode_ = virtualHost->HostStatus(ICVirtualHost::ErrCode).toUInt();
    if(alarmString->PriorAlarmNum() != static_cast<int>(errCode_))
    {
        alarmString->SetPriorAlarmNum(errCode_);
        if(errCode_ != 0)
        {
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("Err" + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));
        }
        else
        {
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("");
        }
    }
    finishCount_ = virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt();
    if(finishCount_ != oldFinishCount_)
    {
        ui->cycleTimeAndFinistWidget->SetFinished(virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt());
        oldFinishCount_ = finishCount_;
    }
    cycleTime_ = virtualHost->HostStatus(ICVirtualHost::DbgY0).toUInt();
    if(cycleTime_ != oldCycleTime_)
    {
        ui->cycleTimeAndFinistWidget->SetCycleTime(QString().sprintf("%.1f", cycleTime_ / 200.0));
        oldCycleTime_ = cycleTime_;
    }

    bool getHostOrigin = IsOrigined();
    if(getHostOrigin != isOrigined_)
    {
        isOrigined_ = getHostOrigin;
        isOrigined_ ? ui->systemStatusFrame->SetOriginStatus(StatusLabel::ONSTATUS) :
                      ui->systemStatusFrame->SetOriginStatus(StatusLabel::OFFSTATUS);
    }
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
    }
    else if(runningStatus_ == ICVirtualHost::Auto)
    {
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
//    else if(runningStatus_ == ICVirtualHost::Origin)
//    {
//        speed_ = virtualHost->HostStatus(ICVirtualHost::S).toString();
//    }
//    else if(runningStatus_ == ICVirtualHost::Return)
//    {
//        speed_ = virtualHost->HostStatus(ICVirtualHost::S).toString();
//    }
    if(speed_ != ui->systemStatusFrame->CurrentSpeed())
    {
        //        ui->speedLabel->setText(speed_);
        ui->systemStatusFrame->SetCurrentSpeed(speed_);
    }
    if(runningStatus_ != oldRunnigStatus_)
    {
        //        ui->statusLabel->setText(statusStr_);
        oldRunnigStatus_ = runningStatus_; // must put here because the stop status want to use the new value;
        if(runningStatus_ == ICVirtualHost::Manual)
        {
            ui->systemStatusFrame->SetManualStatus(StatusLabel::ONSTATUS);
            ui->functionPageButton->setEnabled(false);
//            ui->recordPageButton->setText(tr("Instru            ui->recordPageButton->setText(tr("Instruct"));ct"));
//            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoRunning)
        {
            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Running);
            ui->functionPageButton->setEnabled(false);
            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoReady)
        {
            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Ready);
            ui->functionPageButton->setEnabled(false);
            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoSingleCycle)
        {
            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::SingleCycle);
            ui->functionPageButton->setEnabled(false);
            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::AutoStopping)
        {
            ui->systemStatusFrame->SetAutoStatus(ICSystemStatusFrame::Stopping);
            ui->functionPageButton->setEnabled(false);
            ui->recordPageButton->setEnabled(false);
        }
        else if(runningStatus_ == ICVirtualHost::Teach)
        {
            ui->systemStatusFrame->SetProgramStatus(StatusLabel::ONSTATUS);
        }
        else if(runningStatus_ == ICVirtualHost::Stop)
        {
            ui->systemStatusFrame->SetSystemStop();
            ui->recordPageButton->setText(tr("Records"));
            LevelChanged(ICProgramHeadFrame::Instance()->CurrentLevel());
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
            ShowReturn();
        }
    }
    if(mousePoint_ != QCursor::pos())
    {
        mousePoint_ = QCursor::pos();
        SetHasInput(true);
    }
}

void MainFrame::ShowManualPage()
{
    centerStackedLayout_->setCurrentWidget(manualPage_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Manual"));
    nullButton_->click();
    if(!IsOrigined())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Need to origin!"));
    }
    ui->recordPageButton->setText(tr("Instruct"));
//    ui->recordPageButton->setEnabled(true);
}

void MainFrame::ShowAutoPage()
{
    centerStackedLayout_->setCurrentWidget(autoPage_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Auto"));
    nullButton_->click();
    if(!IsOrigined())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Need to origin!"));
    }
}

void MainFrame::ShowInstructPage()
{
    centerStackedLayout_->setCurrentWidget(instructPage_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Instruct"));
    nullButton_->click();
}

void MainFrame::ShowStandbyPage()
{
    centerStackedLayout_->setCurrentWidget(initialPage_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Standby"));
    nullButton_->click();
}

void MainFrame::ShowFunctionPage()
{
    centerStackedLayout_->setCurrentWidget(functionPage_);
    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(ui->functionPageButton->text());
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
    int status = ICVirtualHost::GlobalVirtualHost()->CurrentStatus();
    if(status == ICVirtualHost::Manual)
    {
        ShowManualPage();
    }
    else if(status == ICVirtualHost::Auto)
    {
        ShowAutoPage();
    }
    else
    {
        ShowStandbyPage();
    }
    ui->childPageNameLabel->setText("");

}

void MainFrame::RecordButtonClicked()
{
//    int status = ICVirtualHost::GlobalVirtualHost()->CurrentStatus();
    if(ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_ManualStatu)
    {
        centerStackedLayout_->setCurrentWidget(instructPage_);
    }
    else
    {
        centerStackedLayout_->setCurrentWidget(recordPage_);
    }
}

void MainFrame::SetChildPageName(const QString &name)
{
    ui->childPageNameLabel->setText(name);
}

void MainFrame::LevelChanged(int level)
{

    switch(level)
    {
    case ICParametersSave::MachineOperator:
    {
        ui->functionPageButton->setEnabled(false);
        ui->recordPageButton->setEnabled(false);
    }
        break;
    case ICParametersSave::MachineAdmin:
    case ICParametersSave::AdvanceAdmin:
    {
        if(oldRunnigStatus_ != ICVirtualHost::Manual &&
                oldRunnigStatus_ != ICVirtualHost::AutoReady
                && oldRunnigStatus_ != ICVirtualHost::AutoRunning
                && oldRunnigStatus_ != ICVirtualHost::AutoSingleCycle)
        {
            ui->functionPageButton->setEnabled(true);
        }
        if(oldRunnigStatus_ == ICVirtualHost::Manual || oldRunnigStatus_ == ICVirtualHost::Stop)
        {
            ui->recordPageButton->setEnabled(true);
        }
    }
        break;
    default:
    {
        ui->functionPageButton->setEnabled(false);
        ui->recordPageButton->setEnabled(false);
    }
    }
}
void MainFrame::StepChanged(int step)
{
    ui->stepLabel->setText(QString::number(step));
    oldStep_ = step;
}


bool MainFrame::IsOrigined() const
{
    return ICVirtualHost::GlobalVirtualHost()->IsOrigined();
}

void MainFrame::ShowScreenSaver()
{
//    screenSaver_->show();
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
        system("BackLight on");
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
//    system("reboot");
}
