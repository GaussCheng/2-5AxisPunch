#include <QDir>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QMessageBox>

#ifndef Q_WS_WIN32
#include <unistd.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#endif
#include <cstring>


#include "icvirtualhost.h"
#include "icparameterssave.h"
//#include "icmold.h"
#include "icmacrosubroutine.h"
#include "ickeyboardhandler.h"
#include "ickeyboard.h"
#include "icvirtualkey.h"
#include "operatingratiosetdialog.h"
#include <QDebug>
#include <QTime>
#include "icactioncommand.h"
#include "icfile.h"
#include "icprogramheadframe.h"

#define REFRESH_TIME 10
static QTime testTime;

ICVirtualHost* ICVirtualHost::globalVirtualHost_ = NULL;
ICVirtualHost::ICVirtualHost(QObject *parent) :
    QObject(parent),
    currentMold_(new ICMold()),
    subroutines_(ICMacroSubroutine::Instance()),
    flag_(false),
    oldStep_(-1),
    tryTimes_(0),
    currentAddr_(0),
    currentStatus_(0),
    freshCount_(0),
    isInitSuccess_(true),
    flag(true),
    oldMoldNum_(8),
    productCount_(0),
    isParamChanged_(false),
    isFixtureCheck_(true),
    isSingleRun_(false)
{
    memset(oldSubStep, -1, 8);
    input0Bits_ = 0;
    input1Bits_ = 0;
    input2Bits_ = 0;
    input3Bits_ = 0;
    output0Bits_ = 0;
    output1Bits_ = 0;
    output2Bits_ = 0;
    output3Bits_ = 0;
    euInputBits_ = 0;
    euOutputBits_ = 0;
    clipLBits_ = 0;
    clipHBits_ = 0;
    if(GlobalVirtualHost() == NULL)
    {
        SetGlobalVirtualHost(this);
    }

    SetSlaveID(1);
    queryStatusCommand_.SetSlave(SlaveID());
    queryStatusCommand_.SetNumber(4);

    InitSystemMap_();
    InitAddrToSysPosMap_();
    InitICStatusMap_();
    InitSubs_();
    InitMold_();
    InitMoldParam_();
    InitSystem_();
//    SetGlobalSpeed(20);
    //    exit(-1);

    ICMold::SetCurrentMold(currentMold_.data());
    //    QList<ICMoldItem> items;
    //    ICMoldItem item;
    //    item.SetValue(0,0,1,3892,0,0,5,0,1,235);
    //    items.append(item);
    //    item.SetValue(0, 0, 13, 3892, 0, 0, 10, 1000, 5, 228);
    //    items.append(item);
    //    item.SetValue(0, 0, 14, 3892, 0, 0, 10, 1000, 5, 35);
    //    items.append(item);
    //    item.SetValue(0, 0, 17, 3892, 0, 0, 10, 1000, 5, 12);
    //    items.append(item);
    //    item.SetValue(0, 0, 129, 0, 0, 0, 10, 1000, 5, 124);
    //    items.append(item);
    //    item.SetValue(0, 0, 130, 0, 0, 0, 10, 1000, 5, 199);
    //    items.append(item);
    //    item.SetValue(0, 0, 131, 0, 0, 0, 10, 1000, 5, 184);
    //    items.append(item);
    //    item.SetValue(0, 0, 144, 0, 0, 0, 10, 1000, 5, 176);
    //    items.append(item);
    //    item.SetValue(0, 0, 148, 0, 0, 0, 10, 1000, 5, 100);
    //    items.append(item);
    //    item.SetValue(0, 0, 149, 0, 0, 0, 10, 1000, 5, 57);
    //    items.append(item);
    //    ICMold::CurrentMold()->Modify(items);
    if(!isInitSuccess_)
    {
        QMessageBox::critical(NULL, tr("Error"), "Init host fail!");
        return;
    }
//    timer_ = new QTimer();
    connect(currentMold_.data(),
            SIGNAL(MoldPramChanged(int,int)),
            this,
            SLOT(SetMoldParam(int,int)));
    connect(&timer_,
            SIGNAL(timeout()),
            this,
            SLOT(RefreshStatus()));
    //    RefreshStatus();
    //    RefreshStatus();
    //    RefreshStatus();
#ifndef Q_WS_WIN32
//    watchdogFd_ = open("/dev/watchdog",O_RDONLY );
#else
    watchdogFd_ = 0;
#endif
    if(watchdogFd_ < 0)
    {
        qWarning("open watchdog fail!");
    }
//#ifndef Q_WS_X11
//#ifdef HC_ARMV6
//    QTimer::singleShot(REFRESH_TIME, this, SLOT(RefreshStatus()));
//#else
//#ifndef HC_ARMV6
//    timer_.start(20);
//#else
    timer_.start(15);
//#endif
//#endif
//#endif

}

//slots:
void ICVirtualHost::SetMoldParam(int param, int value)
{
//    Q_ASSERT_X(moldParamToAddrPos_.contains(param),
//               "ICVirtualHost::SetMoldParam();",
//               (QString::number(param) + " is not a corrent param").toAscii());
    ICCommandProcessor::Instance()->ModifyMoldParam(moldParamToAddrPos_.value(param), value);
    Q_UNUSED(param)
    Q_UNUSED(value)
    isParamChanged_ = true;
}

