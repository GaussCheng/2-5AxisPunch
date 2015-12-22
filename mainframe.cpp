/* If win32 and no cygwin, suppose it's MinGW or any other native windows compiler. */
#if defined(WIN32) && !defined(__CYGWIN__)
#define NATIVE_WIN32
#endif /* win32 and no cygwin */

#ifndef NATIVE_WIN32
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <linux/input.h>
#endif

#include <QBoxLayout>
#include <QButtonGroup>
#include <QKeyEvent>
#include "icmessagebox.h"
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
#include "icmodifyframe.h"
#include "icsystemstatusframe.h"
#include "icvirtualhost.h"
#include "mainframe.h"
#include "moldinformation.h"
#include "icactiondialog.h"
#include "ictimerpool.h"
#include "ichostcomparepage.h"
#include "icupdatesystem.h"
#include "icrecaldialog.h"
#include "icbackupdialog.h"
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
#include "simulateknob.h"
#endif
#include "icmachinestructpage.h"
#include "icupdatesystempage.h"
#include "icmachineconfigpage.h"
#include "icautoactionpage.h"
#include "icmaualactionpage.h"
#include "icutility.h"

#include <QDebug>


QMap<int, int> keyMap;
QMap<int, int> keyToMap;
QMap<int, int> knobMap;
QMap<int, int> pulleyMap;
QList<int> currentKeySeq;

const QList<int> recalKeySeq = QList<int>()<<ICKeyboard::FB_F5
                                          <<ICKeyboard::FB_F1
                                         <<ICKeyboard::FB_F4
                                        <<ICKeyboard::FB_F1
                                       <<ICKeyboard::FB_F3
                                      <<ICKeyboard::FB_F1
                                     <<ICKeyboard::FB_F2
                                    <<ICKeyboard::FB_F5;

const QList<int> backupKeySeq = QList<int>()<<ICKeyboard::FB_F5
                                           <<ICKeyboard::FB_F2
                                          <<ICKeyboard::FB_F4
                                         <<ICKeyboard::FB_F2
                                        <<ICKeyboard::FB_F3
                                       <<ICKeyboard::FB_F2
                                      <<ICKeyboard::FB_F1
                                     <<ICKeyboard::FB_F5;

const QList<int> testKeySeq = QList<int>()<<ICKeyboard::FB_F5
                                         <<ICKeyboard::FB_F3
                                        <<ICKeyboard::FB_F4
                                       <<ICKeyboard::FB_F3
                                      <<ICKeyboard::FB_F2
                                     <<ICKeyboard::FB_F3
                                    <<ICKeyboard::FB_F1
                                   <<ICKeyboard::FB_F5;

MainFrame *icMainFrame = NULL;
MainFrame::MainFrame(QSplashScreen *splashScreen, QWidget *parent) :
    ICMainFrame(parent),
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
    isblockOriginShow_(false),
    axisDefine_(-1),
    showInfoDialog_(0)
  //    registe_timer(new QTimer),
  //    reboot_timer(new QTimer)
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
    connect(&registe_timer,
            SIGNAL(timeout()),
            SLOT(CountRestTime()));
    emit LoadMessage("Connected");
    ui->setupUi(this);
    ICProgramHeadFrame::Instance(ui->headerContainer);
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(MoldButtonClicked()),
            SLOT(OnMoldButtonClicked()));

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
        //        ICMessageBox::ICWarning(this, tr("Warning"), tr("System Configs has been recover, please check the configs first!"));
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
        //        ICMessageBox::ICWarning(this, tr("Warning"), tr("Record has been recover, please check the record first!"));
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
        //        ICMessageBox::ICWarning(this, tr("Warning"), tr("Sub has been recover, please check the sub first!"));
    }
    icMainFrame = this;
    screenSaver_->hide();
    buttonGroup_ = new QButtonGroup();
    nullButton_ = new QPushButton();
    nullButton_->hide();
    buttonGroup_->addButton(ui->teachButton);
    buttonGroup_->addButton(ui->ioMonitorButton);
    buttonGroup_->addButton(ui->recordButton);
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
    connect(&refreshTimer_,
            SIGNAL(timeout()),
            SLOT(StatusRefreshed()));
    //    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
    emit LoadMessage("Ready to Refresh");
    InitCategoryPage();
    BindShortcutKey();
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
    infoDialog_ = new ICInformationDialog(this);
    //    axisWidgets_.append(QList<QWidget*>()<<ui->x1Label<<ui->x1mmLabel<<ui->xPosLabel);
    //    axisWidgets_.append(QList<QWidget*>()<<ui->y1Label<<ui->y1mmLabel<<ui->yPosLabel);
    //    axisWidgets_.append(QList<QWidget*>()<<ui->zLabel<<ui->zmmLabel<<ui->zPosLabel);
    compareAlarmNums_<<210<<211<<212<<213<<214<<215<<216<<217<<218;
    hostCompareDialog_ = new ICHostComparePage(this);
    hostCompareDialog_->move(50,50);
    recordPage_ =  NULL;
    UpdateAxisDefine_();
    ICKeyboard::Instace()->Receive();
    //    QTimer::singleShot(ICParametersSave::Instance()->BackLightTime() * 60000, this, SLOT(CheckedInput()));
    //    QTimer::singleShot(1000, this, SLOT(ClearPosColor()));
    oldFinishCount_ = ICVirtualHost::GlobalVirtualHost()->FinishProductCount();
    finishCount_ = oldFinishCount_;
    ui->cycleTimeAndFinistWidget->SetFinished(oldFinishCount_);


