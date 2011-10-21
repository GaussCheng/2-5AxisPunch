#include <unistd.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <cstring>

#include <QDir>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QMessageBox>

#include "icvirtualhost.h"
#include "icparameterssave.h"
//#include "icmold.h"
#include "icmacrosubroutine.h"
#include "ickeyboardhandler.h"
#include "ickeyboard.h"
#include "icvirtualkey.h"
#include "operatingratiosetdialog.h"
#include <QDebug>

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
    oldMoldNum_(8),
    productCount_(0)
{
    memset(oldSubStep, -1, 8);
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
    SetGlobalSpeed(20);
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
    timer_ = new QTimer();
    connect(currentMold_.data(),
            SIGNAL(MoldPramChanged(int,int)),
            this,
            SLOT(SetMoldParam(int,int)));
    connect(timer_,
            SIGNAL(timeout()),
            this,
            SLOT(RefreshStatus()));
    //    RefreshStatus();
    //    RefreshStatus();
    //    RefreshStatus();
    watchdogFd_ = open("/dev/watchdog",O_RDONLY );
    if(watchdogFd_ < 0)
    {
        qWarning("open watchdog fail!");
    }
    timer_->start(10);

    //    QTimer::singleShot(20, this, SLOT(RefreshStatus()));
}

//slots:
void ICVirtualHost::SetMoldParam(int param, int value)
{
    Q_ASSERT_X(moldParamToAddrPos_.contains(param),
               "ICVirtualHost::SetMoldParam();",
               (QString::number(param) + " is not a corrent param").toAscii());
    ICCommandProcessor::Instance()->ModifyMoldParam(moldParamToAddrPos_.value(param), value);
}

void ICVirtualHost::RefreshStatus()
{
    //    qApp->processEvents();
    //    if(rand() % 10 == 8)
    //    {
    //    emit MoldNumChanged(0);
    //    }
    //    else if(rand() % 10 == 5)
    //    {
    //        emit MoldNumChanged(8);
    //    }
    //    emit StepChanged(rand() % 10);
    //    uint8_t subs[] = {rand() % 10, rand() % 10, rand() % 10, rand() % 10};
    //    emit SubStepChanged(subs);
    static ICCommandProcessor* commandProcess = ICCommandProcessor::Instance();
    static ICKeyboardHandler* keyboardHandler = ICKeyboardHandler::Instance();
    static ICKeyboard* keyboard = ICKeyboard::Instace();
    //    emit StepChanged(rand() % 10);
    if(flag_)
    {
        int key = keyboard->TakeKeyValue();
        if(keyboard->IsPressed())
        {
            (key == -1) ? key = oldKey_ : oldKey_ = key;
        }
        if(key == ICKeyboard::VFB_X1Add || key == ICKeyboard::VFB_X1Sub)
        {
            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_X);
        }
        else if(key == ICKeyboard::VFB_Y1Add || key == ICKeyboard::VFB_Y1Sub)
        {
            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Y);
        }
        else if(key == ICKeyboard::VFB_ZAdd || key == ICKeyboard::VFB_ZSub)
        {
            OperatingRatioSetDialog::Instance()->SetCurrentHandwheelAxis(OperatingRatioSetDialog::Axis_Z);
        }
        keyboardHandler->SwitchChanged(keyboard->TakeSwitchValue());
        keyboardHandler->Keypressed(key);
        keyboardHandler->PulleyChanged(keyboard->TakePulleyValue());
        flag_ = false;
        //        qDebug("Run conmand");
    }
    else
    {
        //        qDebug()<<"refresh statys start";
        ICCommunicationCommandBase::ResultVector result;
        currentAddr_ %= 7;
        if(currentAddr_ == 0)
        {
            currentStatus_ = 0;
        }
        queryStatusCommand_.SetStartAddr(currentAddr_);
        result = commandProcess->ExecuteCommand(queryStatusCommand_).value<ICCommunicationCommandBase::ResultVector>();
        if(queryStatusCommand_.NeedToReconfig())
        {
            qDebug("reconfigure");
            ReConfigure();
            flag_ = true;
            return;
        }
        //        else if(queryStatusCommand_.NeedToGetTeach())
        //        {
        //            emit NeedToGetTeach();
        //            return;
        //        }
        if(!result.isEmpty())
        {
            //                currentStatus_ %= StatusCount;
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(0));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(1));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(2));
            statusMap_.insert(static_cast<ICStatus>(currentStatus_++), result.at(3));
            tryTimes_ = 0;
            ++currentAddr_;

        }
        else
        {
            ++tryTimes_;
            //            qCritical()<<"connect to host fail in refresh status"<<tryTimes_;
//            static int test = 0;
            if(tryTimes_ == 200)
            {
                qCritical("Connect to host fail!!");
                statusMap_.insert(ErrCode, 200);
                tryTimes_ = 0;
//                statusMap_.insert(Status, ICVirtualHost::Stop);
//                statusMap_.insert(DbgX0, ICVirtualHost::AutoRunning);
                emit StatusRefreshed();
//                emit StepChanged(test++ % 10);
            }
//            flag_ =true;
            return;
        }
        freshCount_ = (++freshCount_) % 2;
        input0Bits_ = statusMap_.value(Input0).toUInt();
        input1Bits_ = statusMap_.value(Input1).toUInt();
        output0Bits_ = statusMap_.value(Output0).toUInt();
        output1Bits_ = statusMap_.value(Output1).toUInt();
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
            int queriedSubStep[] = {HostStatus(Sub0).toInt(),
                                    HostStatus(Sub1).toInt(),
                                    HostStatus(Sub2).toInt(),
                                    HostStatus(Sub3).toInt()};
            uint8_t tmpSubStep[] = {queriedSubStep[0] & 0x00FF,
                                    queriedSubStep[0] >> 8,
                                    queriedSubStep[1] & 0x00FF,
                                    queriedSubStep[1] >> 8,
                                    queriedSubStep[2] & 0x00FF,
                                    queriedSubStep[2] >> 8,
                                    queriedSubStep[3] & 0x00FF,
                                    queriedSubStep[3] >> 8};
            if(tmpStep != oldStep_)
            {
                statusMap_.insert(Step, tmpStep);
                oldStep_ = tmpStep;
                emit StepChanged(tmpStep);
            }

            if(memcmp(oldSubStep, tmpSubStep, 8) != 0)
            {
                memcpy(oldSubStep, tmpSubStep, 8);
                emit SubStepChanged(tmpSubStep);
            }

            emit StatusRefreshed();
            ioctl(watchdogFd_, WDIOC_KEEPALIVE);
            flag_ = true;
            //            qDebug("Run query");
        }
    }
}