static int StatusToKey(int status)
{
    switch(status)
    {
    case ICVirtualHost::AutoReady:
    case ICVirtualHost::AutoRunning:
    case ICVirtualHost::AutoSingleCycle:
    case ICVirtualHost::AutoStopping:
    case ICVirtualHost::Auto:
    {
        return ICKeyboard::KS_AutoStatu;
    }
    case ICVirtualHost::Manual:
    {
        return ICKeyboard::KS_ManualStatu;
    }
    default:
        return ICKeyboard::KS_StopStatu;
    }
}

void ICVirtualHost::RefreshStatus()
{
    static ICCommandProcessor* commandProcess = ICCommandProcessor::Instance();
    static ICKeyboardHandler* keyboardHandler = ICKeyboardHandler::Instance();
    static ICKeyboard* keyboard = ICKeyboard::Instace();

    //    emit StepChanged(rand() % 10);
//    qDebug("refresh");
    if(flag_)
    {
//        qDebug("flag");
        int key = keyboard->TakeKeyValue();
        if(keyboard->IsPressed())
        {
            (key == -1) ? key = oldKey_ : oldKey_ = key;
        }
        if(key == ICKeyboard::VFB_X1Add || key == ICKeyboard::VFB_X1Sub)
        {
            ICProgramHeadFrame::Instance()->ChangeHandSelect(0);
//            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_X);

        }
        else if(key == ICKeyboard::VFB_Y1Add || key == ICKeyboard::VFB_Y1Sub)
        {
            ICProgramHeadFrame::Instance()->ChangeHandSelect(1);
//            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Y);
        }
        else if(key == ICKeyboard::VFB_ZAdd || key == ICKeyboard::VFB_ZSub)
        {
            ICProgramHeadFrame::Instance()->ChangeHandSelect(2);
//            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Z);
        }
        else if(key == ICKeyboard::VFB_X2Add || key == ICKeyboard::VFB_X2Sub)
        {
            ICProgramHeadFrame::Instance()->ChangeHandSelect(3);
//            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Z);
        }
        else if(key == ICKeyboard::VFB_Y2Add || key == ICKeyboard::VFB_Y2Sub)
        {
            ICProgramHeadFrame::Instance()->ChangeHandSelect(4);
//            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Z);
        }
        int swKey = keyboard->TakeSwitchValue();
        if(swKey == -1)
        {
            swKey = keyboard->CurrentSwitchStatus();
//            qDebug()<<"swKey"<<swKey<<CurrentStatus();

            int isCan = SystemParameter(ICVirtualHost::SYS_Config_Resv1).toInt()  & 0xFF;
            if(StatusToKey(CurrentStatus()) != swKey && (isCan) < 2 )
            {
                switch(swKey)
                {
                case ICKeyboard::KS_AutoStatu:
                {
                    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnAuto,
                                                                     0,
                                                                     ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
                                                                     ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());
                }
                    break;
                case ICKeyboard::KS_ManualStatu:
                {
                    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual,
                                                                     0);
                }
                    break;
                default:
                    if(CurrentStatus() != Stop)
                    {
//                        qDebug("Turn Stop Auto");
                        ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
                    }
                }
            }
        }
        else
        {
            keyboardHandler->SwitchChanged(swKey);
        }
