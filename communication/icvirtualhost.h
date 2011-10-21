#ifndef ICVIRTUALHOST_H
#define ICVIRTUALHOST_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QVector>
#include <stdint.h>
#include <QScopedPointer>
#include <QApplication>
//#include <QBitArray>

#include "iccommands.h"
#include "iccommandprocessor.h"
#include "icmold.h"

class QTimer;
class ICMacroSubroutine;
//class ICMold;

class ICQueryStatus;

class ICVirtualHost : public QObject
{
    Q_OBJECT
public:
    enum ICSystemParameter
    {
        SYS_Global_Speed,
        SYS_RsvReadMold,
        SYS_RsvWorkmold,
        SYS_Language,
        SYS_Function,
        SYS_AccTime,
        SYS_ElapseTol,
        SYS_AccTimeX,
        SYS_AccTimeY,
        SYS_AccTimeZ,
        SYS_LMT_vX,
        SYS_LMT_vY,
        SYS_LMT_vZ,
        SYS_ARM_CONFIG,
        SYS_YEarlyEnd,
        SYS_ZEarlyEnd,

        SYS_X_Length,
        SYS_X_Maxium,
        SYS_X_InSafe,
        SYS_X_OutSafe,
        SYS_X_TotalL,
        SYS_X_TotalH,
        SYS_X_XorSum,

        SYS_Y_Length,
        SYS_Y_Maxium,
        SYS_Y_InSafe,
        SYS_Y_OutSafe,
        SYS_Y_TotalL,
        SYS_Y_TotalH,
        SYS_Y_XorSum,

        SYS_Z_Length,
        SYS_Z_Maxium,
        SYS_Z_InSafe,
        SYS_Z_OutSafe,
        SYS_Z_TotalL,
        SYS_Z_TotalH,
        SYS_Z_XorSum,

        SYS_X_Origin,
        SYS_Y_Origin,
        SYS_Z_Origin,//40

        ACT_STOP,
        ACT_GX,			//1
        ACT_GY,			//2
        ACT_GZ,			//3
        ACT_GYZ,			//4
        ACT_GZY,			//5
        ACT_GZX,			//6
        ACT_GXZ,			//7

        ACT_MainUp,		//8
        ACT_MainDown,	//9
        ACT_MainForward,	//10
        ACT_MainBackward,//11
        ACT_PoseHori,	//12
        ACT_PoseVert,	//13
        ACT_ViceUp,		//14
        ACT_ViceDown,	//15

        ACT_ViceForward,	//16
        ACT_ViceBackward,//17
        ACT_GoOut,		//18
        ACT_ComeIn,		//19
        ACT_MainMidDown,		//20

        ACT_Reserve0,
        ACT_Reserve1,
        ACT_Reserve2,
        ACT_Reserve3,
        ACT_Reserve4,
        ACT_Reserve5,
        ACT_Reserve6,

        ACT_CheckInput,
        ACT_WaitMoldOpened,
        ACT_Cut,
        ACT_Parallel,
//        ACT_End,
//        ACT_End,

        ACT_CLIP_1On,
        ACT_CLIP_2On,
        ACT_CLIP_3On,
        ACT_CLIP_4On,
        ACT_CLIP_5On,
        ACT_CLIP_6On,
        ACT_CLIP_7On,
        ACT_CLIP_8On,
        ACT_CLSMD_On,
        ACT_EJECT_On,
        ACT_LAYOUT_On,
        ACT_CLIP_12On,
        ACT_CLIP_13On,
        ACT_CLIP_14On,
        ACT_CLIP_15On,
        ACT_CLIP_16On,
        ACT_CLIP_1Off,
        ACT_CLIP_2Off,
        ACT_CLIP_3Off,
        ACT_CLIP_4Off,
        ACT_CLIP_5Off,
        ACT_CLIP_6Off,
        ACT_CLIP_7Off,
        ACT_CLIP_8Off,
        ACT_CLSMD_Off,
        ACT_EJECT_Off,
        ACT_LAYOUT_Off,
        ACT_CLIP_12Off,
        ACT_CLIP_13Off,
        ACT_CLIP_14Off,
        ACT_CLIP_15Off,
        ACT_CLIP_16Off,
        SYS_CheckSum,
        SYS_Param_Count
    };