#ifdef Q_WS_QWS
    SetScreenSaverInterval(ICParametersSave::Instance()->BackLightTime() * 60000);  //背光时间
    connect(this,SIGNAL(ScreenSave()),this,SLOT(CloseBackLight()));
    connect(this,SIGNAL(ScreenRestore()),this,SLOT(OpenBackLight()));
#endif

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
    QString moldName = ICParametersSave::Instance()->MoldName("Default.act");
    qDebug()<<"Last mold:"<<moldName;
    ICProgramHeadFrame::Instance()->SetCurrentMoldName(moldName);


    keyMap.insert(Qt::Key_F11, ICKeyboard::VFB_Run);
    keyMap.insert(Qt::Key_X, ICKeyboard::FB_Stop);
    keyMap.insert(Qt::Key_S, ICKeyboard::FB_Origin);
    keyMap.insert(Qt::Key_D, ICKeyboard::FB_Reset);
    keyMap.insert(Qt::Key_I, ICKeyboard::FB_Up);
    keyMap.insert(Qt::Key_N, ICKeyboard::FB_Down);

    keyMap.insert(Qt::Key_F9, ICKeyboard::VFB_X1Sub);
    keyMap.insert(Qt::Key_U, ICKeyboard::VFB_X1Add);
    keyMap.insert(Qt::Key_Z, ICKeyboard::VFB_Y1Sub);
    keyMap.insert(Qt::Key_V, ICKeyboard::VFB_Y1Add);

    keyMap.insert(Qt::Key_B, ICKeyboard::VFB_ZSub);
    keyMap.insert(Qt::Key_A, ICKeyboard::VFB_ZAdd);
    keyMap.insert(Qt::Key_G, ICKeyboard::VFB_Pose_Horizontal);
    keyMap.insert(Qt::Key_F, ICKeyboard::VFB_Pose_Vertical);

    keyMap.insert(Qt::Key_Q, ICKeyboard::VFB_X2Sub);
#ifdef HC_SK_8
    keyMap.insert(Qt::Key_P, ICKeyboard::VFB_X2Add);
    keyMap.insert(Qt::Key_K, ICKeyboard::VFB_Y2Sub);
#else
    keyMap.insert(Qt::Key_K, ICKeyboard::VFB_X2Add);
    keyMap.insert(Qt::Key_P, ICKeyboard::VFB_Y2Sub);
#endif
    keyMap.insert(Qt::Key_L, ICKeyboard::VFB_Y2Add);



#ifdef Q_WS_QWS
    keyMap.insert(Qt::Key_C, ICKeyboard::FB_F1);
    keyMap.insert(Qt::Key_W, ICKeyboard::FB_F2);
    keyMap.insert(Qt::Key_R, ICKeyboard::FB_F3);
    keyMap.insert(Qt::Key_M, ICKeyboard::FB_F4);
    keyMap.insert(Qt::Key_H, ICKeyboard::FB_F5);
    keyToMap.insert(ICKeyboard::FB_F1,Qt::Key_C);
    keyToMap.insert(ICKeyboard::FB_F2,Qt::Key_W);
    keyToMap.insert(ICKeyboard::FB_F3,Qt::Key_R);
    keyToMap.insert(ICKeyboard::FB_F4,Qt::Key_M);
    keyToMap.insert(ICKeyboard::FB_F5,Qt::Key_H);
#else
    keyMap.insert(Qt::Key_F1, ICKeyboard::FB_F1);
    keyMap.insert(Qt::Key_F2, ICKeyboard::FB_F2);
    keyMap.insert(Qt::Key_F3, ICKeyboard::FB_F3);
    keyMap.insert(Qt::Key_F4, ICKeyboard::FB_F4);
    keyMap.insert(Qt::Key_F5, ICKeyboard::FB_F5);
    keyToMap.insert(ICKeyboard::FB_F1,Qt::Key_F1);
    keyToMap.insert(ICKeyboard::FB_F2,Qt::Key_F2);
    keyToMap.insert(ICKeyboard::FB_F3,Qt::Key_F3);
    keyToMap.insert(ICKeyboard::FB_F4,Qt::Key_F4);
    keyToMap.insert(ICKeyboard::FB_F5,Qt::Key_F5);