void ICVirtualHost::SaveSystemConfig()
{
    QFile file("./sysconfig/system.txt");
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qCritical("open system file fail when save!");
        return;
    }
    int sum = 0;
    for(ICSystemParameter i = SYS_Language; i != SYS_CheckSum; i = static_cast<ICSystemParameter>(i + 1))
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
    file.write(toWrite);
    file.close();
}

void ICVirtualHost::SaveAxisParam()
{
    QFile file("./sysconfig/paramx.txt");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QByteArray toWrite;
        for(ICSystemParameter i = SYS_X_Length; i != SYS_Y_Length; i = static_cast<ICSystemParameter>(i + 1))
        {
            toWrite += systemParamMap_.value(i).toByteArray() + "\n";
        }
        file.write(toWrite);
        file.close();
    }

    file.setFileName("./sysconfig/paramy.txt");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QByteArray toWrite;
        for(ICSystemParameter i = SYS_Y_Length; i != SYS_Z_Length; i = static_cast<ICSystemParameter>(i + 1))
        {
            toWrite += systemParamMap_.value(i).toByteArray() + "\n";
        }
        file.write(toWrite);
        file.close();
    }

    file.setFileName("./sysconfig/paramz.txt");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QByteArray toWrite;
        for(ICSystemParameter i = SYS_Z_Length; i != SYS_X_Origin; i = static_cast<ICSystemParameter>(i + 1))
        {
            toWrite += systemParamMap_.value(i).toByteArray() + "\n";
        }
        file.write(toWrite);
        file.close();
    }
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
    tempItemValues.append(0);

    QFile xParam("./sysconfig/paramx.txt");
    if(!xParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMX.TXT fail");
        //        exit(-1);
    }
    fileContent = xParam.readAll();
    xParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    int itemsIndex = 0;
    for(int i = SYS_X_Length; i != SYS_Y_Length; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

    QFile yParam("./sysconfig/paramy.txt");
    if(!yParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMY.TXT fail");
        //        exit(-1);
    }
    fileContent = yParam.readAll();
    yParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    itemsIndex = 0;
    for(int i = SYS_Y_Length; i != SYS_Z_Length; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

    QFile zParam("./sysconfig/paramz.txt");
    if(!zParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMZ.TXT fail");
        //        exit(-1);
    }
    fileContent = zParam.readAll();
    zParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    itemsIndex = 0;
    for(int i = SYS_Z_Length; i != SYS_X_Origin; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

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

    QFile xParam("./sysconfig/paramx.txt");
    if(!xParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMX.TXT fail");
        //        exit(-1);
    }
    fileContent = xParam.readAll();
    xParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    int itemsIndex = 0;
    for(int i = SYS_X_Length; i != SYS_Y_Length; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

    QFile yParam("./sysconfig/paramy.txt");
    if(!yParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMY.TXT fail");
        //        exit(-1);
    }
    fileContent = yParam.readAll();
    yParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    itemsIndex = 0;
    for(int i = SYS_Y_Length; i != SYS_Z_Length; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

    QFile zParam("./sysconfig/paramz.txt");
    if(!zParam.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical("Open PARAMZ.TXT fail");
        //        exit(-1);
    }
    fileContent = zParam.readAll();
    zParam.close();
    items = fileContent.split('\n', QString::SkipEmptyParts);
    itemsIndex = 0;
    for(int i = SYS_Z_Length; i != SYS_X_Origin; ++i)
    {
        tempItemValues[i] = items.at(itemsIndex++).toUInt();
        systemParamMap_.insert(static_cast<ICSystemParameter>(i), tempItemValues.at(i));
    }

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
    QString path = "./records/" + config->MoldName("TEST.act");
    QString configPath = path;
    configPath.chop(3);
    configPath += "fnc";
    Q_ASSERT_X(QFile::exists(path), "ICVirtualHost::InitMold_", "act is not exits");
    Q_ASSERT_X(QFile::exists(configPath), "ICVirtualHost::InitMold_", "fnc is not exits");
    if(!QFile::exists(path) || !QFile::exists(configPath))
    {
        path = "./records/ERRRT.act";
        isInitSuccess_ = false;
    }
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
        statusMap_.insert(i, -1);
    }
    statusMap_.insert(ErrCode, 0);
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
    addrToSysPos_.insert(SM_TIMECLIP1, ACT_CLIP_1On);		//夹限时1
    addrToSysPos_.insert(SM_TIMECLIP2, ACT_CLIP_2On);	//夹限时2
    addrToSysPos_.insert(SM_TIMECLIP3, ACT_CLIP_3On);
    addrToSysPos_.insert(SM_TIMECLIP4, ACT_CLIP_4On);
    addrToSysPos_.insert(SM_TIMECLIP5, ACT_CLIP_5On);
    addrToSysPos_.insert(SM_TIMECLIP6, ACT_CLIP_6On);
    addrToSysPos_.insert(SM_TIMECLIP7, ACT_CLIP_7On);
    addrToSysPos_.insert(SM_TIMECLIP8, ACT_CLIP_8On);
    addrToSysPos_.insert(SM_TIMEPUT1, ACT_CLIP_1Off);		//放限时1
    addrToSysPos_.insert(SM_TIMEPUT2, ACT_CLIP_2Off);
    addrToSysPos_.insert(SM_TIMEPUT3, ACT_CLIP_3Off);
    addrToSysPos_.insert(SM_TIMEPUT4, ACT_CLIP_4Off);
    addrToSysPos_.insert(SM_TIMEPUT5, ACT_CLIP_5Off);
    addrToSysPos_.insert(SM_TIMEPUT6, ACT_CLIP_6Off);
    addrToSysPos_.insert(SM_TIMEPUT7, ACT_CLIP_7Off);
    addrToSysPos_.insert(SM_TIMEPUT8, ACT_CLIP_8Off);
    addrToSysPos_.insert(SM_TIMEHORI, ACT_PoseHori);
    addrToSysPos_.insert(SM_TIMEVERT, ACT_PoseVert);
    addrToSysPos_.insert(SM_TIMEVUP, ACT_ViceUp);
    addrToSysPos_.insert(SM_TIMEVDOWN, ACT_ViceDown);
    addrToSysPos_.insert(SM_TIMEVFOR, ACT_ViceForward);
    addrToSysPos_.insert(SM_TIMEVBACK, ACT_ViceBackward);
    addrToSysPos_.insert(SM_Global_Speed, SYS_Global_Speed);
    addrToSysPos_.insert(SM_ACCTIMEX, SYS_AccTimeX);
    addrToSysPos_.insert(SM_ACCTIMEY, SYS_AccTimeY);
    addrToSysPos_.insert(SM_ACCTIMEZ, SYS_AccTimeZ);
    addrToSysPos_.insert(SM_LMTVX, SYS_LMT_vX);
    addrToSysPos_.insert(SM_LMTVY, SYS_LMT_vY);
    addrToSysPos_.insert(SM_LMTVZ, SYS_LMT_vZ);
    addrToSysPos_.insert(SM_ARM_CONFIG, SYS_ARM_CONFIG);
    addrToSysPos_.insert(SM_YEarlyEnd, SYS_YEarlyEnd);
    addrToSysPos_.insert(SM_ZEarlyEnd, SYS_ZEarlyEnd);
    addrToSysPos_.insert(SM_WaitMoldOpenLimit, ACT_WaitMoldOpened);
    addrToSysPos_.insert(SM_TIMEMFOR, ACT_MainForward);
    addrToSysPos_.insert(SM_TIMEMBACK, ACT_MainBackward);
    moldParamToAddrPos_.insert(ICMold::CheckClip1, SM_CHKCLIP1);
    moldParamToAddrPos_.insert(ICMold::CheckClip2, SM_CHKCLIP2);
    moldParamToAddrPos_.insert(ICMold::CheckClip3, SM_CHKCLIP3);
    moldParamToAddrPos_.insert(ICMold::CheckClip4, SM_CHKCLIP4);
    moldParamToAddrPos_.insert(ICMold::CheckClip5, SM_CHKCLIP5);
    moldParamToAddrPos_.insert(ICMold::CheckClip6, SM_CHKCLIP6);
    moldParamToAddrPos_.insert(ICMold::CheckClip7, SM_CHKCLIP7);
    moldParamToAddrPos_.insert(ICMold::CheckClip8, SM_CHKCLIP8);
    moldParamToAddrPos_.insert(ICMold::Product, SM_PRODUCT);			//设定产量
    moldParamToAddrPos_.insert(ICMold::PosMainDown, SM_Position_DOWN);
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


ICVirtualHost::~ICVirtualHost(){}