    enum ICStatus
    {
        Status,
        Step,
        Sub0,
        Sub1,
        Sub2,
        Sub3,
        ActL,
        ActH,
        ClipL,
        ClipH,
        Time,
        XPos,
        YPos,
        ZPos,
        S,
        Input0,
        Input1,
        Output0,
        Output1,
        EuIn,
        EuOut,
        ErrCode,
        DbgX0,
        DbgY0,
        DbgZ0,
        DbgX1,
        DbgY1,
        DbgZ1,

        StatusCount
    };

    enum ICRuntimeStatus
    {
        AutoReady       = 0,
        AutoRunning     = 1,
        AutoSingleCycle = 2,
        AutoStopping    = 3,
        Stop            = 0x20,
        Teach           = 0x21,
        Origin          = 0x22,
        Return          = 0x24,
        Auto            = 0x60,
        Manual          = 0x80
    };

    enum ICSystemParameterAddr
    {
        SM_Global_Speed,			//加减速时间
        SM_XLMT,			//X极限
        SM_XMAX,			//X最大值
        SM_YLMT,			//Y极限
        SM_YMAX,			//Y极限
        SM_ZLMT,			//Z极限
        SM_ZMAX,			//Z极限
        SM_ZINSAFE,			//型内安全区
        SM_ZOUTSAFE,		//型外安全区
        SM_Position_DOWN,			//主上限制
        SM_MAINDOWN,		//主下限制
        SM_MAININ,			//主入限制
        SM_MAINOUT,			//主出限制
        SM_VICEUP,			//副上限制
        SM_VICEDOWN,		//副下限制
        SM_VICEIN,			//副入限制
        SM_VICEOUT,			//副出限制
        SM_LAYSEQ,			//置放顺序
        SM_LAYXARRAY,		//置放点阵X
        SM_LAYYARRAY,		//置放点阵Y
        SM_LAYZARRAY,		//置放点阵Z
        SM_LAYXGAP,			//置放长度X
        SM_LAYYGAP,			//置放长度Y
        SM_LAYZGAP,			//置放长度Z
        SM_LAYXCNT,			//置放计数X
        SM_LAYYCNT,			//置放计数Y
        SM_LAYZCNT,			//置放计数Z
        SM_CHKCLIP1,		//夹检测1
        SM_CHKCLIP2,		//夹检测2
        SM_CHKCLIP3,		//夹检测3
        SM_CHKCLIP4,		//夹检测4
        SM_CHKCLIP5,		//夹检测5
        SM_CHKCLIP6,		//夹检测6
        SM_CHKCLIP7,		//夹检测7
        SM_CHKCLIP8,		//夹检测8
        SM_PRODUCT,			//设定产量
        SM_TIMECLIP1,		//夹限时1
        SM_TIMECLIP2,		//夹限时2
        SM_TIMECLIP3,
        SM_TIMECLIP4,
        SM_TIMECLIP5,
        SM_TIMECLIP6,
        SM_TIMECLIP7,
        SM_TIMECLIP8,
        SM_TIMEPUT1,		//放限时1
        SM_TIMEPUT2,
        SM_TIMEPUT3,
        SM_TIMEPUT4,
        SM_TIMEPUT5,
        SM_TIMEPUT6,
        SM_TIMEPUT7,
        SM_TIMEPUT8,
        SM_TIMEHORI,
        SM_TIMEVERT,
        SM_TIMEVUP,
        SM_TIMEVDOWN,
        SM_TIMEVFOR,
        SM_TIMEVBACK,
        SM_XINSAFE,
        SM_XOUTSAFE,
        SM_YINSAFE,
        SM_YOUTSAFE,
        SM_ALLOWANCE,			//容差
        SM_TIMEMUP,
        SM_TIMEMDOWN,
        SM_TIMEMFOR,
        SM_TIMEMBACK,
        SM_ACCTIMEX,			//X A&D
        SM_ACCTIMEY,			//Y A&D
        SM_ACCTIMEZ,			//Z A&D
        SM_LMTVX,				//X Max Speed
        SM_LMTVY,				//Y Max Speed
        SM_LMTVZ,				//Z Max Speed
        SM_ARM_CONFIG,
        SM_YEarlyEnd,
        SM_ZEarlyEnd,
        SM_WaitMoldOpenLimit,
        SM_ACCTIME
    };