//        keyboardHandler->SwitchChanged(keyboard->TakeSwitchValue());
        keyboardHandler->Keypressed(key);
        keyboardHandler->PulleyChanged(keyboard->TakePulleyValue());
        flag_ = false;
        //        qDebug("Run conmand");
    }
    else
    {
//        qDebug("!flag");
        //        qDebug()<<"refresh statys start";
        ICCommunicationCommandBase::ResultVector result;
        currentAddr_ %= 9;
        if(currentAddr_ == 0)
        {
            currentStatus_ = 0;
        }
        queryStatusCommand_.SetStartAddr(currentAddr_);
//        qDebug()<<"Query Time:"<<testTime.restart();

   //     qApp->processEvents();

        result = commandProcess->ExecuteCommand(queryStatusCommand_).value<ICCommunicationCommandBase::ResultVector>();
        if(queryStatusCommand_.NeedToReconfig())
        {
            qDebug("reconfigure");
            QMessageBox::warning(NULL,
                                 tr("Reconfig"),
                                 tr("Need to reconfig"));
            ReConfigure();
            flag_ = true;
//#ifdef HC_ARMV6
//            QTimer::singleShot(REFRESH_TIME, this, SLOT(RefreshStatus()));
//#endif
            return;
        }
        if(!result.isEmpty())  //Success,return to status
        {
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(0));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(1));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(2));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(3));
            tryTimes_ = 0;
            ++currentAddr_;

        }
        else
        {
#ifdef Q_WS_X11
//            statusMap_.insert(Status, rand());
//            statusMap_.insert(Step, rand()  % 100);
//            statusMap_.insert(Input2, rand());
//            statusMap_.insert(Input3, rand());
//            statusMap_.insert(Output2, rand());
//            statusMap_.insert(Output3, rand());
//            statusMap_.insert(ActL, rand());
//            statusMap_.insert(ActH, rand());
//            statusMap_.insert(ClipL, rand());
//            statusMap_.insert(ClipH, rand());
//            statusMap_.insert(Time, rand());
//            statusMap_.insert(XPos, rand());
//            statusMap_.insert(YPos, rand());
//            statusMap_.insert(ZPos, rand());
//            statusMap_.insert(PPos, rand());
//            statusMap_.insert(QPos, rand());
//            statusMap_.insert(S, rand());
//            statusMap_.insert(Input0, rand());
//            statusMap_.insert(Input1, rand());
//            statusMap_.insert(Output0, rand());
//            statusMap_.insert(Output1, rand());
//            statusMap_.insert(EuIn, rand());
//            statusMap_.insert(EuOut, rand());
//            statusMap_.insert(ErrCode, rand());
//            statusMap_.insert(DbgX0, rand());
//            statusMap_.insert(DbgX1, rand());
//            statusMap_.insert(DbgY0, 1);
//            statusMap_.insert(DbgY1, rand());
//            statusMap_.insert(DbgZ0, rand());
//            statusMap_.insert(DbgZ1, rand());
//            statusMap_.insert(DbgP0, rand());
//            statusMap_.insert(DbgP1, rand());
//            statusMap_.insert(DbgQ0, rand());
//            statusMap_.insert(DbgQ1, rand());
            emit StepChanged(statusMap_.value(Step).toInt());
//            if(flag)
//            {
//                statusMap_.insert(ErrCode, 304);
//                flag = FALSE;
//            }
//            else
//            {
//                statusMap_.insert(ErrCode, 0);
//                flag = TRUE ;
//            }


#endif
//            statusMap_.insert(
//            statusMap_.insert(Input0, 1025);
//            statusMap_.insert(YPos, 0);
//            qDebug()<<"Resend time:"<<testTime.restart();
            ++tryTimes_;
//            qCritical()<<"connect to host fail in refresh status"<<tryTimes_;
//            static int test = 0;
            if(tryTimes_  >= 200)
            {
//                qCritical("Connect to host fail!!");
#ifdef Q_WS_X11
                statusMap_.insert(ErrCode, 0);
                statusMap_.insert(XPos, 65530);
                statusMap_.insert(YPos, 65530);
                statusMap_.insert(ZPos, 65530);
                statusMap_.insert(PPos, 65530);
                statusMap_.insert(QPos, 65530);
                statusMap_.insert(DbgY0, 0);
//                statusMap_.insert(APos, rand());
                clipLBits_ = -1;
                clipHBits_ = -1;
//                statusMap_.insert(DbgP0, (2 << 8));
#else
                statusMap_.insert(ErrCode, 500);
#endif
                tryTimes_ = 0;
                statusMap_.insert(Status, ICVirtualHost::Stop);
//                statusMap_.insert(DbgX0, ICVirtualHost::AutoRunning);
                emit StatusRefreshed();
//                emit StepChanged(test++ % 10);
            }
//            flag_ =true;
//            QTimer::singleShot(5, this, SLOT(RefreshStatus()));
//#ifdef HC_ARMV6
//            QTimer::singleShot(REFRESH_TIME, this, SLOT(RefreshStatus()));
//#endif
            return;
        }
        freshCount_ = (freshCount_ + 1) % 2;
        input0Bits_ = statusMap_.value(Input0).toUInt();
        input1Bits_ = statusMap_.value(Input1).toUInt();
        input2Bits_ = statusMap_.value(Input2).toUInt();
        input3Bits_ = statusMap_.value(Input3).toUInt();
        output0Bits_ = statusMap_.value(Output0).toUInt();
        output1Bits_ = statusMap_.value(Output1).toUInt();
        output2Bits_ = statusMap_.value(Output2).toUInt();
        output3Bits_ = statusMap_.value(Output3).toUInt();
        euInputBits_ = statusMap_.value(EuIn).toUInt();
        euOutputBits_ = statusMap_.value(EuOut).toUInt();
        clipLBits_ = statusMap_.value(ClipL).toUInt();
        clipHBits_ = statusMap_.value(ClipH).toUInt();

        actionLBits_ = statusMap_.value(ActL).toUInt();
        actionHBits_ = statusMap_.value(ActH).toUInt();
        if(freshCount_)
        {
            int moldNum = HostStatus(S).toInt();
            if(moldNum != oldMoldNum_)
            {
                oldMoldNum_ = moldNum;
                emit MoldNumChanged(moldNum);
            }
            int tmpStep = CurrentStep();
//            int queriedSubStep[] = {HostStatus(Sub0).toInt(),
//                                    HostStatus(Sub1).toInt(),
//                                    HostStatus(Sub2).toInt(),
//                                    HostStatus(Sub3).toInt()};
//            uint8_t tmpSubStep[] = {queriedSubStep[0] & 0x00FF,
//                                    queriedSubStep[0] >> 8,
//                                    queriedSubStep[1] & 0x00FF,
//                                    queriedSubStep[1] >> 8,
//                                    queriedSubStep[2] & 0x00FF,
//                                    queriedSubStep[2] >> 8,
//                                    queriedSubStep[3] & 0x00FF,
//                                    queriedSubStep[3] >> 8};

            if(tmpStep != oldStep_)
            {
                statusMap_.insert(Step, tmpStep);
                oldStep_ = tmpStep;
                emit StepChanged(tmpStep);
            }

//            if(memcmp(oldSubStep, tmpSubStep, 8) != 0)
//            {
//                memcpy(oldSubStep, tmpSubStep, 8);
//                emit SubStepChanged(tmpSubStep);
//            }


            emit StatusRefreshed();
#ifndef Q_WS_WIN32
//            ioctl(watchdogFd_, WDIOC_KEEPALIVE);
#endif
            flag_ = true;
            //            qDebug("Run query");
        }
    }

//#ifdef HC_ARMV6
//            QTimer::singleShot(REFRESH_TIME, this, SLOT(RefreshStatus()));
//#endif
}

