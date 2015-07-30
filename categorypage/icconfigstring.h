#ifndef ICCONFIGSTRING_H
#define ICCONFIGSTRING_H

#include <QString>
#include <QMap>
#include <QObject>

#include "icbuttongroup.h"
#include "iccombobox.h"
#include "iclineeditwithvirtualnumerickeypad.h"
#include <QCheckBox>


#define ICLogInit \
    QMap<QObject*, int>::iterator p = editorToConfigIDs_.begin();\
    ICLineEditWithVirtualNumericKeypad* le;\
    ICComboBox* cb;\
    QButtonGroup* bg;\
    QCheckBox* cbox;\
    while(p != editorToConfigIDs_.end())\
    {\
        le = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(p.key());\
        if(le != NULL)\
        {\
            connect(le, SIGNAL(textChanged(QString)), SLOT(OnConfigChanged(QString)));\
            ++p;\
            continue;\
        }\
        cb = qobject_cast<ICComboBox*>(p.key());\
        if(cb != NULL)\
        {\
            connect(cb, SIGNAL(currentIndexChanged(int)), SLOT(OnConfigChanged(int)));\
            ++p;\
            continue;\
        }\
        bg = qobject_cast<QButtonGroup*>(p.key());\
        if(bg != NULL)\
        {\
            ICButtonGroup* icbg = new ICButtonGroup(bg, bg->parent());\
            connect(icbg, SIGNAL(buttonClicked(int, int)), SLOT(OnConfigChanged(int, int)));\
            ++p;\
            continue;\
        }\
        cbox = qobject_cast<QCheckBox*>(p.key());\
        if(cbox != NULL)\
        {\
            connect(cbox, SIGNAL(toggled(bool)), SLOT(OnConfigChanged(bool)));\
            ++p;\
            continue;\
        }\
    }


#define ICLogFunctions(cl) \
    void cl::OnConfigChanged(QObject *w, const QString& newV, const QString& oldV)\
    {\
        ICModifyFrame::Instance()->OnActionTriggered(editorToConfigIDs_.value(w, ICConfigString::kCS_Err),\
                                                    newV,\
                                                    oldV);\
    }\
    \
    void cl::OnConfigChanged(const QString &text)\
    {\
        ICLineEditWithVirtualNumericKeypad* edit = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(sender());\
        OnConfigChanged(edit, text, edit->LastValue());\
    }\
    \
    void cl::OnConfigChanged(int v, int ov)\
    {\
        ICButtonGroup* icbg = qobject_cast<ICButtonGroup*>(sender());\
        QButtonGroup* bg = icbg->ButtongGroup();\
        OnConfigChanged(bg, bg->button(v)->text(), bg->button(ov)->text());\
    }\
    \
    void cl::OnConfigChanged(int v)\
    {\
        ICComboBox* edit = qobject_cast<ICComboBox*>(sender());\
        OnConfigChanged(edit, edit->currentText(), edit->itemText(edit->LastValue()));\
    }\
\
    void cl::OnConfigChanged(bool b)\
    {\
        QCheckBox* edit = qobject_cast<QCheckBox*>(sender());\
        QString oldVal = b ? QObject::tr("Not Used") : QObject::tr("Used"); \
        QString newVal = b ? QObject::tr("Used") : QObject::tr("Not Used"); \
        OnConfigChanged(edit, newVal, oldVal);\
    }


class ICConfigString:public QObject
{
    Q_OBJECT
public:
    enum {
        kCS_Global_Speed,
        kCS_User_Changed,
        kCS_Mold_Changed,

        kCS_SIG_START = 100,
        kCS_SIG_Fixture1,
        kCS_SIG_Fixture2,
        kCS_SIG_Fixture3,
        kCS_SIG_Fixture4,
        kCS_SIG_IMM_Stop,
        kCS_SIG_Z_Move_Pos,
        kCS_SIG_Origin,
        kCS_SIG_Y_Origin_Pos,
        kCS_SIG_Hor_Standby,
        kCS_SIG_CM_Limit,
        kCS_SIG_SEDoor,
        kCS_SIG_Press,
        kCS_SIG_Mid_Mold,
        kCS_SIG_EJE_Linked,
        kCS_SIG_Auto,

        kCS_PRD_START = 200,
        kCS_PRD_Number,
        kCS_PRD_Wait_OM_Limit,
        kCS_PRD_Alarm_Time,
        kCS_PRD_Cycle_Time,
        kCS_PRD_Fixture_Define,
        kCS_PRD_Alarm_Occasion_When_Get_Fail,
        kCS_PRD_Transport_Count_Way,
        kCS_PRD_Save_Count,
        kCS_PRD_Product_Clear,
        kCS_PRD_Try_number,
        kCS_PRD_Sample_cycle,