#endif

    knobMap.insert(Qt::Key_F4, ICKeyboard::KS_ManualStatu);
    knobMap.insert(Qt::Key_F7, ICKeyboard::KS_StopStatu);
    knobMap.insert(Qt::Key_F5, ICKeyboard::KS_AutoStatu);

    pulleyMap.insert(Qt::Key_F13, 1);
    pulleyMap.insert(Qt::Key_F14, -1);

    installEventFilter(this);

#ifndef Q_WS_WIN32
    int keyFD_ = open("/dev/input/event1", O_RDWR);
    struct input_event inputEvent;
    inputEvent.type = EV_SYN; //__set_bit
    inputEvent.code = SYN_CONFIG;  //__set_bit
    inputEvent.value = 1;
    write(keyFD_,&inputEvent,sizeof(inputEvent));
    ::close(keyFD_);
#endif

    int restTime = ICParametersSave::Instance()->RestTime(0);
    if(restTime != 0)
    {
        QDateTime last = ICParametersSave::Instance()->BootDatetime();
        qint64 overTime = QDateTime::currentDateTime().secsTo(last) / 3600;
        restTime -= qAbs(overTime);
        if(restTime <= 1){
            restTime = 1;
//            ICMessageBox::ICWarning(this, "rest time", QString("%1 %2 %3").arg(last.toString())
//                                    .arg(overTime)
//                                    .arg(restTime));
        }
        ICParametersSave::Instance()->SetRestTime(restTime);
    }
    ICParametersSave::Instance()->SetBootDatetime(QDateTime::currentDateTime());
    isOverTime_ = (restTime == 1);
    registe_timer.start(3600000);
    refreshTimer_.start(ICTimerPool::RefreshTime);

}


MainFrame::~MainFrame()
{
    //    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
    delete nullButton_;
    delete buttonGroup_;
    delete originExecutingPage_;
    delete ui;

}


void MainFrame::closeEvent(QCloseEvent *e)
{
#ifndef Q_WS_QWS
    simulateKnob_->close();
    delete simulateKnob_;
#endif
    ICKeyboard::Instace()->Stop();
    QWidget::closeEvent(e);
}

void MainFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        BindShortcutKey();
        UpdateTranslate();
    }
        break;
    default:
        break;
    }
}

void MainFrame::keyPressEvent(QKeyEvent *e)
{
    //    SetHasInput(true);
    if(keyMap.contains(e->key()))
    {
        int key = keyMap.value(e->key());
        //        qDebug()<<key;
        switch(key)
        {
        default:
        {
            ICKeyboard *keyboard = ICKeyboard::Instace();
            keyboard->SetKeyValue(key);
            if(key == ICKeyboard::VFB_X1Add ||
                    key == ICKeyboard::VFB_Y1Add ||
                    key == ICKeyboard::VFB_ZAdd ||
                    key == ICKeyboard::VFB_X2Add ||
                    key == ICKeyboard::VFB_Y2Add ||
                    key == ICKeyboard::VFB_AAdd ||
                    key == ICKeyboard::VFB_BAdd ||
                    key == ICKeyboard::VFB_CAdd ||
                    key == ICKeyboard::VFB_X1Sub ||
                    key == ICKeyboard::VFB_Y1Sub ||
                    key == ICKeyboard::VFB_ZSub ||
                    key == ICKeyboard::VFB_X2Sub ||
                    key == ICKeyboard::VFB_Y2Sub ||
                    key == ICKeyboard::VFB_ASub ||
                    key == ICKeyboard::VFB_BSub ||
                    key == ICKeyboard::VFB_CSub ||
                    key == ICKeyboard::VFB_Pose_Horizontal ||
                    key == ICKeyboard::VFB_Pose_Vertical)
            {
                keyboard->SetPressed(true);
                if(instructPage_->isVisible())
                {
                    KeyToInstructEditor(key);
                }
            }
            //            ICKeyboardHandler::Instance()->Keypressed(key);
            //        QWidget::keyPressEvent(e);
        }
        }
    }
    else if(knobMap.contains(e->key()))
    {
        int k = knobMap.value(e->key());
        if(ICKeyboard::Instace()->CurrentSwitchStatus() != k)
        {
            ICKeyboard::Instace()->SetSwitchValue(k);
            currentKeySeq.clear();
        }
#ifndef Q_WS_WIN32
        //        static bool isExeced = false;
        //        if(!isExeced)
        //        {
        //            struct input_event inputEvent;
        //            inputEvent.type = EV_SYN; //__set_bit
        //            inputEvent.code = SYN_CONFIG;  //__set_bit
        //            inputEvent.value = 1;
        //            int keyFD_ = open("/dev/input/event1", O_RDWR);
        //            write(keyFD_,&inputEvent,sizeof(inputEvent));
        //            ::close(keyFD_);
        //            isExeced = true;
        //        }
#endif
        //        ICKeyboardHandler::Instance()->Keypressed(key);
    }
    else if(pulleyMap.contains(e->key()))
    {
        ICKeyboard::Instace()->SetPulleyValue(pulleyMap.value(e->key()));
    }
    QFileInfo keylog("./keylog");
    if(keylog.exists())
    {
        if(keylog.size() >= 1048576)
        {
            ICUtility::system(QString("echo %1,%2 > %3").arg(e->key()).arg(QDateTime::currentDateTime().toString()).arg("./keylog").toAscii());
        }
        else
        {
            ICUtility::system(QString("echo %1,%2 >> %3").arg(e->key()).arg(QDateTime::currentDateTime().toString()).arg("./keylog").toAscii());
        }
    }
    else
    {
        ICUtility::system(QString("echo %1,%2 >> %3").arg(e->key()).arg(QDateTime::currentDateTime().toString()).arg("./keylog").toAscii());
    }
}

