#ifndef ICVIRTUALHOST_H
#define ICVIRTUALHOST_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QVector>
#include <stdint.h>
#include <QScopedPointer>
#include <QApplication>
#include <QTimer>
#include "config.h"
#include "iccommands.h"
#include "iccommandprocessor.h"
#include "icmold.h"

typedef union {
     struct {
        uint16_t canType : 8;
        uint16_t  canAddr:8;
    }b;
    uint16_t all;
}CanConfig;

typedef union{
     struct{
        uint16_t r1 : 1;
        uint16_t r2 : 1;
        uint16_t r3 : 1;
        uint16_t r4 : 1;
        uint16_t r5 : 1;
        uint16_t r6 : 1;
        uint16_t r7 : 1;
        uint16_t r8 : 1;
        uint16_t r : 8;
    }b;
    uint16_t all;
}ReserveProgConfig;

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
#ifdef HC_8AXIS
        SYS_AccTimeP,
        SYS_AccTimeQ,
        SYS_AccTimeA,
        SYS_AccTimeB,
        SYS_AccTimeC,
#endif
        SYS_LMT_vX,
        SYS_LMT_vY,
        SYS_LMT_vZ,
#ifdef HC_8AXIS
        SYS_LMT_vP,
        SYS_LMT_vQ,
        SYS_LMT_vA,
        SYS_LMT_vB,
        SYS_LMT_vC,
#endif
        SYS_ARM_CONFIG,
        SYS_YEarlyEnd,
        SYS_PullPushDistance,

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
#ifdef HC_8AXIS
        SYS_P_Length,
        SYS_P_Maxium,
        SYS_P_InSafe,
        SYS_P_OutSafe,
        SYS_P_TotalL,
        SYS_P_TotalH,
        SYS_P_XorSum,
        SYS_Q_Length,
        SYS_Q_Maxium,
        SYS_Q_InSafe,
        SYS_Q_OutSafe,
        SYS_Q_TotalL,
        SYS_Q_TotalH,
        SYS_Q_XorSum,
        SYS_A_Length,
        SYS_A_Maxium,
        SYS_A_InSafe,
        SYS_A_OutSafe,
        SYS_A_TotalL,
        SYS_A_TotalH,
        SYS_A_XorSum,
        SYS_B_Length,
        SYS_B_Maxium,
        SYS_B_InSafe,
        SYS_B_OutSafe,
        SYS_B_TotalL,
        SYS_B_TotalH,
        SYS_B_XorSum,
        SYS_C_Length,
        SYS_C_Maxium,
        SYS_C_InSafe,
        SYS_C_OutSafe,
        SYS_C_TotalL,
        SYS_C_TotalH,
        SYS_C_XorSum,
        SYS_Config_Signal,
        SYS_Config_Arm,
        SYS_Config_Out,
        SYS_Config_Fixture,
        SYS_Config_Resv1,
        SYS_Config_Resv2,
        SYS_Config_Xorsum,
#endif

        SYS_X_Origin,
        SYS_Y_Origin,
        SYS_Z_Origin,//40
#ifdef HC_8AXIS
        SYS_P_Origin,
        SYS_Q_Origin,
        SYS_A_Origin,
        SYS_B_Origin,
        SYS_C_Origin,
#endif

        //TimeLimit[32]
        ACT_MainUp,		//8
        ACT_MainDown,	//9
        ACT_MainForward,	//10
        ACT_MainBackward,//11
        ACT_PoseHori,	//12
        ACT_PoseVert,	//13
        ACT_ViceUp,		//14
        ACT_ViceDown,	//15
        ACT_GC,
        ACT_GX,			//1
        ACT_GY,			//2
        ACT_GZ,			//3
        ACT_GP,			//4
        ACT_GQ,			//5
        ACT_GA,			//6
        ACT_GB,			//7

        ACT_ViceForward,	//16
        ACT_ViceBackward,//17
        ACT_GoOut,		//18
        ACT_ComeIn,		//19
        ACT_PoseHori2,		//20

        ACT_PoseVert2,
        ACT_GASUB,
        ACT_GAADD,
        ACT_GBSUB,
        ACT_GBADD,
        ACT_GCSUB,
        ACT_GCADD,

        ACT_CheckInput,
        ACT_WaitMoldOpened,
        ACT_Cut,
        ACT_Parallel,