    enum ICTeachParamAddr
    {
        ST_ARC_TRACK,
        ST_ARC_RADIO,
        ST_EDIT_X1,
        ST_EDIT_Y1,
        ST_EDIT_Z,
        ST_EDIT_SPEED,
        ST_DLY_POSE,
        ST_DLY_X2,
        ST_DLY_Y2,
        ST_DLY_CLIP1,
        ST_DLY_CLIP2,
        ST_DLY_CLIP3,
        ST_DLY_CLIP4,
        ST_DLY_CLIP5,
        ST_DLY_CLIP6,
        ST_DLY_CLIP7,
        ST_DLY_CLIP8,

        ST_DLY_CLSMDON,
        ST_DLY_EJECTON,
        ST_DLY_LAYOUTON,
        ST_COUNT4,
        ST_DLY_CLIP13,
        ST_DLY_CLIP14,
        ST_COUNT7,
        ST_COUNT8,
        //检测点
        ST_CHECKUSE,
        ST_CHECKSEL,
        ST_CHECKASON,
        ST_CHECKASOFF,
        //速度
        ST_SPD_CLIP1,
        ST_SPD_CLIP2,
        ST_SPD_CLIP3,
        ST_SPD_CLIP4,
        ST_SPD_CLIP5,
        ST_SPD_CLIP6,
        ST_SPD_CLIP7,
        ST_SPD_CLIP8,
        ST_SPD_CLSMDON,
        ST_SPD_EJECTON,
        ST_SPD_LAYOUTON,
        ST_SPD_COUNT4,
        ST_SPD_CLIP13,
        ST_SPD_CLIP14,
        ST_SPD_COUNT7,
        ST_SPD_COUNT8,

        ST_DLY_X1,
        ST_DLY_Y1,
        ST_DLY_Z,
        ST_SPEED2,

        ST_END
    };

    typedef QMap<ICSystemParameter, QVariant> ICSystemParameterMap ;
    typedef QMap<ICStatus, QVariant> ICStatusMap;

    explicit ICVirtualHost(QObject *parent = 0);
    ~ICVirtualHost();

    int SlaveID() const { return slaveID_;}
    void SetSlaveID(int id) { slaveID_ = id;}

    QVariant SystemParameter(ICSystemParameter which) const ;
    void SetSystemParameter(ICSystemParameter which, QVariant value);
    void ChangeSystemParameter(ICSystemParameter which, QVariant value);

    QVariant SystemParameter(ICSystemParameterAddr which) const ;
    void SetSystemParameter(ICSystemParameterAddr which, QVariant value);

    QVariant HostStatus(ICStatus status) const;

    static ICVirtualHost* GlobalVirtualHost() { return globalVirtualHost_;}
    static void SetGlobalVirtualHost(ICVirtualHost* virtualHost) {globalVirtualHost_ = virtualHost;}

    bool IsInputOn(int pos) const;
    bool IsOutputOn(int pos) const;
    bool IsClipOn(int pos) const;
    bool IsAction(int pos) const;

    bool IsPressureCheck() const { return (SystemParameter(SYS_Function).toInt() & 0x00000030) != 0;}
    void SetPressureCheck(bool isCheck);
    bool IsSecurityCheck() const { return (SystemParameter(SYS_Function).toInt() & 0x00000003) != 0;}
    void SetSecurityCheck(bool isCheck);
    bool IsMidMoldCheck() const {return (SystemParameter(SYS_Function).toInt() & 0x0000000C) != 0;}
    void SetMidMoldCheck(bool isCheck);
    bool IsEjectionLink() const { return (SystemParameter(SYS_Function).toInt() & 0x000000C0) != 0;}
    void SetEjectionLink(bool permit);
    bool IsAlarmWhenOrigin() const { return (SystemParameter(SYS_Function).toInt() & 0x00000300) != 0;}
    void SetAlarmWhenOrigin(bool isAlarm);
    bool IsPositionDetect() const { return (SystemParameter(SYS_Function).toInt() & 0x00000C00) != 0;}
    void SetPositionDetect(bool detect);
    bool IsOriginPositionHorizontal() const { return (SystemParameter(SYS_Function).toInt() & 0x00003000) != 0;}
    void SetOriginPosition(bool ishorizontal);