void MainFrame::keyReleaseEvent(QKeyEvent *e)
{
    if(keyMap.contains(e->key()))
    {
        ICKeyboard::Instace()->SetPressed(false);
    }
}

bool MainFrame::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *e = static_cast<QKeyEvent*>(event);
    if(e){
        //        qDebug() << e->type();
        if(keyToMap.values().contains(e->key()) && e->type() == QKeyEvent::KeyRelease)
        {
            int key = keyMap.value(e->key());
            currentKeySeq.append(key);
            if(currentKeySeq.size() == recalKeySeq.size())
            {
                if(currentKeySeq == recalKeySeq)
                {
                    ICRecalDialog recalDialog;
                    recalDialog.exec();

                }
                else if(currentKeySeq == backupKeySeq)
                {
                    ICBackupDialog backupDialog;
                    backupDialog.exec();
                }
                else if(currentKeySeq == testKeySeq)
                {
                    ICUtility::system("chmod +x ./test_robot.sh && ./test_robot.sh");
                    //                exit(0);
                }
                currentKeySeq.clear();
            }
        }
    }
    return ICMainFrame::eventFilter(obj,event);
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
    manualPage_ = new ICHCManualOperationPageFrame();//ICHCManualOperationPageFrame
    centerStackedLayout_->addWidget(manualPage_);
    connect(manualPage_,
            SIGNAL(clearProductButtonClicked()),
            SLOT(ClearProduct()));


    emit LoadMessage("Start to Initialize auto pages");
    autoPage_ = new ICHCProgramMonitorFrame();
    centerStackedLayout_->addWidget(autoPage_);

    //    settingsPage_ = new ICSettingsFrame();
    //    functionButtonToPage_.insert(ui->settingsButton, settingsPage_);
    //    centerStackedLayout_->addWidget(settingsPage_);

    emit LoadMessage("Start to Initialize alarm pages");
    //    functionButtonToPage_.insert(ui->recordButton, NULL);

    alarmButtonToPage_.insert(ui->alarmReturn,NULL);
    alarmPage_ = ICAlarmFrame::Instance();
    alarmButtonToPage_.insert(ui->alarmButton,alarmPage_);
    centerStackedLayout_->addWidget(alarmPage_);

    emit LoadMessage("Start to Initialize modify pages");
    modifyPage_ = ICModifyFrame::Instance();
    alarmButtonToPage_.insert(ui->modifyButton,modifyPage_);
    centerStackedLayout_->addWidget(modifyPage_);

    emit LoadMessage("Start to Initialize monitor pages");
    monitorPage_ = new ICMonitorPageFrame();
    //    monitorPage_ = new ICProgramMainPage();
    functionButtonToPage_.insert(ui->ioMonitorButton, monitorPage_);
    centerStackedLayout_->addWidget(monitorPage_);


    emit LoadMessage("Start to Initialize instruct pages");
#ifdef TEACH_PAGE
    instructPage_ =  new ICHCInstructionPageFrame();//ICHCInstructionPageFrame  ICProgramMainPage
#else
    instructPage_ =  ICProgramPage::Instance_();//ICHCInstructionPageFrame  ICProgramMainPage