//        ACT_End,
//        ACT_End,
#ifdef HC_8AXIS
        //reserved[32]
        SYS_OriginSpeed,
        Sys_EnforceInput0,
        Sys_EnforceInput1,
        Sys_EnforceOutput0,
        Sys_EnforceOutput1,
        ACT_Y_Sec2,
        ACT_Y_Sec3,
        ACT_Y_Sec4,
        ACT_Z_Sec1,
        ACT_Z_Sec2,
        ACT_Z_Sec3,
        ACT_Z_Sec4,
        ACT_P_Sec1,
        ACT_P_Sec2,
        ACT_P_Sec3,
        ACT_P_Sec4,
        ACT_Q_Sec1,
        ACT_Q_Sec2,
        ACT_Q_Sec3,
        ACT_Q_Sec4,
        ACT_A_Sec1,
        ACT_A_Sec2,
        ACT_A_Sec3,
        ACT_A_Sec4,
        ACT_B_Sec1,
        ACT_B_Sec2,
        ACT_B_Sec3,
        ACT_B_Sec4,
        ACT_C_Sec1,
        ACT_C_Sec2,
        ACT_C_Sec3,
        ACT_C_Sec4,
#else
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
        ACT_CORE1_ON,
        ACT_CORE2_ON,
        ACT_AUX1,
        ACT_AUR,
        ACT_AUX3,
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
        ACT_CORE1_OFF,
        ACT_CORE2_OFF,
        ACT_AUX4,
        ACT_AUX5,
        ACT_AUX6,