void ICVirtualHost::SaveSystemConfig()
{
//    QFile file("./sysconfig/system.txt");
//    if(!file.open(QFile::WriteOnly | QFile::Text))
//    {
//        qCritical("open system file fail when save!");
//        return;
//    }
    int sum = 0;
    for(ICSystemParameter i = SYS_Function; i != SYS_CheckSum; i = static_cast<ICSystemParameter>(i + 1))
    {
        sum += systemParamMap_.value(i).toUInt();
    }
    sum = (-sum) & 0xFFFF;
    systemParamMap_.insert(SYS_CheckSum, sum);

    QByteArray toWrite;
    for(ICSystemParameter i = SYS_Global_Speed; i != SYS_Param_Count; i = static_cast<ICSystemParameter>(i + 1))
    {
        toWrite += systemParamMap_.value(i).toByteArray() + "\n";
    }
    ICFile file("./sysconfig/system.txt");
    file.ICWrite(toWrite);
//    QFile::copy("./sysconfig/system.txt", "./sysconfig/system.txt~");
//    file.write(toWrite);
//    file.close();
////    system("rm ./sysconfig/system.txt~");
//    QFile::remove("./sysconfig/system.txt~");
}

void ICVirtualHost::SaveAxisParam(int axis)
{
    switch(axis)
    {
    case ICAxis_AxisX1:
        SaveAxisParamHelper_("paramx.txt",
                             SYS_X_Length,
                             SYS_Y_Length);
        break;
    case ICAxis_AxisY1:
        SaveAxisParamHelper_("paramy.txt",
                             SYS_Y_Length,
                             SYS_Z_Length);
        break;
    case ICAxis_AxisZ:
        SaveAxisParamHelper_("paramz.txt",
                             SYS_Z_Length,
                             SYS_P_Length);
        break;
    case ICAxis_AxisX2:
        SaveAxisParamHelper_("paramp.txt",
                             SYS_P_Length,
                             SYS_Q_Length);
        break;
    case ICAxis_AxisY2:
        SaveAxisParamHelper_("paramq.txt",
                             SYS_Q_Length,
                             SYS_A_Length);
        break;
    case ICAxis_AxisA:
        SaveAxisParamHelper_("parama.txt",
                             SYS_A_Length,
                             SYS_B_Length);
        break;
    case ICAxis_AxisB:
        SaveAxisParamHelper_("paramb.txt",
                             SYS_B_Length,
                             SYS_C_Length);
        break;
    case ICAxis_AxisC:
        SaveAxisParamHelper_("paramc.txt",
                             SYS_C_Length,
                             SYS_Config_Signal);
        break;
    }
    isParamChanged_ = true;
}

//private functions
void ICVirtualHost::InitSubs_()
{
    if(!subroutines_->ReadMacroSubroutieFiles("./subs"))
    {
        isInitSuccess_ = false;
        qCritical("Read macro subroutines files fail!");
        return ;
    }

    WriteSubTohost_();

    qDebug("init sub finish");
}

void ICVirtualHost::WriteSubTohost_()
{
    ICMacroSubroutine::ICMacroAllSubroutine allSubs = subroutines_->SubRoutines();
    QList<ICMoldItem> sub;
    ICMoldItem subItem;
    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    ICWriteParameters writeParamtersCommand;
    writeParamtersCommand.SetSlave(1);
    writeParamtersCommand.SetStartAddr(0);
    QVector<uint8_t> dataSection;
    QVector<uint> tempItemValues;
    uint startAddrHi = 0;
    uint startAddrLo;

    for(int i = 0; i != allSubs.size(); ++i)
    {
        sub = allSubs.at(i);
        startAddrLo = 0;
        startAddrHi = startAddrHi << 8;
        for(int j = 0; j != sub.size(); ++j)
        {
            subItem = sub.at(j);

            dataSection.clear();
            AppendDWord_(dataSection, subItem.Seq());
            AppendDWord_(dataSection, subItem.Num());
            dataSection.append(subItem.SubNum());
            dataSection.append(subItem.GMVal());
            AppendDWord_(dataSection, subItem.Pos());
            writeParamtersCommand.SetStartAddr(startAddrHi | (startAddrLo++));
            writeParamtersCommand.SetDataSection(dataSection);
            if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
            {
                qCritical("Connect to host fail when init sub");
                qDebug()<<"init sub fail"<<j;
                break;
                //                exit(-1);
            }

            dataSection.clear();
            dataSection.append(subItem.IFVal());
            AppendDWord_(dataSection, subItem.IFPos());
            AppendDWord_(dataSection, subItem.DVal());
            dataSection.append(subItem.SVal());
            AppendDWord_(dataSection, subItem.Sum());

            writeParamtersCommand.SetStartAddr(startAddrHi | startAddrLo++);
            writeParamtersCommand.SetDataSection(dataSection);
            if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
            {
                qCritical("Connect to host fail when init sub");
                //                exit(-1);
            }
        }
        startAddrHi = startAddrHi >> 8;
        ++startAddrHi;
    }
}

void ICVirtualHost::InitSystem_()
{
    QString path = "./sysconfig/system.txt";
    QFile systemParaFile(path);
    if(!systemParaFile.exists())
    {
        isInitSuccess_ = false;
        path += ".bak";
        systemParaFile.setFileName(path);
    }
    if(!systemParaFile.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open system.txt fail");
        //        exit(-1);
    }
    QString fileContent = systemParaFile.readAll();
    systemParaFile.close();

    QStringList items = fileContent.split("\n", QString::SkipEmptyParts);
    QVector<uint8_t> dataSection;
    QVector<uint> tempItemValues;
    for(int i = 0; i != items.size(); ++i)
    {
        tempItemValues.append(items.at(i).toUInt());
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.last());
    }

    if(tempItemValues.size() < systemParamMap_.size())
    {
        tempItemValues.resize(systemParamMap_.size());
    }