    int CurrentStep() const { return (statusMap_.value(Step).toInt() & 0x00FF);}
    int CurrentStatus() const { return (statusMap_.value(Status).toUInt() & 0x0FFF);}
    int AlarmNum() const { return (statusMap_.value(ErrCode).toUInt() & 0x0FFF);}
    int HintNum() const { return (statusMap_.value(ErrCode).toUInt() >> 12);}
    bool IsOrigined() const { return (statusMap_.value(Status).toUInt() >> 12) == 1;}

    int GlobalSpeed() const {return systemParamMap_.value(SYS_Global_Speed).toInt();}
    void SetGlobalSpeed(int speed);

    void SaveSystemConfig();
    void SaveAxisParam();
    void ReConfigure();

    bool IsSpeedEnable() const { return isSpeedEnable_;}
    void SetSpeedEnable(bool enble) { isSpeedEnable_ = enble;}

    bool HasTuneSpeed() const { return hasTuneSpeed_;}
    void SetTuneSpeed(bool tune){ hasTuneSpeed_ = tune;}

    int FinishProductCount() const { return productCount_;}
    void SetFinishProductCount(int product) { productCount_ = product;}

    bool IsSingleArm() const { return ((systemParamMap_.value(SYS_ARM_CONFIG).toInt() & 0x0300) >> 8) == 0;}

public Q_SLOTS:
    void SetMoldParam(int param, int value);
Q_SIGNALS:
    void StatusRefreshed();
    void NeedToGetTeach();
    void StepChanged(int);
    void SubStepChanged(uint8_t*);
    void MoldNumChanged(int);
private Q_SLOTS:
    void RefreshStatus();

private:
    void InitSubs_();
    void InitSystem_();
    void InitMold_();
    void InitMoldParam_();
    void InitAddrToSysPosMap_();
    void InitSystemMap_();
    void InitICStatusMap_();
    void WriteMoldTohost_();
    void WriteSubTohost_();
    void WriteSystemTohost_();
    void AppendDWord_(QVector<uint8_t> &datas, uint value) { datas.append(value & 0x00FF); datas.append(value >> 8);}
    ICSystemParameterMap systemParamMap_;
    QScopedPointer<ICMold> currentMold_;
    ICMacroSubroutine* subroutines_;
    ICStatusMap statusMap_;
    QMap<ICSystemParameterAddr, ICSystemParameter> addrToSysPos_;
//    QMap<int, ICSystemParameter> moldParamToAddrPos_;
    QMap<int, ICSystemParameterAddr> moldParamToAddrPos_;
    ICQueryStatus queryStatusCommand_;
    int slaveID_;

    uint input0Bits_;
    uint input1Bits_;
    uint output0Bits_;
    uint output1Bits_;
    uint euInputBits_;
    uint euOutputBits_;
    uint clipLBits_;
    uint clipHBits_;
    uint actionLBits_;
    uint actionHBits_;
    bool flag_;
    int oldStep_;
    int tryTimes_;
    int oldKey_;
    int currentAddr_;
    int currentStatus_;
    int freshCount_;

    bool isInitSuccess_;
    QTimer* timer_;
    uint8_t oldSubStep[8];
    int oldMoldNum_;
    int watchdogFd_;
    bool isSpeedEnable_;
    bool hasTuneSpeed_;
    int productCount_;
    static ICVirtualHost* globalVirtualHost_;
};
#define icGlobalVirtuallHost ICVirtualHost::GlobalVirtualHost()

inline void ICVirtualHost::ReConfigure()
{
//    qApp->processEvents();
    qDebug("Reconfig");
    WriteSubTohost_();
    WriteMoldTohost_();
    currentMold_->UpdateSyncSum();
    InitMoldParam_();
    InitSystem_();
//    WriteSystemTohost_();
}

inline void ICVirtualHost::SetGlobalSpeed(int speed)
{
    if(ICCommandProcessor::Instance()->ModifySysParam(SYS_Global_Speed, speed))
    {
        systemParamMap_.insert(SYS_Global_Speed, speed);
        SaveSystemConfig();
    }
}

inline QVariant ICVirtualHost::SystemParameter(ICSystemParameter which) const
{
    Q_ASSERT_X(systemParamMap_.contains(which), "GetSystemParameter", (QString::number(which) + " is not a correct parameter").toAscii());
    return systemParamMap_.value(which);
}