#endif
        SYS_CheckSum,
        SYS_Param_Count
    };

    enum ICStatus
    {
        Status,
        Step,
        Input2,
        Input3,
        Output2,
        Output3,
        ActL,
        ActH,
        ClipL,
        ClipH,
        Time,
        XPos,
        YPos,
        ZPos,
#ifdef HC_8AXIS
        PPos,
        QPos,
//        APos,
//        BPos,
//        CPos,
#endif
        S,
        Input0,
        Input1,
        Output0,
        Output1,
        EuIn,
        EuOut,
        ErrCode,
#ifdef HC_8AXIS
        DbgX0,
        DbgX1,
        DbgY0,
        DbgY1,
        DbgZ0,
        DbgZ1,
        DbgP0,
        DbgP1,
        DbgQ0,
        DbgQ1,
        DbgA0,
        DbgA1,
        DbgB0,
        DbgB1,
        DbgC0,
        DbgC1,

#else
        DbgX0,
        DbgY0,
        DbgZ0,
        DbgX1,
        DbgY1,
        DbgZ1,
#endif
//        AxisLastPos1,
//        AxisLastPos2,

        StatusCount
    };

    enum ICRuntimeStatus
    {
        AutoReady       = 0,
        AutoRunning     = 1,
        AutoSingleCycle = 2,
        AutoStopping    = 3,
        AutoOneCycle    = 4,
        Stop            = 0x20,
        Teach           = 0x21,
        Origin          = 0x22,
        Return          = 0x24,
        Auto            = 0x60,
        Manual          = 0x80
    };

    enum ICAxisDefine
    {
        ICAxisDefine_None,
        ICAxisDefine_Pneumatic,
        ICAxisDefine_Reve,
        ICAxisDefine_Servo
    };

    enum ICAxis
    {
        ICAxis_AxisX1,
        ICAxis_AxisY1,
        ICAxis_AxisZ,
        ICAxis_AxisX2,
        ICAxis_AxisY2,
        ICAxis_AxisA,
        ICAxis_AxisB,
        ICAxis_AxisC
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
        SM_TIMEHORI2,
        SM_TIMEVERT2,
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
        SM_ACCTIMEP,
        SM_ACCTIMEQ,			//X A&D
        SM_ACCTIMEA,			//Y A&D
        SM_ACCTIMEB,			//Z A&D
        SM_ACCTIMEC,
        SM_LMTVX,				//X Max Speed
        SM_LMTVY,				//Y Max Speed
        SM_LMTVZ,				//Z Max Speed
        SM_LMTVP,
        SM_LMTVQ,				//X Max Speed
        SM_LMTVA,				//Y Max Speed
        SM_LMTVB,				//Z Max Speed
        SM_LMTVC,
        SM_ARM_CONFIG,
        SM_YEarlyEnd,
        SM_ZEarlyEnd,
        SM_WaitMoldOpenLimit,
        SM_ACCTIME,
        SM_PullPushDistance,
#ifdef HC_8AXIS
        SM_X_SEC1,
        SM_X_SEC2,
        SM_X_SEC3,
        SM_X_SEC4,
        SM_Y_SEC1,
        SM_Y_SEC2,
        SM_Y_SEC3,
        SM_Y_SEC4,
        SM_Z_SEC1,
        SM_Z_SEC2,
        SM_Z_SEC3,
        SM_Z_SEC4,
        SM_P_SEC1,
        SM_P_SEC2,
        SM_P_SEC3,
        SM_P_SEC4,
        SM_Q_SEC1,
        SM_Q_SEC2,
        SM_Q_SEC3,
        SM_Q_SEC4,
        SM_A_SEC1,
        SM_A_SEC2,
        SM_A_SEC3,
        SM_A_SEC4,
        SM_B_SEC1,
        SM_B_SEC2,
        SM_B_SEC3,
        SM_B_SEC4,
        SM_C_SEC1,
        SM_C_SEC2,
        SM_C_SEC3,
        SM_C_SEC4,
#endif
        SM_StandBy,  //待机姿势
        SM_TryProduct, //试产
        SM_Sampling //取样
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

    enum FunctionBits{
        FLEEBIT    = 0x00000100,
        ORIGINBIT  = 0x00000200,
        AUTOBIT   = 0x00000400,
        WAITBIT   = 0x00000800,
        PUNCHBIT = 0x000001000,
        SAFEINFO  = 0x000002000

    };

    typedef QMap<ICSystemParameter, QVariant> ICSystemParameterMap ;
    typedef QMap<ICStatus, QVariant> ICStatusMap;

    explicit ICVirtualHost(QObject *parent = 0);
    ~ICVirtualHost();

    int SlaveID() const { return slaveID_;}
    void SetSlaveID(int id) { slaveID_ = id;}

    void StopRefreshStatus();
    void RestartRefreshStatus();

    QVariant SystemParameter(ICSystemParameter which) const ;
    void SetSystemParameter(ICSystemParameter which, QVariant value);
    void ChangeSystemParameter(ICSystemParameter which, QVariant value);

    QVariant SystemParameter(ICSystemParameterAddr which) const ;
    void SetSystemParameter(ICSystemParameterAddr which, QVariant value);

    QVariant HostStatus(ICStatus status) const;
    void SetHostStatus(ICStatus status, QVariant v) { statusMap_.insert(status,v);}

    static ICVirtualHost* GlobalVirtualHost() { return globalVirtualHost_;}
    static void SetGlobalVirtualHost(ICVirtualHost* virtualHost) {globalVirtualHost_ = virtualHost;}

    bool IsInputOn(int pos) const;
    bool IsOutputOn(int pos) const;
    bool IsClipOn(int pos) const;
    bool IsAction(int pos) const;

    int PressureCheckMode() const { return (SystemParameter(SYS_Function).toInt() & 0x00000003);}
    void SetPressureCheckMode(int mode);
    bool IsSecurityCheck() const { return (SystemParameter(SYS_Function).toInt() & 0x00000003) != 0;}
    void SetSecurityCheck(bool isCheck);
    bool IsMidMoldCheck() const {return (SystemParameter(SYS_Function).toInt() & 0x00000004) != 0;}
    void SetMidMoldCheck(bool isCheck);
    bool IsOrignSyncCheck() const {return (SystemParameter(SYS_Function).toInt() & 0x00000008) != 0;}
    void SetOrignSyncCheck(bool isCheck);
    int PunchCheckMode() const {return ((SystemParameter(SYS_Function).toInt() & 0x000000030)) >> 4;}
    void SetPunchCheckMode(int mode);
    bool IsEjectionLink() const { return (SystemParameter(SYS_Function).toInt() & 0x00000040) != 0;}
    void SetEjectionLink(bool permit);
    bool IsAlarmWhenOrigin() const { return (SystemParameter(SYS_Function).toInt() & 0x00000300) != 0;}
    void SetAlarmWhenOrigin(bool isAlarm);
    bool IsPositionDetect() const { return (SystemParameter(SYS_Function).toInt() & 0x00000C00) != 0;}
    void SetPositionDetect(bool detect);
    int OriginPosition() const { return (SystemParameter(SYS_Function).toInt() & 0x00003000) >> 12;}
    void SetOriginPosition(int position);
    int TranserferPosition() const { return (SystemParameter(SYS_Function).toInt() & 0x00000C00) >> 10;}
    void SetTranserferPosition(int position);
    int EscapeWay() const { return (SystemParameter(SYS_Function).toInt() & 0x0000C000) >> 14;}
    void SetEscapeWay(int way);
    bool IsAutoSignalUse() const { return (SystemParameter(SYS_Function).toInt() & 0x08) == 0; }
    void SetAutoSignalUse(bool isUse);
    int FailAlarmWay() const { return (SystemParameter(SYS_Function).toInt() & 0x20) >> 5;}
    void SetGetFailAlarmWay(int way);
    bool IsCloseMoldEn() const { return (SystemParameter((SYS_Function)).toInt() & 0x80 ) != 0;}
    void SetCloseMoldEn(bool isEn);
    bool IsFleeEn() const { return (SystemParameter((SYS_Function)).toInt() & FLEEBIT ) != 0;}   //逃跑
    void SetFleeEn(bool isEn);
    bool IsOriginModeEn() const { return (SystemParameter((SYS_Function)).toInt() & ORIGINBIT ) != 0;} //原点信号
    void SetOriginModeEn(bool isEn);
    bool IsAutoModeEn() const { return (SystemParameter((SYS_Function)).toInt() & AUTOBIT ) != 0;} //全自动信号
    void SetAutoModeEn(bool isEn);
    bool IsWaitModeEn() const { return (SystemParameter((SYS_Function)).toInt() & WAITBIT ) != 0;} //等待信号
    void SetWaitModeEn(bool isEn);
    bool IsForcePunchModeEn() const { return (SystemParameter((SYS_Function)).toInt() & PUNCHBIT ) != 0;}  //强制冲压
    void SetForcePunchModeEn(bool isEn);
    bool IsSafeInfoModeEn() const { return (SystemParameter((SYS_Function)).toInt() & SAFEINFO ) != 0;} //安全提示
    void SetSafeInfoModeEn(bool isEn);

    int CurrentStep() const { return (statusMap_.value(Step).toInt() & 0x00FF);}
    int CurrentStatus() const { return (statusMap_.value(Status).toUInt() & 0x0FFF);}
    int AlarmNum() const { return (statusMap_.value(ErrCode).toUInt() & 0x0FFF);}
    int HintNum() const { return (statusMap_.value(ErrCode).toUInt() >> 12);}
    bool IsOrigined() const { return (( (statusMap_.value(Status).toUInt() >> 12) & 3) == 1);}

    int64_t GlobalSpeed() const {return systemParamMap_.value(SYS_Global_Speed).toInt() |
             (systemParamMap_.value(SYS_RsvReadMold).toInt() << 16) |
                ((int64_t)(systemParamMap_.value(SYS_Language).toInt()) << 32);}
    void SetGlobalSpeed(quint64 speed);

    void SaveSystemConfig();
    void SaveAxisParam(int axis);
    void ReConfigure();

    bool IsSpeedEnable() const { return isSpeedEnable_;}
    void SetSpeedEnable(bool enble) { isSpeedEnable_ = enble;}

    bool HasTuneSpeed() const { return hasTuneSpeed_;}
    void SetTuneSpeed(bool tune){ hasTuneSpeed_ = tune;}

//    int FinishProductCount() const { return productCount_;}
//    void SetFinishProductCount(int product) { productCount_ = product;}
    int FinishProductCount() const;
    void SetFinishProductCount(int product) { systemParamMap_.insert(SYS_RsvWorkmold, product);}
    bool HasGetFinishProductCount() const { return (currentAddr_ >= 7) && (CurrentStatus() == Auto && statusMap_.value(DbgX0).toInt() == 1);}

//    bool IsSingleArm() const { return (systemParamMap_.value(SYS_ARM_CONFIG).toInt() & 0x0100) > 0;}
//    void SetSingleArm(bool isSingle);
    bool HasMainArmForwardLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x0002) > 0; }
    void SetMainArmForwardLimit(bool hasForward);
    bool HasMainArmBackwardLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x0001) > 0;}
    void SetMainArmBackwardLimit(bool hasBackward);
    bool HasMainArmDownLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x000C) >> 3 == 1;}
    void SetMainArmDownLimit(bool hasDown);
    bool HasSubArmForwardLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x0080) > 0;}
    void SetSubArmForwardLimit(bool hasForward);
    bool HasSubArmBackwardLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x0040) > 0;}
    void SetSubArmBackwardLimit(bool hasBackward);
    bool HasSubArmDownLimit() const { return (systemParamMap_.value(SYS_Config_Signal).toInt() & 0x0300) >> 9 == 1;}
    void SetSubArmDownLimit(bool hasDown);
    int PeripheryOutput(int number) const;
    void CalPeripheryOutput(int & config, int number, int val);
    void SetPeripheryOutput(int config) { systemParamMap_.insert(SYS_Config_Out, config);}
    int FixtureDefine() const { return (systemParamMap_.value(SYS_Config_Fixture).toInt()) == 0x0555 ? 1 : 0;}
    void SetFixtureDefine(int val) { systemParamMap_.insert(SYS_Config_Fixture, val == 0 ? 0x0AAA : 0x0555);}
    int FixtureDefineSwitch(int index) const {return index == 0 ? 0x0AAA : 0x0555;}

    ICAxisDefine AxisDefine(ICAxis which) const;
    void CalAxisDefine(int &config, ICAxis which, ICAxisDefine define) const;
    void SetAxisDefine(int config) { systemParamMap_.insert(SYS_Config_Arm, config);}

    bool IsParamChanged() const { return isParamChanged_;}

    bool IsFixtureCheck() const { return isFixtureCheck_;}
    void SetFixtureCheck(bool isCheck) { isFixtureCheck_ = isCheck;}

    int GetActualPos(ICAxis axis) const;
    int GetActualPos(ICAxis axis, uint axisLastPos) const;

    bool IsSingleRun() const { return isSingleRun_;}
    void SetSingleRun(bool isrun) {isSingleRun_ = isrun;}

    bool DoseControled() const { return ((statusMap_.value(Status).toUInt() >> 14) & 1) == 1; }

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
    void GetAxisParam_(const QString& file, int start, int end, QVector<uint>& tmp);
    void SaveAxisParamHelper_(const QString& file, int start, int end);
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
    uint input2Bits_;
    uint input3Bits_;
    uint output0Bits_;
    uint output1Bits_;
    uint output2Bits_;
    uint output3Bits_;
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
    QTimer timer_;
    uint8_t oldSubStep[8];
    int oldMoldNum_;
    int watchdogFd_;
    bool isSpeedEnable_;
    bool hasTuneSpeed_;
    int productCount_;
    bool isParamChanged_;
    bool isFixtureCheck_;
    static ICVirtualHost* globalVirtualHost_;
    bool flag;
    bool isSingleRun_;
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
    isParamChanged_ = false;