//    tempItemValues.append(0);

    GetAxisParam_("./sysconfig/paramx.txt",
                  SYS_X_Length,
                  SYS_Y_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramy.txt",
                  SYS_Y_Length,
                  SYS_Z_Length,
                  tempItemValues);
#ifdef HC_8AXIS
    GetAxisParam_("./sysconfig/paramz.txt",
                  SYS_Z_Length,
                  SYS_P_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramp.txt",
                  SYS_P_Length,
                  SYS_Q_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramq.txt",
                  SYS_Q_Length,
                  SYS_A_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/parama.txt",
                  SYS_A_Length,
                  SYS_B_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramb.txt",
                  SYS_B_Length,
                  SYS_C_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramc.txt",
                  SYS_C_Length,
                  SYS_Config_Signal,
                  tempItemValues);
#else
    GetAxisParam_("./sysconfig/paramz.txt",
                  SYS_Z_Length,
                  SYS_X_Origin,
                  tempItemValues);
#endif
    const int count = tempItemValues.size();
    if(count % 4 != 0)
    {
        for(int i = 0; i != 4 - (count % 4); ++i)
        {
            tempItemValues.append(0);
        }
    }

    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    ICWriteParameters writeParamtersCommand;
    writeParamtersCommand.SetSlave(1);

    uint startAddr = 0;
    for(int i = 0; i != tempItemValues.size(); ++i)
    {
        dataSection.clear();
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i));
        writeParamtersCommand.SetStartAddr(((((startAddr >> 8) & 0x00FF) | 0x00C0) << 8) | (startAddr & 0X00FF));
        writeParamtersCommand.SetDataSection(dataSection);
        if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
        {
            qCritical("connect to host fail when init system");
            break;
        }
        ++startAddr;
    }
    qDebug("Init sys finish");
}

void ICVirtualHost::WriteSystemTohost_()
{
    QString path = "./sysconfig/system.txt";
    QFile systemParaFile(path);
    if(!systemParaFile.exists())
    {
        isInitSuccess_ = false;
        path += ".bak";
        systemParaFile.setFileName(path);
    }
    if(!systemParaFile.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open system.txt fail");
        //        exit(-1);
    }
    QString fileContent = systemParaFile.readAll();
    systemParaFile.close();

    QStringList items = fileContent.split("\n", QString::SkipEmptyParts);
    QVector<uint8_t> dataSection;
    QVector<uint> tempItemValues;
    for(int i = 0; i != items.size(); ++i)
    {
        tempItemValues.append(items.at(i).toUInt());
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.last());
    }
    tempItemValues.append(0);

    GetAxisParam_("./sysconfig/paramx.txt",
                  SYS_X_Length,
                  SYS_Y_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramy.txt",
                  SYS_Y_Length,
                  SYS_Z_Length,
                  tempItemValues);
#ifdef HC_8AXIS
    GetAxisParam_("./sysconfig/paramz.txt",
                  SYS_Z_Length,
                  SYS_P_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/parapz.txt",
                  SYS_P_Length,
                  SYS_Q_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramq.txt",
                  SYS_Q_Length,
                  SYS_A_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/parama.txt",
                  SYS_A_Length,
                  SYS_B_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramb.txt",
                  SYS_B_Length,
                  SYS_C_Length,
                  tempItemValues);
    GetAxisParam_("./sysconfig/paramc.txt",
                  SYS_C_Length,
                  SYS_X_Origin,
                  tempItemValues);
#else
    GetAxisParam_("./sysconfig/paramz.txt",
                  SYS_Z_Length,
                  SYS_X_Origin,
                  tempItemValues);
#endif

    const int count = tempItemValues.size();
    if(count % 4 != 0)
    {
        for(int i = 0; i != 4 - (count % 4); ++i)
        {
            tempItemValues.append(0);
        }
    }

    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    ICWriteParameters writeParamtersCommand;
    writeParamtersCommand.SetSlave(1);

    uint startAddr = 0;
    for(int i = 0; i != tempItemValues.size(); ++i)
    {
        dataSection.clear();
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i));
        writeParamtersCommand.SetStartAddr(((((startAddr >> 8) & 0x00FF) | 0x00C0) << 8) | (startAddr & 0X00FF));
        writeParamtersCommand.SetDataSection(dataSection);
        if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
        {
            qCritical("connect to host fail when init system");
        }
        ++startAddr;
    }
    qDebug("Init sys finish");
}

void ICVirtualHost::InitMold_()
{
    ICParametersSave* config = ICParametersSave::Instance();
    QString path = "./records/";
    if(config->MoldName("").isEmpty())
    {
        path += "Default.act";
        config->SetMoldName("Default.act");
    }
    else
    {
        path += config->MoldName("");
    }
    QString configPath = path;
    configPath.chop(3);
    configPath += "fnc";
//    Q_ASSERT_X(QFile::exists(path), "ICVirtualHost::InitMold_", "act is not exits");
//    Q_ASSERT_X(QFile::exists(configPath), "ICVirtualHost::InitMold_", "fnc is not exits");
    if(!QFile::exists(path) || !QFile::exists(configPath))
    {
        path = "./records/ERRRT.act";
        isInitSuccess_ = false;
    }
    // TODO remove it
//    currentMold_->SaveMoldParamsFile();
    if(!currentMold_->ReadMoldFile(path))
    {
        qDebug("Init Mold fail!");
        return;
    }

    WriteMoldTohost_();

    qDebug("Init mold finish");
}