#endif

    functionButtonToPage_.insert(ui->teachButton, instructPage_);
    centerStackedLayout_->addWidget(instructPage_);


    emit LoadMessage("Start to Initialize settings pages");
    baseFuncPage_ = new ICHCSystemSettingsFrame();
    settingButtonToPage_.insert(ui->baseButton,baseFuncPage_);
    centerStackedLayout_->addWidget(baseFuncPage_);

    axisPage_ = new ICMachineStructPage();
    settingButtonToPage_.insert(ui->axisButton,axisPage_);
    centerStackedLayout_->addWidget(axisPage_);

    servoPage_ = new ICMachineConfigPage();
    settingButtonToPage_.insert(ui->servoButton,servoPage_);
    centerStackedLayout_->addWidget(servoPage_);

    updatePage_ =  ICUpdateSystemPage::Instance();
    settingButtonToPage_.insert(ui->updateButton,updatePage_);
    centerStackedLayout_->addWidget(updatePage_);


    //      updatePage_ =  new ICProgramPage();
    //      settingButtonToPage_.insert(ui->updateButton,updatePage_);
    //      centerStackedLayout_->addWidget(updatePage_);

    settingButtonToPage_.insert(ui->SettingReturn,NULL);


    emit LoadMessage("Start to Initialize origin pages");
    originExecutingPage_ = new ICOriginDialog();
    emit LoadMessage("end to Initialize  pages");


    //    connect(manualPage_,SIGNAL(ChangeDelay(int)),
    //            instructPage_,SLOT(ChangeDelay(int)));

    //    connect(manualPage_,SIGNAL(ChangeWaste(bool)),
    //            instructPage_,SLOT(ChangeWaste(bool)));

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
    connect(ui->recordButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->teachButton,
            SIGNAL(clicked()),
            SLOT(CategoryButtonClicked()));
    connect(ui->returnButton,
            SIGNAL(clicked()),
            SLOT(ReturnButtonClicked()));

    foreach(QAbstractButton *btn,settingButtonToPage_.keys()){
        connect(btn,SIGNAL(clicked()),
                SLOT(SettingButtonClicked()));
    }

    foreach(QAbstractButton *btn,alarmButtonToPage_.keys()){
        connect(btn,SIGNAL(clicked()),
                SLOT(RecordsButtonClicked()));
    }

}


void MainFrame::BindShortcutKey()
{
    ui->teachButton->setShortcut(keyToMap.value(ICKeyboard::FB_F1));
    ui->ioMonitorButton->setShortcut(keyToMap.value(ICKeyboard::FB_F2));
    ui->recordButton->setShortcut(keyToMap.value(ICKeyboard::FB_F3));
    ui->settingsButton->setShortcut(keyToMap.value(ICKeyboard::FB_F4));
    ui->returnButton->setShortcut(keyToMap.value(ICKeyboard::FB_F5));

    ui->baseButton->setShortcut(keyToMap.value(ICKeyboard::FB_F1));
    ui->axisButton->setShortcut(keyToMap.value(ICKeyboard::FB_F2));
    ui->servoButton->setShortcut(keyToMap.value(ICKeyboard::FB_F3));
    ui->updateButton->setShortcut(keyToMap.value(ICKeyboard::FB_F4));
    ui->SettingReturn->setShortcut(keyToMap.value(ICKeyboard::FB_F5));


}

quint32 MainFrame::GetPointValue(quint16 pos)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    quint32 s  = host->HostStatus(ICVirtualHost::DbgB0).toUInt() << 16;
    s = s + host->HostStatus(ICVirtualHost::DbgA1).toUInt();

    return ( (s >>( (pos -  ICVirtualHost::XPos)* 4 ) )& 0xF);
}

qint32 MainFrame::GetPosValue(ICVirtualHost::ICStatus status)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    qint16  p =  host->HostStatus(status).toInt() ;
    if(p < 0){
        qint32 v = p * 10 -  GetPointValue(status) ;
        return v;
    }
    else{
        qint32 v = p * 10 + GetPointValue(status) ;
        return v;
    }
}

void MainFrame::UpdateTranslate()
{
}

void MainFrame::CategoryButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(clickedButton == ui->settingsButton){
        ui->stackedWidget->setCurrentIndex(1);
        ui->baseButton->click();

    }
    if(clickedButton == ui->recordButton){
        ui->stackedWidget->setCurrentIndex(2);
        ui->alarmButton->click();
    }

    if(functionButtonToPage_.contains(clickedButton))
    {
        centerStackedLayout_->setCurrentWidget(functionButtonToPage_.value(clickedButton));
    }


    if(alarmButtonToPage_.contains(clickedButton))
    {
        centerStackedLayout_->setCurrentWidget(alarmButtonToPage_.value(clickedButton));
    }

}

void MainFrame::RecordsButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(alarmButtonToPage_.contains(clickedButton))
    {
        if(alarmButtonToPage_.value(clickedButton)){
            centerStackedLayout_->setCurrentWidget(alarmButtonToPage_.value(clickedButton));
        }
        else{
            ui->stackedWidget->setCurrentIndex(0);
            ReturnButtonClicked();
        }
    }
}

void MainFrame::SettingButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(settingButtonToPage_.contains(clickedButton))
    {
        if(settingButtonToPage_.value(clickedButton)){
            centerStackedLayout_->setCurrentWidget(settingButtonToPage_.value(clickedButton));
        }
        else{
            ui->stackedWidget->setCurrentIndex(0);
            ReturnButtonClicked();
        }
    }
}