//    WriteSystemTohost_();
}

inline void ICVirtualHost::SetGlobalSpeed(quint64 speed)
{
    if(ICCommandProcessor::Instance()->ModifySysParam(SYS_Global_Speed, speed & 0x0000FFFF))
    {
        systemParamMap_.insert(SYS_Global_Speed, speed & 0x0000FFFF);
        if(ICCommandProcessor::Instance()->ModifyMoldParam(SYS_RsvReadMold, (speed >> 16) & 0xFFFF ))
        {
            systemParamMap_.insert(SYS_RsvReadMold,(speed >> 16) & 0x0000FFFF );
            if(ICCommandProcessor::Instance()->ModifyMoldParam(SYS_Language, (speed >> 32) & 0xFFFF))
            {
                systemParamMap_.insert(SYS_Language,(speed >> 32) & 0xFFFF);
            }
        }
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
        isParamChanged_ = true;
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
    else if(pos < 48)
    {
        uint temp = 1 << (pos - 32);
        return input2Bits_ & temp;
    }
    else if(pos < 64)
    {
        uint temp = 1 << (pos - 48);
        return input3Bits_ & temp;
    }
    else if(pos < 80)
    {
        uint temp = 1 << (pos - 64);
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
    else if(pos < 48)
    {
        uint temp = 1 << (pos - 32);
        return output2Bits_ & temp;
    }
    else if(pos < 64)
    {
        uint temp = 1 << (pos - 48);
        return output3Bits_ & temp;
    }
    else if(pos < 80)
    {
        uint temp = 1 << (pos - 64);
        return euOutputBits_ & temp;
    }
    else if(pos < 96)
    {
        uint temp = 1 << (pos - 80);
        return clipLBits_ & temp;
    }
    else if(pos < 128)
    {
        uint temp = 1 << (pos - 90);
        return clipHBits_ & temp;
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

inline void ICVirtualHost::SetPressureCheckMode(int mode)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFFC;
    val |= (mode & 3);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetSecurityCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFFD;
    (isCheck ? val |=0x00000001 : val&= 0xFFFFFFFE);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetMidMoldCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFFB;
    (isCheck ? val |= 0x00000004 : val &= 0xFFFFFFFB);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetOrignSyncCheck(bool isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFF7;
    (isCheck ? val |= 0x00000008 : val &= 0xFFFFFFF7);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetPunchCheckMode(int isCheck)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFCF;
    val |= isCheck<<4;
//    (isCheck ? val |= 0x000000030 : val &= 0xFFFFFFCF);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetEjectionLink(bool permit)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFBF;
    (permit ? val |= 0x00000040 : val &= 0xFFFFFFBF);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetAlarmWhenOrigin(bool isAlarm)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFDFF;
    (isAlarm ? val |= 0x00000100 : val &= 0xFFFFFEFF);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetPositionDetect(bool detect)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFF7FF;
    (detect ? val |= 0x00000400 : val &= 0xFFFFFBFF);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetOriginPosition(int position)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFCFFF;
    val |= (position << 12);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetTranserferPosition(int position)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFF3FF;
    val |= (position << 10);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetEscapeWay(int way)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFF3FFF;
    val |= (way << 14);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetAutoSignalUse(bool isUse)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFF7;
    (!isUse ? val |= 0x00000008 : val &= 0xFFFFFFF7);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetGetFailAlarmWay(int way)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFFDF;
    val |= way << 5;
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetCloseMoldEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= 0xFFFFFF7F;
    (isEn ? val |= 0x00000080 : val &= 0xFFFFFF7F);
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetFleeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~FLEEBIT);
    (isEn ? val |= FLEEBIT : val &= (~FLEEBIT));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetOriginModeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~ORIGINBIT);
    (isEn ? val |= ORIGINBIT : val &= (~ORIGINBIT));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetAutoModeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~AUTOBIT);
    (isEn ? val |= AUTOBIT : val &= (~AUTOBIT));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}


inline void ICVirtualHost::SetWaitModeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~WAITBIT);
    (isEn ? val |= WAITBIT : val &= (~WAITBIT));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}