void ICVirtualHost::WriteMoldTohost_()
{
    QVector<uint8_t> dataSection;
    QList<ICMoldItem> moldContent = currentMold_->MoldContent();
    ICMoldItem moldItem;
    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    ICWriteParameters writeParamtersCommand;
    writeParamtersCommand.SetSlave(1);
    int startAddr = 0;
    for(int i = 0; i != moldContent.size(); ++i)
    {
        dataSection.clear();
        moldItem = moldContent.at(i);
        AppendDWord_(dataSection, moldItem.Seq());
        AppendDWord_(dataSection, moldItem.Num());
        dataSection.append(moldItem.SubNum());
        dataSection.append(moldItem.GMVal());
        AppendDWord_(dataSection, moldItem.Pos());

        writeParamtersCommand.SetStartAddr(((((startAddr >> 8) & 0x00FF) | 0x0040) << 8) | (startAddr & 0x00FF));
        writeParamtersCommand.SetDataSection(dataSection);
        //        qDebug()<<dataSection;

        if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
        {
            qCritical("Connect to host fail when init mold");
            break;
            //            exit(-1);
        }

        ++startAddr;
        dataSection.clear();

        dataSection.append(moldItem.IFVal());
        AppendDWord_(dataSection, moldItem.IFPos());
        AppendDWord_(dataSection, moldItem.DVal());
        dataSection.append(moldItem.SVal());
        AppendDWord_(dataSection, moldItem.Sum());

        writeParamtersCommand.SetStartAddr(((((startAddr >> 8) & 0x00FF) | 0x0040) << 8) | (startAddr & 0x00FF));
        writeParamtersCommand.SetDataSection(dataSection);
        //        qDebug()<<dataSection;

        if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
        {
            qCritical("Connect to host fail when init mold");
            //            exit(-1);
        }
        ++startAddr;
    }
}

void ICVirtualHost::InitMoldParam_()
{
    //    ICSystemConfig config;
    //    if(!currentMold_->ReadMoldParamsFile(":/sysconfig/" + config.MoldName() + ".fnc"))
    //    {
    //        qCritical("open fnc fail fail");
    //        return ;
    //    }

    QVector<uint8_t> dataSection;
    QList<int> tempItemValues = currentMold_->AllParams();
//    int finishedCount = FinishProductCount();
//    tempItemValues[ICMold::Product] -=

    const int count = tempItemValues.size();
    if(count % 4 != 0)
    {
        for(int i = 0; i != 4 - (count % 4); ++i)
        {
            tempItemValues.append(0);
        }
    }

    ICCommandProcessor* commandProcessor = ICCommandProcessor::Instance();
    ICWriteParameters writeParamtersCommand;
    writeParamtersCommand.SetSlave(1);

    uint startAddr = 0;
    for(int i = 0; i != tempItemValues.size(); ++i)
    {
        dataSection.clear();
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i++));
        AppendDWord_(dataSection, tempItemValues.at(i));
        writeParamtersCommand.SetStartAddr(((((startAddr >> 8) & 0x00FF) | 0x80) << 8) | (startAddr & 0X00FF));
        writeParamtersCommand.SetDataSection(dataSection);
        if(!commandProcessor->ExecuteCommand(writeParamtersCommand).toBool())
        {
            qCritical("connect to host fail when init mold param");
            break;
        }
        ++startAddr;
    }
    qDebug("Init mold param finish");
}

void ICVirtualHost::InitICStatusMap_()
{
    for(ICStatus i = Status; i != StatusCount; i = static_cast<ICStatus>(i + 1))
    {
        statusMap_.insert(i, 0);
    }
    statusMap_.insert(ErrCode, 0);
    qDebug()<<statusMap_.size();
}

void ICVirtualHost::InitSystemMap_()
{
    for(ICSystemParameter i = SYS_Global_Speed; i != SYS_Param_Count; i = static_cast<ICSystemParameter>(i + 1))
    {
        systemParamMap_.insert(i, 0);
    }
}