static int32_t oldX = -1;
static int32_t oldY = -1;
static int32_t oldZ = -1;
static int32_t oldQ = -1;
static int32_t oldP = -1;
//static int oldS = -1;
void MainFrame::StatusRefreshed()
{

    static ICAlarmString* alarmString = ICAlarmString::Instance();
    static ICVirtualHost* virtualHost = ICVirtualHost::GlobalVirtualHost();

    ICVirtualHost* host = virtualHost;

    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->xCurrentPos->show();ui->label_7->show();ui->label_22->show();
    }
    else
    {
        ui->xCurrentPos->hide();ui->label_7->hide();ui->label_22->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->yCurrentPos->show();ui->label_11->show();ui->label_9->show();
    }
    else
    {
        ui->yCurrentPos->hide();ui->label_11->hide();ui->label_9->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->zCurrentPos->show();ui->label_17->show();ui->label_10->show();
    }
    else
    {
        ui->zCurrentPos->hide();ui->label_17->hide();ui->label_10->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->rCurrentPos->show();ui->label_8->show();ui->label_14->show();
    }
    else
    {
        ui->rCurrentPos->hide();ui->label_8->hide();ui->label_14->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->tCurrentPos->show();ui->label_20->show();ui->label_19->show();
    }
    else
    {
        ui->tCurrentPos->hide();ui->label_20->hide();ui->label_19->hide();
    }
    int32_t pos = GetPosValue(ICVirtualHost::XPos) ;
    if(pos != oldX)
    {
        oldX = pos;
        ui->xCurrentPos->setText(QString::number(pos / 100.0, 'f', 2));
    }
    int32_t posy = GetPosValue(ICVirtualHost::YPos) ;
    if(posy != oldY)
    {
        oldY = posy;
        ui->yCurrentPos->setText(QString::number(posy / 100.0, 'f', 2));
    }
#ifdef HC_AXIS_COUNT_5
    int32_t posz =  GetPosValue(ICVirtualHost::ZPos) ;
    if(posz != oldZ)
    {
        oldZ = posz;
        ui->zCurrentPos->setText(QString::number(posz / 100.0, 'f', 2));
    }
    pos = GetPosValue(ICVirtualHost::QPos) ;
    if(pos != oldQ)
    {
        oldQ = pos;
        ui->tCurrentPos->setText(QString::number(pos / 100.0, 'f', 2));
    }

    pos =  GetPosValue(ICVirtualHost::PPos) ;
    if(pos != oldP)
    {
        oldP = pos;
        ui->rCurrentPos->setText(QString::number(pos / 100.0, 'f', 2));
        //        ui->rCurrentPos->setText(QString::number(host->HostStatus(ICVirtualHost::DbgY0).toUInt()));
    }
#endif
    //    int speed = host->GlobalSpeed();
    //    if(speed != oldS)
    //    {
    //        oldS = pos;
    //        ui->xSpeedLabel->setText(QString::number((speed >> 8) & 0xFF));
    //        ui->ySpeedLabel->setText(QString::number(speed & 0xFF));
    //#ifdef HC_AXIS_COUNT_5
    //        ui->zSpeedLabel->setText(QString::number((speed >> 16) & 0xFF));
    //        ui->x2SpeedLabel->setText(ui->xSpeedLabel->text());
    //        ui->tSpeedLabel->setText(ui->ySpeedLabel->text());
    //#endif
    //    }

    bool isControled = virtualHost->DoseControled();
    ICProgramHeadFrame::Instance()->ChangeControlStatus(isControled);
    ICProgramHeadFrame::Instance()->RefreshSingleStatus();

    newLedFlags_ = 0;
    newLedFlags_ |= (virtualHost->IsInputOn(64)? 8 : 0);
    newLedFlags_ |= (virtualHost->IsInputOn(67)? 4 : 0);
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
        ioctl(ledFD_, 0, ledFlags_);