inline void ICVirtualHost::SetForcePunchModeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~PUNCHBIT);
    (isEn ? val |= PUNCHBIT : val &= (~PUNCHBIT));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}
inline void ICVirtualHost::SetSafeInfoModeEn(bool isEn)
{
    int val = SystemParameter(SYS_Function).toInt();
    val &= (~SAFEINFO);
    (isEn ? val |= SAFEINFO : val &= (~SAFEINFO));
    systemParamMap_.insert(SYS_Function, val);
    isParamChanged_ = true;
}



//inline void ICVirtualHost::SetSingleArm(bool isSingle)
//{
//    int val = SystemParameter(SYS_ARM_CONFIG).toInt();
//    isSingle ? val &= 0xFEFF : val |= 0x100;
//    systemParamMap_.insert(SYS_ARM_CONFIG, val);
//}

inline void ICVirtualHost::SetMainArmForwardLimit(bool hasForward)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasForward ? val |= 0x0002 : val &= 0xFFFD;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline void ICVirtualHost::SetMainArmBackwardLimit(bool hasBackward)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasBackward ? val |= 0x0001 : val &= 0xFFFE;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline void ICVirtualHost::SetMainArmDownLimit(bool hasDown)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasDown ? val |= 0x000C : val &= 0xFFF7;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline void ICVirtualHost::SetSubArmForwardLimit(bool hasForward)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasForward ? val |= 0x0080 : val &= 0xFF7F;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline void ICVirtualHost::SetSubArmBackwardLimit(bool hasBackward)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasBackward ? val |= 0x0040 : val &= 0xFFBF;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline void ICVirtualHost::SetSubArmDownLimit(bool hasDown)
{
    int val = SystemParameter(SYS_Config_Signal).toInt();
    hasDown ? val |= 0x0300 : val &= 0xFDFF;
    systemParamMap_.insert(SYS_Config_Signal, val);
}