void ICVirtualHost::InitAddrToSysPosMap_()
{
    addrToSysPos_.insert(SM_ACCTIME, SYS_AccTime);
    addrToSysPos_.insert(SM_XLMT, SYS_X_Length);			//X极限
    addrToSysPos_.insert(SM_XMAX, SYS_X_Maxium);			//X最大值
    addrToSysPos_.insert(SM_YLMT, SYS_Y_Length);			//Y极限
    addrToSysPos_.insert(SM_YMAX, SYS_Y_Maxium);			//Y极限
    addrToSysPos_.insert(SM_ZLMT, SYS_Z_Length);			//Z极限
    addrToSysPos_.insert(SM_ZMAX, SYS_Z_Maxium);			//Z极限
    addrToSysPos_.insert(SM_ZINSAFE, SYS_Z_InSafe);			//型内安全区
    addrToSysPos_.insert(SM_ZOUTSAFE, SYS_Z_OutSafe);		//型外安全区
    addrToSysPos_.insert(SM_XINSAFE, SYS_X_InSafe);
    addrToSysPos_.insert(SM_XOUTSAFE, SYS_X_OutSafe);
    addrToSysPos_.insert(SM_YINSAFE, SYS_Y_InSafe);
    addrToSysPos_.insert(SM_YOUTSAFE, SYS_Y_OutSafe);
    addrToSysPos_.insert(SM_ALLOWANCE, SYS_ElapseTol);			//容差
    addrToSysPos_.insert(SM_TIMEHORI, ACT_PoseHori);
    addrToSysPos_.insert(SM_TIMEVERT, ACT_PoseVert);
    addrToSysPos_.insert(SM_TIMEVERT2, ACT_PoseVert2);
    addrToSysPos_.insert(SM_TIMEHORI2, ACT_PoseHori2);
    addrToSysPos_.insert(SM_TIMEVUP, ACT_ViceUp);
    addrToSysPos_.insert(SM_TIMEVDOWN, ACT_ViceDown);
    addrToSysPos_.insert(SM_TIMEVFOR, ACT_ViceForward);
    addrToSysPos_.insert(SM_TIMEVBACK, ACT_ViceBackward);
    addrToSysPos_.insert(SM_Global_Speed, SYS_Global_Speed);
    addrToSysPos_.insert(SM_ACCTIMEX, SYS_AccTimeX);
    addrToSysPos_.insert(SM_ACCTIMEY, SYS_AccTimeY);
    addrToSysPos_.insert(SM_ACCTIMEZ, SYS_AccTimeZ);
    addrToSysPos_.insert(SM_ACCTIMEP, SYS_AccTimeP);
    addrToSysPos_.insert(SM_ACCTIMEQ, SYS_AccTimeQ);
    addrToSysPos_.insert(SM_ACCTIMEA, SYS_AccTimeA);
    addrToSysPos_.insert(SM_ACCTIMEB, SYS_AccTimeB);
    addrToSysPos_.insert(SM_ACCTIMEC, SYS_AccTimeC);
    addrToSysPos_.insert(SM_LMTVX, SYS_LMT_vX);
    addrToSysPos_.insert(SM_LMTVY, SYS_LMT_vY);
    addrToSysPos_.insert(SM_LMTVZ, SYS_LMT_vZ);
    addrToSysPos_.insert(SM_LMTVP, SYS_LMT_vP);
    addrToSysPos_.insert(SM_LMTVQ, SYS_LMT_vQ);
    addrToSysPos_.insert(SM_LMTVA, SYS_LMT_vA);
    addrToSysPos_.insert(SM_LMTVB, SYS_LMT_vB);
    addrToSysPos_.insert(SM_LMTVC, SYS_LMT_vC);
    addrToSysPos_.insert(SM_ARM_CONFIG, SYS_ARM_CONFIG);
    addrToSysPos_.insert(SM_YEarlyEnd, SYS_YEarlyEnd);
//    addrToSysPos_.insert(SM_ZEarlyEnd, SYS_ZEarlyEnd);
    addrToSysPos_.insert(SM_WaitMoldOpenLimit, ACT_WaitMoldOpened);
    addrToSysPos_.insert(SM_TIMEMFOR, ACT_MainForward);
    addrToSysPos_.insert(SM_TIMEMBACK, ACT_MainBackward);
    addrToSysPos_.insert(SM_TIMEMUP, ACT_MainUp);
    addrToSysPos_.insert(SM_TIMEMDOWN, ACT_MainDown);
    addrToSysPos_.insert(SM_PullPushDistance, SYS_PullPushDistance);
//    addrToSysPos_.insert(SM_X_SEC1, ACT_X_Sec1);
    addrToSysPos_.insert(SM_X_SEC2, ACT_X_Sec2);
    addrToSysPos_.insert(SM_X_SEC3, ACT_X_Sec3);
    addrToSysPos_.insert(SM_X_SEC4, ACT_X_Sec4);
    addrToSysPos_.insert(SM_Y_SEC1, ACT_Y_Sec1);
    addrToSysPos_.insert(SM_Y_SEC2, ACT_Y_Sec2);
    addrToSysPos_.insert(SM_Y_SEC3, ACT_Y_Sec3);
    addrToSysPos_.insert(SM_Y_SEC4, ACT_Y_Sec4);
    addrToSysPos_.insert(SM_Z_SEC1, ACT_Z_Sec1);
    addrToSysPos_.insert(SM_Z_SEC2, ACT_Z_Sec2);
    addrToSysPos_.insert(SM_Z_SEC3, ACT_Z_Sec3);
    addrToSysPos_.insert(SM_Z_SEC4, ACT_Z_Sec4);
    addrToSysPos_.insert(SM_P_SEC1, ACT_P_Sec1);
    addrToSysPos_.insert(SM_P_SEC2, ACT_P_Sec2);
    addrToSysPos_.insert(SM_P_SEC3, ACT_P_Sec3);
    addrToSysPos_.insert(SM_P_SEC4, ACT_P_Sec4);
    addrToSysPos_.insert(SM_Q_SEC1, ACT_Q_Sec1);
    addrToSysPos_.insert(SM_Q_SEC2, ACT_Q_Sec2);
    addrToSysPos_.insert(SM_Q_SEC3, ACT_Q_Sec3);
    addrToSysPos_.insert(SM_Q_SEC4, ACT_Q_Sec4);
    addrToSysPos_.insert(SM_A_SEC1, ACT_A_Sec1);
    addrToSysPos_.insert(SM_A_SEC2, ACT_A_Sec2);
    addrToSysPos_.insert(SM_A_SEC3, ACT_A_Sec3);
    addrToSysPos_.insert(SM_A_SEC4, ACT_A_Sec4);
    addrToSysPos_.insert(SM_B_SEC1, ACT_B_Sec1);
    addrToSysPos_.insert(SM_B_SEC2, ACT_B_Sec2);
    addrToSysPos_.insert(SM_B_SEC3, ACT_B_Sec3);
    addrToSysPos_.insert(SM_B_SEC4, ACT_B_Sec4);
    addrToSysPos_.insert(SM_C_SEC1, ACT_C_Sec1);
    addrToSysPos_.insert(SM_C_SEC2, ACT_C_Sec2);
    addrToSysPos_.insert(SM_C_SEC3, ACT_C_Sec3);
    addrToSysPos_.insert(SM_C_SEC4, ACT_C_Sec4);
    for(int i = 0; i != 33; ++i)
    {
        moldParamToAddrPos_.insert(ICMold::point0 + i, static_cast<ICSystemParameterAddr>(SM_Position_DOWN + i));
    }
//    moldParamToAddrPos_.insert(ICMold::CheckClip2, SM_CHKCLIP2);
//    moldParamToAddrPos_.insert(ICMold::CheckClip3, SM_CHKCLIP3);
//    moldParamToAddrPos_.insert(ICMold::CheckClip4, SM_CHKCLIP4);
//    moldParamToAddrPos_.insert(ICMold::CheckClip5, SM_CHKCLIP5);
//    moldParamToAddrPos_.insert(ICMold::CheckClip6, SM_CHKCLIP6);
//    moldParamToAddrPos_.insert(ICMold::CheckClip7, SM_CHKCLIP7);
//    moldParamToAddrPos_.insert(ICMold::CheckClip8, SM_CHKCLIP8);
//    moldParamToAddrPos_.insert(ICMold::Product, SM_PRODUCT);			//设定产量
//    moldParamToAddrPos_.insert(ICMold::CountUnit, SM_Position_DOWN);

//    moldParamToAddrPos_.insert(ICMold::StandbyPose, SM_StandBy);  //待机姿势
//    moldParamToAddrPos_.insert(ICMold::TryProduct, SM_TryProduct); //试产
//    moldParamToAddrPos_.insert(ICMold::Sampling, SM_Sampling); //取样
//    moldParamToAddrPos_.insert(ICMold::)

    //    addrToSysPos_.insert(SM_MAINUP, ACT_MainUp);			//主上限制
    //    addrToSysPos_.insert(SM_MAINDOWN, ACT_MainDown);		//主下限制
    //    addrToSysPos_.insert(SM_MAININ, ACT_MainBackward);			//主入限制
    //    addrToSysPos_.insert(SM_MAINOUT, ACT_MainForward);			//主出限制
    //    addrToSysPos_.insert(SM_VICEUP,	ACT_ViceUp);		//副上限制
    //    addrToSysPos_.insert(SM_VICEDOWN, ACT_ViceDown);		//副下限制
    //    addrToSysPos_.insert(SM_VICEIN,	ACT_ViceBackward);		//副入限制
    //    addrToSysPos_.insert(SM_VICEOUT, ACT_ViceForward);			//副出限制
    //    addrToSysPos_.insert(SM_LAYSEQ,	ACT		//置放顺序
    //    SM_LAYXARRAY,		//置放点阵X
    //    SM_LAYYARRAY,		//置放点阵Y
    //    SM_LAYZARRAY,		//置放点阵Z
    //    SM_LAYXGAP,			//置放长度X
    //    SM_LAYYGAP,			//置放长度Y
    //    SM_LAYZGAP,			//置放长度Z
    //    SM_LAYXCNT,			//置放计数X
    //    SM_LAYYCNT,			//置放计数Y
    //    SM_LAYZCNT,			//置放计数Z
}