        kCS_RUN_START = 300,
        kCS_RUN_Tolerance,
        kCS_RUN_Distance_X1_X2,
        kCS_RUN_Acc_Time_X1,
        kCS_RUN_Acc_Time_Y1,
        kCS_RUN_Acc_Time_Z,
        kCS_RUN_Acc_Time_X2,
        kCS_RUN_Acc_Time_Y2,
        kCS_RUN_Acc_Time_A,
        kCS_RUN_Acc_Time_B,
        kCS_RUN_Acc_Time_C,
        kCS_RUN_Speed_Limit_X1,
        kCS_RUN_Speed_Limit_Y1,
        kCS_RUN_Speed_Limit_Z,
        kCS_RUN_Speed_Limit_X2,
        kCS_RUN_Speed_Limit_Y2,
        kCS_RUN_Speed_Limit_A,
        kCS_RUN_Speed_Limit_B,
        kCS_RUN_Speed_Limit_C,

        kCS_SECP_START = 400,
        kCS_SECP_Inside_X_Min,
        kCS_SECP_Inside_X_Max,
        kCS_SECP_Outside_X_Min,
        kCS_SECP_Outside_X_Max,

        kCS_STRUCT_START = 500,
        kCS_STRUCT_Axis_Define_X1,
        kCS_STRUCT_Axis_Define_Y1,
        kCS_STRUCT_Axis_Define_Z,
        kCS_STRUCT_Axis_Define_X2,
        kCS_STRUCT_Axis_Define_Y2,
        kCS_STRUCT_Axis_Define_A,
        kCS_STRUCT_Axis_Define_B,
        kCS_STRUCT_Axis_Define_C,
        kCS_STRUCT_Reserve_Define_Y017,
        kCS_STRUCT_Reserve_Define_Y022,
        kCS_STRUCT_Reserve_Define_Y031,
        kCS_STRUCT_Reserve_Define_Y032,
        kCS_STRUCT_Reserve_Define_Y035,
        kCS_STRUCT_Reserve_Define_Y036,
        kCS_STRUCT_Reserve_Define_R1,
        kCS_STRUCT_Reserve_Define_R2,
        kCS_STRUCT_Limit_Define_Arm_Num,
        kCS_STRUCT_Limit_Define_Arm_X_Limit,
        kCS_STRUCT_Limit_Define_Arm_Y_Limit,
        kCS_STRUCT_Limit_Define_Arm_S_Limit,
        kCS_STRUCT_Limit_Define_Arm_R_Limit,
        kCS_STRUCT_Limit_Define_Arm_T_Limit,
        kCS_STRUCT_Limit_Define_Arm_BW_Limit,
        kCS_STRUCT_Other_Define_Escape,
        kCS_STRUCT_Other_Define_Tune_bit,
        kCS_STRUCT_Other_Define_Inside_Position,
        kCS_STRUCT_Other_Define_Servo_Flex,
        kCS_STRUCT_Config_Save,
        kCS_STRUCT_Config_CanType,
        kCS_STRUCT_Config_CanId,
        kCS_STRUCT_Config_PunchType,
        kCS_STRUCT_Config_PressureCheck,
        kCS_STRUCT_Config_TryRun,
        kCS_STRUCT_Config_DownMode,
        kCS_STRUCT_Config_Flee,
        kCS_STRUCT_Config_Origin,
        kCS_STRUCT_Config_Auto,
        kCS_STRUCT_Config_OriginX,
        kCS_STRUCT_Config_OriginY,
        kCS_STRUCT_Config_OriginS,
        kCS_STRUCT_Config_OriginR,
        kCS_STRUCT_Config_OriginT,
        kCS_STRUCT_Config_SyncOrigin,
        kCS_STRUCT_Config_OriginSpeed,
        kCS_STRUCT_Config_Suck1,
        kCS_STRUCT_Config_Suck2,
        kCS_STRUCT_Config_Clip1,
        kCS_STRUCT_Config_Clip2,

        kCS_TIME_START = 600,
        kCS_TIME_Ver1,
        kCS_TIME_Hor1,
        kCS_TIME_Ver2,
        kCS_TIME_Hor2,
        kCS_TIME_MArm_UP,
        kCS_TIME_MArm_Dw,
        kCS_TIME_MArm_FW,
        kCS_TIME_MArm_BW,
        kCS_TIME_Z_FW,
        kCS_TIME_Z_BW,
        kCS_TIME_SArm_UP,
        kCS_TIME_SArm_DW,
        kCS_TIME_SArm_FW,
        kCS_TIME_SArm_BW,