inline void ICVirtualHost::SetSystemParameter(ICSystemParameter which, QVariant value)
{
    Q_ASSERT_X(systemParamMap_.contains(which), "GetSystemParameter", (QString::number(which) + " is not a correct parameter").toAscii());
//    if(ICCommandProcessor::Instance()->ModifySysParam(which, value.toInt()))
//    {
        systemParamMap_.insert(which, value);
//    }
}

inline void ICVirtualHost::ChangeSystemParameter(ICSystemParameter which, QVariant value)
{
    Q_ASSERT_X(systemParamMap_.contains(which), "GetSystemParameter", (QString::number(which) + " is not a correct parameter").toAscii());
    systemParamMap_.insert(which, value);
}

inline QVariant ICVirtualHost::SystemParameter(ICSystemParameterAddr which) const
{
    Q_ASSERT_X(addrToSysPos_.contains(which), "ICVirtualHost::SystemParamter", (QString::number(which) + " is not a currect address").toAscii());
    return SystemParameter(addrToSysPos_.value(which));
}

inline void ICVirtualHost::SetSystemParameter(ICSystemParameterAddr which, QVariant value)
{
    Q_ASSERT_X(addrToSysPos_.contains(which), "ICVirtualHost::SystemParamter", (QString::number(which) + " is not a currect address").toAscii());
//    systemParamMap_.insert(addrToSysPos_.value(which), value);
    SetSystemParameter(addrToSysPos_.value(which), value);
}

inline QVariant ICVirtualHost::HostStatus(ICStatus status) const
{
    Q_ASSERT_X(statusMap_.contains(status), "GetHostStatus", (QString::number(status) + " is not a correct status").toAscii());
    return statusMap_.value(status);
}

inline bool ICVirtualHost::IsInputOn(int pos) const
{
    if(pos < 16)
    {
        uint temp = 1 << pos;
        return input0Bits_ & temp;
    }
    else if(pos < 32)
    {
        uint temp = 1 << (pos - 16);
        return input1Bits_ & temp;
    }
    else if(pos >= 32)
    {
        uint temp = 1 << (pos - 32);
        return euInputBits_ & temp;
    }
    return false;
}

inline bool ICVirtualHost::IsOutputOn(int pos) const
{
    if(pos < 16)
    {
        uint temp = 1 << pos;
        return output0Bits_ & temp;
    }
    else if(pos < 32)
    {
        uint temp = 1 << (pos - 16);
        return output1Bits_ & temp;
    }
    else if(pos >= 32)
    {
        uint temp = 1 << (pos - 32);
        return euOutputBits_ & temp;
    }
    return false;
}

inline bool ICVirtualHost::IsClipOn(int pos) const
{
    if(pos < 16)
    {
        uint temp = 1 << pos;
        return clipLBits_ & temp;
    }
    else if(pos < 32)
    {
        uint temp = 1 << (pos - 16);
        return clipHBits_ & temp;
    }
    return false;
}

inline bool ICVirtualHost::IsAction(int pos) const
{
    if(pos < 16)
    {
        uint temp = 1 << pos;
        return actionLBits_ & temp;
    }
    else if(pos < 32)
    {
        uint temp = 1 << (pos - 16);
        return actionHBits_ & temp;
    }
    return false;
}

inline void ICVirtualHost::SetPressureCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFDF;
    (isCheck ? val |= 0x00000010 : val &= 0xFFFFFFEF);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetSecurityCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFFD;
    (isCheck ? val |=0x00000001 : val&= 0xFFFFFFFE);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetMidMoldCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFF7;
    (isCheck ? val |= 0x00000004 : val &= 0xFFFFFFFB);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetEjectionLink(bool permit)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFF7F;
    (permit ? val |= 0x00000040 : val &= 0xFFFFFFBF);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetAlarmWhenOrigin(bool isAlarm)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFDFF;
    (isAlarm ? val |= 0x00000100 : val &= 0xFFFFFEFF);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetPositionDetect(bool detect)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFF7FF;
    (detect ? val |= 0x00000400 : val &= 0xFFFFFBFF);
    systemParamMap_.insert(SYS_Function, val);
}

inline void ICVirtualHost::SetOriginPosition(bool isHorizontal)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFDFFF;
    (isHorizontal ? val |= 0x00001000 : val &= 0xFFFFEFFF);
    systemParamMap_.insert(SYS_Function, val);
}

#endif // ICVIRTUALHOST_H