void ICVirtualHost::GetAxisParam_(const QString &file, int start, int end, QVector<uint> &tmp)
{
    QFile paramFile(file);
    if(!paramFile.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMX.TXT fail");
        //        exit(-1);
    }
    QString fileContent = paramFile.readAll();
    paramFile.close();
    QStringList items = fileContent.split('\n', QString::SkipEmptyParts);
    int itemsIndex = 0;
    for(int i = start; i != end; ++i)
    {
        tmp[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tmp.at(i));
    }
}

void ICVirtualHost::SaveAxisParamHelper_(const QString &fileName, int start, int end)
{
    QByteArray toWrite;
    for(ICSystemParameter i = static_cast<ICSystemParameter>(start); i != static_cast<ICSystemParameter>(end); i = static_cast<ICSystemParameter>(i + 1))
    {
        toWrite += systemParamMap_.value(i).toByteArray() + "\n";
    }
    ICFile file("./sysconfig/" + fileName);
    file.ICWrite(toWrite);
//    QFile file("./sysconfig/" + fileName);
//    if(file.open(QFile::WriteOnly | QFile::Text))
//    {
//        QFile::copy("./sysconfig/" + fileName, "./sysconfig/" + fileName + "~");
//        file.write(toWrite);
//        file.close();
////        system(QString("rm ./sysconfig/%1~").arg(fileName).toAscii());
//        QFile::remove("./sysconfig/" + fileName + "~");
//    }
}

void ICVirtualHost::StopRefreshStatus()
{
//    timer_.blockSignals(true);
//    timer_.stop();
    this->blockSignals(true);
}

void ICVirtualHost::RestartRefreshStatus()
{
//    timer_.blockSignals(false);
//    timer_.start(15);
    this->blockSignals(false);
}

int ICVirtualHost::FinishProductCount() const
{

    if(ICKeyboard::Instace()->CurrentSwitchStatus() != ICKeyboard::KS_AutoStatu)
    {
        return (systemParamMap_.value(ICVirtualHost::SYS_RsvWorkmold).toUInt());
    }
    return (statusMap_.value(DbgX1).toUInt() );

}

ICVirtualHost::~ICVirtualHost(){}