#endif
#endif
    }
    //    ioctl(ledFD_, 0, ledFlags_);
    errCode_ = virtualHost->AlarmNum();
    if(isOverTime_)
    {
        errCode_ = 4000;
        ui->cycleTimeAndFinistWidget->SetAlarmInfo("Err" + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));

    }
    if(compareAlarmNums_.indexOf(errCode_) != -1)
    {
        hostCompareDialog_->show();
    }
    ICProgramHeadFrame::Instance()->ChangePunchOrigin(virtualHost->IsInputOn(72));
    int hintCode = virtualHost->HintNum();
    if(alarmString->PriorAlarmNum() != static_cast<int>(errCode_))
    {
        alarmString->SetPriorAlarmNum(errCode_);
        if(errCode_ != 0)
        {
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("Err" + QString::number(errCode_) + ":" + alarmString->AlarmInfo(errCode_));
            QTimer::singleShot(5000, this, SLOT(checkAlarmModify()));
        }
        else{
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("");
        }
    }
    else if((errCode_ == 0) && (hintCode != oldHintCode_))
    {
        oldHintCode_ = hintCode;
        qDebug()<<"hint code"<<hintCode<<alarmString->HintInfo(hintCode);
        if(hintCode != 0)
        {
            ui->cycleTimeAndFinistWidget->SetHintInfo(tr("Hint") + QString::number(hintCode) + ":" + alarmString->HintInfo(hintCode));
        }
        else{
            ui->cycleTimeAndFinistWidget->SetAlarmInfo("");
        }
    }
    //    finishCount_ = virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt();
    //    if(finishCount_ != oldFinishCount_)
    //    {
    //        ui->cycleTimeAndFinistWidget->SetFinished(virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt());
    //        oldFinishCount_ = finishCount_;
    //    }
    if( runningStatus_ == ICVirtualHost::Stop){
        ui->cycleTimeAndFinistWidget->SetCycleTime("0.00");
    }
    else{
        cycleTime_ = virtualHost->HostStatus(ICVirtualHost::Time).toUInt();
        if(cycleTime_ != oldCycleTime_)
        {
            ui->cycleTimeAndFinistWidget->SetCycleTime(QString().sprintf("%.2f", cycleTime_ / qreal(100)));
            oldCycleTime_ = cycleTime_;
        }
    }


    ICProgramHeadFrame::Instance()->ChangeRobotOrigin(IsOrigined());
    runningStatus_ = virtualHost->CurrentStatus();

    if(runningStatus_ == ICVirtualHost::Manual)
    {
        speed_ = virtualHost->HostStatus(ICVirtualHost::DbgX0).toString();
        //        statusStr_ = tr("Manual");
        ICProgramHeadFrame::Instance()->ChangStatusmoldNameLabelOperation(false);
        ICProgramHeadFrame::Instance()->SetHanSelectEnable(true);
        ui->cycleTimeAndFinistWidget->SetFinished(host->FinishProductCount());

        //        ui->teachButton->setEnabled(true);
    }
    else if(runningStatus_ == ICVirtualHost::Stop)
    {
        speed_ = "0";
        ICProgramHeadFrame::Instance()->ChangStatusmoldNameLabelOperation(true);
        ICProgramHeadFrame::Instance()->SetHanSelectEnable(false);
        ui->cycleTimeAndFinistWidget->SetFinished(host->FinishProductCount());

        if(!actionDialog_->isHidden()){
            actionDialog_->hide();
        }

    }
    else if(runningStatus_ == ICVirtualHost::Auto)
    {
        ICProgramHeadFrame::Instance()->ChangStatusmoldNameLabelOperation(false);
        ICProgramHeadFrame::Instance()->SetHanSelectEnable(false);
        //        ui->teachButton->setEnabled(false);
        if((virtualHost->HostStatus(ICVirtualHost::ClipL).toInt() >> 15)
                && ( (errCode_ == 2423)
                     ||   (errCode_ == 2424)
                     ||   (errCode_ == 2410)
                     ||   (errCode_ == 2411)
                     )
                )
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

        if(virtualHost->HasGetFinishProductCount())
        {
            finishCount_ = virtualHost->HostStatus(ICVirtualHost::DbgX1).toUInt();
            //        finishCount_ = virtualHost->FinishProductCount();
            if(finishCount_ != oldFinishCount_)
            {
                ui->cycleTimeAndFinistWidget->SetFinished(finishCount_);
                virtualHost->SetFinishProductCount(finishCount_);
                oldFinishCount_ = finishCount_;
                virtualHost->SaveSystemConfig();
            }
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
    //        ICMessageBox::ICWarning(this, tr("Warning"), tr("Need to origin!"));
    //    }
    //    ui->recordPageButton->setText(tr("Instruct"));
    //    ui->recordPageButton->setEnabled(true);
}

void MainFrame::ShowAutoPage()
{
    centerStackedLayout_->setCurrentWidget(autoPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(tr("Auto")) ;
    nullButton_->click();

    if(!showInfoDialog_)
        return;
    //    if(!IsOrigined())
    //    {
    //        ICMessageBox::ICWarning(this, tr("Warning"), tr("Need to origin!"));
    //    }
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    CanConfig canConfig;
    canConfig.all = host->SystemParameter(ICVirtualHost::SYS_Config_Resv1).toInt();
    if(canConfig.b.canType == 1){
        QStringList ql;
        uint16_t a0 = host->HostStatus(ICVirtualHost::DbgA0).toInt();
        for(int i =0 ;i < 16;i ++){
            if(a0 & (1 << i)){
                ql.append(QString("%1").arg(i+1));
            }
        }
        if(a0){
            infoDialog_->setInfo(tr("Notic Arm %1 Is in Safe Area?").arg( ql.join(QString::fromUtf8("、 ") )));
            infoDialog_->exec();
        }

    }

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
        ICMessageBox::ICWarning(this,
                             tr("Warning"),
                             tr("Controlled, Can't modify!"));
        return;
    }
    //    centerStackedLayout_->setCurrentWidget(settingsPage_);
    //    ICProgramHeadFrame::Instance()->SetCurrentCategoryName(ui->functionPageButton->text());
}

void MainFrame::ShowOrigin()
{
    if(!originExecutingPage_->isVisible() &&
            !isblockOriginShow_ &&
            (ICKeyboard::Instace()->CurrentSwitchStatus() == ICKeyboard::KS_StopStatu))
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
            showInfoDialog_ = 0;
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
        ICMessageBox::ICWarning(this,
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

    static int oldLevel;
    static int oldStatus;

    if((oldLevel == level) && (oldStatus == runningStatus_)) return;
    oldLevel = level;
    oldStatus = runningStatus_;
    ui->axisButton->setEnabled(false);
    ui->servoButton->setEnabled(false);
    switch(level)
    {
    case ICParametersSave::MachineOperator:
    {
        //        ui->functionPageButton->setEnabled(false);
        //        ui->recordPageButton->setEnabled(false);
        ui->settingsButton->setEnabled(false);
#ifdef Q_WS_QWS
        ui->teachButton->setEnabled(false);
#endif

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
        ui->settingsButton->setEnabled(true);
        ui->axisButton->setEnabled(false);
        ui->servoButton->setEnabled(false);

        if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Stop){
            ui->settingsButton->setEnabled(true);
#ifdef Q_WS_QWS
            ui->teachButton->setEnabled(false);
#endif

        }
        else if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Auto){
            ui->settingsButton->setEnabled(false);
#ifdef Q_WS_QWS
            ui->teachButton->setEnabled(false);
#endif
            ui->stackedWidget->setCurrentWidget(ui->page);
        }
        else if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Manual){
            ui->settingsButton->setEnabled(false);
            ui->teachButton->setEnabled(true);
            ui->stackedWidget->setCurrentWidget(ui->page);
        }
    }
        break;
    case ICParametersSave::AdvanceAdmin:
    {
        ui->settingsButton->setEnabled(true);

        if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Stop)
        {
            ui->settingsButton->setEnabled(true);
            ui->axisButton->setEnabled(true);
            ui->servoButton->setEnabled(true);
#ifdef Q_WS_QWS
            ui->teachButton->setEnabled(false);
#endif

        }
        else if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Auto){
            ui->settingsButton->setEnabled(false);
#ifdef Q_WS_QWS
            ui->teachButton->setEnabled(false);
#endif
            ui->stackedWidget->setCurrentWidget(ui->page);
        }
        else if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Manual){
            ui->settingsButton->setEnabled(false);
            ui->teachButton->setEnabled(true);
            ui->stackedWidget->setCurrentWidget(ui->page);
        }