inline ICVirtualHost::ICAxisDefine ICVirtualHost::AxisDefine(ICAxis which) const
{
    return static_cast<ICAxisDefine>((systemParamMap_.value(SYS_Config_Arm).toInt() >> (which << 1)) & 0x0003);
} 
inline void ICVirtualHost::CalAxisDefine(int &config, ICAxis which, ICAxisDefine define) const
{
    int mask = ~(0x003 << (which << 1));
    config &= mask;
    mask = define << (which << 1);
    config |= mask;
}

inline int ICVirtualHost::PeripheryOutput(int number) const
{
    return (systemParamMap_.value(SYS_Config_Out).toInt() >> (number << 1)) & 0x0003;
}

inline void ICVirtualHost::CalPeripheryOutput(int &config, int number, int val)
{
    config &= ~(3 << (number << 1));
    config |= (val << (number << 1));
//    systemParamMap_.insert(SYS_Config_Out, current);
}

inline int ICVirtualHost::GetActualPos(ICAxis axis) const
{
//    uint axisLastPos = HostStatus(AxisLastPos1).toUInt() | (HostStatus(AxisLastPos2).toUInt() << 16);
//    return GetActualPos(axis, axisLastPos);
    return GetActualPos(axis, 0);

}

inline int ICVirtualHost::GetActualPos(ICAxis axis, uint axisLastPos) const
{
    return (int16_t)HostStatus(static_cast<ICStatus>(XPos + axis)).toInt();
//    int ret = tmp;
//    ret *= 10;
//    if(tmp >= 0)
//    {
//        return ret += ((axisLastPos >> (axis * 4)) & 0xF);
//    }
//    else
//    {
//        return ret -=((axisLastPos >> (axis * 4)) & 0xF);
//    }
}

#endif // ICVIRTUALHOST_H