        kCS_STACK_START = 700,
        kCS_STACK_Group1_Seq,
        kCS_STACK_Group2_Seq,
        kCS_STACK_Group3_Seq,
        kCS_STACK_Group4_Seq,
        kCS_STACK_Group1_X_Dir,
        kCS_STACK_Group2_X_Dir,
        kCS_STACK_Group3_X_Dir,
        kCS_STACK_Group4_X_Dir,
        kCS_STACK_Group1_Y_Dir,
        kCS_STACK_Group2_Y_Dir,
        kCS_STACK_Group3_Y_Dir,
        kCS_STACK_Group4_Y_Dir,
        kCS_STACK_Group1_Z_Dir,
        kCS_STACK_Group2_Z_Dir,
        kCS_STACK_Group3_Z_Dir,
        kCS_STACK_Group4_Z_Dir,
        kCS_STACK_Group1_X_Number,
        kCS_STACK_Group2_X_Number,
        kCS_STACK_Group3_X_Number,
        kCS_STACK_Group4_X_Number,
        kCS_STACK_Group1_Y_Number,
        kCS_STACK_Group2_Y_Number,
        kCS_STACK_Group3_Y_Number,
        kCS_STACK_Group4_Y_Number,
        kCS_STACK_Group1_Z_Number,
        kCS_STACK_Group2_Z_Number,
        kCS_STACK_Group3_Z_Number,
        kCS_STACK_Group4_Z_Number,
        kCS_STACK_Group1_X_Space,
        kCS_STACK_Group2_X_Space,
        kCS_STACK_Group3_X_Space,
        kCS_STACK_Group4_X_Space,
        kCS_STACK_Group1_Y_Space,
        kCS_STACK_Group2_Y_Space,
        kCS_STACK_Group3_Y_Space,
        kCS_STACK_Group4_Y_Space,
        kCS_STACK_Group1_Z_Space,
        kCS_STACK_Group2_Z_Space,
        kCS_STACK_Group3_Z_Space,
        kCS_STACK_Group4_Z_Space,
        kCS_STACK_Group1_Is_SArm,
        kCS_STACK_Group2_Is_SArm,
        kCS_STACK_Group3_Is_SArm,
        kCS_STACK_Group4_Is_SArm,
        kCS_STACK_Group1_Count_Way,
        kCS_STACK_Group2_Count_Way,
        kCS_STACK_Group3_Count_Way,
        kCS_STACK_Group4_Count_Way,

        kCS_AXIS_START = 800,
        kCS_AXIS_Length_X1,
        kCS_AXIS_Length_Y1,
        kCS_AXIS_Length_Z,
        kCS_AXIS_Length_X2,
        kCS_AXIS_Length_Y2,
        kCS_AXIS_Length_A,
        kCS_AXIS_Length_B,
        kCS_AXIS_Length_C,
        kCS_AXIS_Move_Limit_X1,
        kCS_AXIS_Move_Limit_Y1,
        kCS_AXIS_Move_Limit_Z,
        kCS_AXIS_Move_Limit_X2,
        kCS_AXIS_Move_Limit_Y2,
        kCS_AXIS_Move_Limit_A,
        kCS_AXIS_Move_Limit_B,
        kCS_AXIS_Move_Limit_C,
        kCS_AXIS_Min_X1,
        kCS_AXIS_Min_Y1,
        kCS_AXIS_Min_Z,
        kCS_AXIS_Min_X2,
        kCS_AXIS_Min_Y2,
        kCS_AXIS_Min_A,
        kCS_AXIS_Min_B,
        kCS_AXIS_Min_C,
        kCS_AXIS_Max_X1,
        kCS_AXIS_Max_Y1,
        kCS_AXIS_Max_Z,
        kCS_AXIS_Max_X2,
        kCS_AXIS_Max_Y2,
        kCS_AXIS_Max_A,
        kCS_AXIS_Max_B,
        kCS_AXIS_Max_C,
        kCS_AXIS_Rotate_X1,
        kCS_AXIS_Rotate_Y1,
        kCS_AXIS_Rotate_Z,
        kCS_AXIS_Rotate_X2,
        kCS_AXIS_Rotate_Y2,
        kCS_AXIS_Rotate_A,
        kCS_AXIS_Rotate_B,
        kCS_AXIS_Rotate_C,
        kCS_AXIS_Config_Save,

        kCS_PANEL_START = 900,
        kCS_PANEL_Key_Tone,
        kCS_PANEL_Touch_Recal,
        kCS_PANEL_Language,
        kCS_PANEL_Datetime,
        kCS_PANEL_Backlight,
        kCS_PANEL_Bright,
        kCS_PANEL_Admin_Password,
        kCS_PANEL_Root_Password,
        kCS_PANEL_Super_Password,
        kCS_PANEL_Function_Extent,
        kCS_PANEL_Register_Extent,
        kCS_PANEL_Config_Save,

        kCS_CARE_START = 1000,
        kCS_CARE_Item1_Cycle,
        kCS_CARE_Item2_Cycle,
        kCS_CARE_Item3_Cycle,
        kCS_CARE_Item4_Cycle,
        kCS_CARE_Item5_Cycle,
        kCS_CARE_Item6_Cycle,
        kCS_CARE_Item7_Cycle,


        kCS_ERR_START = 10000,
        kCS_Err,

    };


    ICConfigString();

    static QString ConfigString(int configID);

};

#endif // ICCONFIGSTRING_H