//        else
//        {
//            ui->axisButton->setEnabled(true);
//            ui->servoButton->setEnabled(true);
//        }

    }
        //        if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Stop)
        //        {
        //            ui->settingsButton->setEnabled(true);
        //#ifdef Q_WS_QWS
        //            ui->teachButton->setEnabled(false);
        //#endif

        //            //            settingsPage_->SetToShowAll(true);
        //            //            ui->functionPageButton->setEnabled(true);
        //        }
        //        else
        //        {
        //            ui->settingsButton->setEnabled(false);
        //            //            ui->functionPageButton->setEnabled(false);
        //        }
        //        if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() != ICVirtualHost::Auto)
        //        {
        //            if(ICVirtualHost::GlobalVirtualHost()->CurrentStatus() == ICVirtualHost::Manual){
        //                ui->teachButton->setEnabled(true);
        //                ui->stackedWidget->setCurrentWidget(ui->page);
        //            }
        //            //            ui->recordPageButton->setEnabled(true);
        //        }
        //        else
        //        {
        //#ifdef Q_WS_QWS
        //            ui->teachButton->setEnabled(false);
        //            ui->stackedWidget->setCurrentWidget(ui->page);
        //#endif
        //            //            ui->recordPageButton->setEnabled(false);
        //        }
        //    }
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
        //        system("BackLight off");
        ICUtility::system("BackLight.sh 0");
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
    //    instructPage_->ShowServoAction(key);
}


void MainFrame::OpenBackLight()
{
#ifdef Q_WS_QWS
    ICParametersSave::Instance()->SetBrightness(ICParametersSave::Instance()->Brightness());
#endif
}

void MainFrame::CloseBackLight()
{
#ifdef Q_WS_QWS
    ShowScreenSaver();
    ICUtility::system("BackLight.sh 0");
    SetBackLightOff(true);
#endif
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
    ::system("sync");
}



void MainFrame::OnMoldButtonClicked()
{
    nullButton_->click();
    if(recordPage_ == NULL)
    {
        recordPage_ = MoldInformation::Instance();
        centerStackedLayout_->addWidget(recordPage_);
    }
    centerStackedLayout_->setCurrentWidget(recordPage_);
}

void MainFrame::ShowInfoDialog()
{
    showInfoDialog_ = 1;
}

void MainFrame::OnRegisterChanged()
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
    ICParametersSave::Instance()->SetBootDatetime(QDateTime::currentDateTime());
    ::system("sync");
}

void MainFrame::ClearProduct()
{
    ui->cycleTimeAndFinistWidget->SetFinished(0);
}
