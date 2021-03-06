#include "icconfigstring.h"
#include "icvirtualhost.h"
#include "icmold.h"

ICConfigString::ICConfigString()
{
}

//#AUTO GEN BEGIN 
QString ICConfigString::ConfigString(int configID)
{
    switch (configID)
    {
    case kCS_Global_Speed: return tr("kCS_Global_Speed");
    case kCS_User_Changed: return tr("kCS_User_Changed");
    case kCS_Mold_Changed: return tr("kCS_Mold_Changed");
    case kCS_SIG_Fixture1: return tr("kCS_SIG_Fixture1");
    case kCS_SIG_Fixture2: return tr("kCS_SIG_Fixture2");
    case kCS_SIG_Fixture3: return tr("kCS_SIG_Fixture3");
    case kCS_SIG_Fixture4: return tr("kCS_SIG_Fixture4");
    case kCS_SIG_IMM_Stop: return tr("kCS_SIG_IMM_Stop");
    case kCS_SIG_Z_Move_Pos: return tr("kCS_SIG_Z_Move_Pos");
    case kCS_SIG_Origin: return tr("kCS_SIG_Origin");
    case kCS_SIG_Y_Origin_Pos: return tr("kCS_SIG_Y_Origin_Pos");
    case kCS_SIG_Hor_Standby: return tr("kCS_SIG_Hor_Standby");
    case kCS_SIG_CM_Limit: return tr("kCS_SIG_CM_Limit");
    case kCS_SIG_SEDoor: return tr("kCS_SIG_SEDoor");
    case kCS_SIG_Press: return tr("kCS_SIG_Press");
    case kCS_SIG_Mid_Mold: return tr("kCS_SIG_Mid_Mold");
    case kCS_SIG_EJE_Linked: return tr("kCS_SIG_EJE_Linked");
    case kCS_SIG_Auto: return tr("kCS_SIG_Auto");
    case kCS_PRD_Number: return tr("kCS_PRD_Number");
    case kCS_PRD_Wait_OM_Limit: return tr("kCS_PRD_Wait_OM_Limit");
    case kCS_PRD_Alarm_Time: return tr("kCS_PRD_Alarm_Time");
    case kCS_PRD_Cycle_Time: return tr("kCS_PRD_Cycle_Time");
    case kCS_PRD_Fixture_Define: return tr("kCS_PRD_Fixture_Define");
    case kCS_PRD_Alarm_Occasion_When_Get_Fail: return tr("kCS_PRD_Alarm_Occasion_When_Get_Fail");
    case kCS_PRD_Transport_Count_Way: return tr("kCS_PRD_Transport_Count_Way");
    case kCS_PRD_Save_Count: return tr("kCS_PRD_Save_Count");
    case kCS_PRD_Product_Clear: return tr("kCS_PRD_Product_Clear");
    case kCS_PRD_Try_number: return tr("kCS_PRD_Try_number");
    case kCS_PRD_Sample_cycle: return tr("kCS_PRD_Sample_cycle");
    case kCS_RUN_Tolerance: return tr("kCS_RUN_Tolerance");
    case kCS_RUN_Distance_X1_X2: return tr("kCS_RUN_Distance_X1_X2");
    case kCS_RUN_Acc_Time_X1: return tr("kCS_RUN_Acc_Time_X1");
    case kCS_RUN_Acc_Time_Y1: return tr("kCS_RUN_Acc_Time_Y1");
    case kCS_RUN_Acc_Time_Z: return tr("kCS_RUN_Acc_Time_Z");
    case kCS_RUN_Acc_Time_X2: return tr("kCS_RUN_Acc_Time_X2");
    case kCS_RUN_Acc_Time_Y2: return tr("kCS_RUN_Acc_Time_Y2");
    case kCS_RUN_Acc_Time_A: return tr("kCS_RUN_Acc_Time_A");
    case kCS_RUN_Acc_Time_B: return tr("kCS_RUN_Acc_Time_B");
    case kCS_RUN_Acc_Time_C: return tr("kCS_RUN_Acc_Time_C");
    case kCS_RUN_Speed_Limit_X1: return tr("kCS_RUN_Speed_Limit_X1");
    case kCS_RUN_Speed_Limit_Y1: return tr("kCS_RUN_Speed_Limit_Y1");
    case kCS_RUN_Speed_Limit_Z: return tr("kCS_RUN_Speed_Limit_Z");
    case kCS_RUN_Speed_Limit_X2: return tr("kCS_RUN_Speed_Limit_X2");
    case kCS_RUN_Speed_Limit_Y2: return tr("kCS_RUN_Speed_Limit_Y2");
    case kCS_RUN_Speed_Limit_A: return tr("kCS_RUN_Speed_Limit_A");
    case kCS_RUN_Speed_Limit_B: return tr("kCS_RUN_Speed_Limit_B");
    case kCS_RUN_Speed_Limit_C: return tr("kCS_RUN_Speed_Limit_C");
    case kCS_SECP_Inside_X_Min: return tr("kCS_SECP_Inside_X_Min");
    case kCS_SECP_Inside_X_Max: return tr("kCS_SECP_Inside_X_Max");
    case kCS_SECP_Outside_X_Min: return tr("kCS_SECP_Outside_X_Min");
    case kCS_SECP_Outside_X_Max: return tr("kCS_SECP_Outside_X_Max");
    case kCS_STRUCT_Axis_Define_X1: return tr("kCS_STRUCT_Axis_Define_X1");
    case kCS_STRUCT_Axis_Define_Y1: return tr("kCS_STRUCT_Axis_Define_Y1");
    case kCS_STRUCT_Axis_Define_Z: return tr("kCS_STRUCT_Axis_Define_Z");
    case kCS_STRUCT_Axis_Define_X2: return tr("kCS_STRUCT_Axis_Define_X2");
    case kCS_STRUCT_Axis_Define_Y2: return tr("kCS_STRUCT_Axis_Define_Y2");
    case kCS_STRUCT_Axis_Define_A: return tr("kCS_STRUCT_Axis_Define_A");
    case kCS_STRUCT_Axis_Define_B: return tr("kCS_STRUCT_Axis_Define_B");
    case kCS_STRUCT_Axis_Define_C: return tr("kCS_STRUCT_Axis_Define_C");
    case kCS_STRUCT_Reserve_Define_Y017: return tr("kCS_STRUCT_Reserve_Define_Y017");
    case kCS_STRUCT_Reserve_Define_Y022: return tr("kCS_STRUCT_Reserve_Define_Y022");
    case kCS_STRUCT_Reserve_Define_Y031: return tr("kCS_STRUCT_Reserve_Define_Y031");
    case kCS_STRUCT_Reserve_Define_Y032: return tr("kCS_STRUCT_Reserve_Define_Y032");
    case kCS_STRUCT_Reserve_Define_Y035: return tr("kCS_STRUCT_Reserve_Define_Y035");
    case kCS_STRUCT_Reserve_Define_Y036: return tr("kCS_STRUCT_Reserve_Define_Y036");
    case kCS_STRUCT_Reserve_Define_R1: return tr("kCS_STRUCT_Reserve_Define_R1");
    case kCS_STRUCT_Reserve_Define_R2: return tr("kCS_STRUCT_Reserve_Define_R2");
    case kCS_STRUCT_Limit_Define_Arm_Num: return tr("kCS_STRUCT_Limit_Define_Arm_Num");
    case kCS_STRUCT_Limit_Define_Arm_X_Limit:return tr("kCS_STRUCT_Limit_Define_Arm_X_Limit");
    case kCS_STRUCT_Limit_Define_Arm_Y_Limit:return tr("kCS_STRUCT_Limit_Define_Arm_Y_Limit");
    case kCS_STRUCT_Limit_Define_Arm_S_Limit:return tr("kCS_STRUCT_Limit_Define_Arm_S_Limit");
    case kCS_STRUCT_Limit_Define_Arm_R_Limit:return tr("kCS_STRUCT_Limit_Define_Arm_R_Limit");
    case kCS_STRUCT_Limit_Define_Arm_T_Limit:return tr("kCS_STRUCT_Limit_Define_Arm_T_Limit");
    case kCS_STRUCT_Config_CanType:return tr("kCS_STRUCT_Config_CanType");
    case kCS_STRUCT_Config_CanId:return tr("kCS_STRUCT_Config_CanId");
    case kCS_STRUCT_Config_PunchType:return tr("kCS_STRUCT_Config_PunchType");
    case kCS_STRUCT_Config_PressureCheck:return tr("kCS_STRUCT_Config_PressureCheck");
    case kCS_STRUCT_Config_TryRun:return tr("kCS_STRUCT_Config_TryRun");
    case kCS_STRUCT_Config_DownMode:return tr("kCS_STRUCT_Config_DownMode");
    case kCS_STRUCT_Config_Flee:return tr("kCS_STRUCT_Config_Flee");
    case kCS_STRUCT_Config_Origin:return tr("kCS_STRUCT_Config_Origin");
    case kCS_STRUCT_Config_Auto:return tr("kCS_STRUCT_Config_Auto");
    case kCS_STRUCT_Config_OriginX:return tr("kCS_STRUCT_Config_OriginX");
    case kCS_STRUCT_Config_OriginY:return tr("kCS_STRUCT_Config_OriginY");
    case kCS_STRUCT_Config_OriginS:return tr("kCS_STRUCT_Config_OriginS");
    case kCS_STRUCT_Config_OriginR:return tr("kCS_STRUCT_Config_OriginR");
    case kCS_STRUCT_Config_OriginT:return tr("kCS_STRUCT_Config_OriginT");
    case kCS_STRUCT_Config_SyncOrigin:return tr("kCS_STRUCT_Config_SyncOrigin");
    case kCS_STRUCT_Config_OriginSpeed:return tr("kCS_STRUCT_Config_OriginSpeed");
    case kCS_STRUCT_Config_Suck1:return tr("kCS_STRUCT_Config_Suck1");
    case kCS_STRUCT_Config_Suck2:return tr("kCS_STRUCT_Config_Suck2");
    case kCS_STRUCT_Config_Clip1:return tr("kCS_STRUCT_Config_Clip1");
    case kCS_STRUCT_Config_Clip2:return tr("kCS_STRUCT_Config_Clip2");
    case kCS_STRUCT_Limit_Define_Arm_BW_Limit: return tr("kCS_STRUCT_Limit_Define_SArm_BW_Limit");
    case kCS_STRUCT_Other_Define_Escape: return tr("kCS_STRUCT_Other_Define_Escape");
    case kCS_STRUCT_Other_Define_Tune_bit: return tr("kCS_STRUCT_Other_Define_Tune_bit");
    case kCS_STRUCT_Other_Define_Inside_Position: return tr("kCS_STRUCT_Other_Define_Inside_Position");
    case kCS_STRUCT_Other_Define_Servo_Flex: return tr("kCS_STRUCT_Other_Define_Servo_Flex");
    case kCS_STRUCT_Config_Save: return tr("kCS_STRUCT_Config_Save");
    case kCS_TIME_Ver1: return tr("kCS_TIME_Ver1");
    case kCS_TIME_Hor1: return tr("kCS_TIME_Hor1");
    case kCS_TIME_Ver2: return tr("kCS_TIME_Ver2");
    case kCS_TIME_Hor2: return tr("kCS_TIME_Hor2");
    case kCS_TIME_MArm_UP: return tr("kCS_TIME_MArm_UP");
    case kCS_TIME_MArm_Dw: return tr("kCS_TIME_MArm_Dw");
    case kCS_TIME_MArm_FW: return tr("kCS_TIME_MArm_FW");
    case kCS_TIME_MArm_BW: return tr("kCS_TIME_MArm_BW");
    case kCS_TIME_Z_FW: return tr("kCS_TIME_Z_FW");
    case kCS_TIME_Z_BW: return tr("kCS_TIME_Z_BW");
    case kCS_TIME_SArm_UP: return tr("kCS_TIME_SArm_UP");
    case kCS_TIME_SArm_DW: return tr("kCS_TIME_SArm_DW");
    case kCS_TIME_SArm_FW: return tr("kCS_TIME_SArm_FW");
    case kCS_TIME_SArm_BW: return tr("kCS_TIME_SArm_BW");
    case kCS_STACK_Group1_Seq: return tr("kCS_STACK_Group1_Seq");
    case kCS_STACK_Group2_Seq: return tr("kCS_STACK_Group2_Seq");
    case kCS_STACK_Group3_Seq: return tr("kCS_STACK_Group3_Seq");
    case kCS_STACK_Group4_Seq: return tr("kCS_STACK_Group4_Seq");
    case kCS_STACK_Group1_X_Dir: return tr("kCS_STACK_Group1_X_Dir");
    case kCS_STACK_Group2_X_Dir: return tr("kCS_STACK_Group2_X_Dir");
    case kCS_STACK_Group3_X_Dir: return tr("kCS_STACK_Group3_X_Dir");
    case kCS_STACK_Group4_X_Dir: return tr("kCS_STACK_Group4_X_Dir");
    case kCS_STACK_Group1_Y_Dir: return tr("kCS_STACK_Group1_Y_Dir");
    case kCS_STACK_Group2_Y_Dir: return tr("kCS_STACK_Group2_Y_Dir");
    case kCS_STACK_Group3_Y_Dir: return tr("kCS_STACK_Group3_Y_Dir");
    case kCS_STACK_Group4_Y_Dir: return tr("kCS_STACK_Group4_Y_Dir");
    case kCS_STACK_Group1_Z_Dir: return tr("kCS_STACK_Group1_Z_Dir");
    case kCS_STACK_Group2_Z_Dir: return tr("kCS_STACK_Group2_Z_Dir");
    case kCS_STACK_Group3_Z_Dir: return tr("kCS_STACK_Group3_Z_Dir");
    case kCS_STACK_Group4_Z_Dir: return tr("kCS_STACK_Group4_Z_Dir");
    case kCS_STACK_Group1_X_Number: return tr("kCS_STACK_Group1_X_Number");
    case kCS_STACK_Group2_X_Number: return tr("kCS_STACK_Group2_X_Number");
    case kCS_STACK_Group3_X_Number: return tr("kCS_STACK_Group3_X_Number");
    case kCS_STACK_Group4_X_Number: return tr("kCS_STACK_Group4_X_Number");
    case kCS_STACK_Group1_Y_Number: return tr("kCS_STACK_Group1_Y_Number");
    case kCS_STACK_Group2_Y_Number: return tr("kCS_STACK_Group2_Y_Number");
    case kCS_STACK_Group3_Y_Number: return tr("kCS_STACK_Group3_Y_Number");
    case kCS_STACK_Group4_Y_Number: return tr("kCS_STACK_Group4_Y_Number");
    case kCS_STACK_Group1_Z_Number: return tr("kCS_STACK_Group1_Z_Number");
    case kCS_STACK_Group2_Z_Number: return tr("kCS_STACK_Group2_Z_Number");
    case kCS_STACK_Group3_Z_Number: return tr("kCS_STACK_Group3_Z_Number");
    case kCS_STACK_Group4_Z_Number: return tr("kCS_STACK_Group4_Z_Number");
    case kCS_STACK_Group1_X_Space: return tr("kCS_STACK_Group1_X_Space");
    case kCS_STACK_Group2_X_Space: return tr("kCS_STACK_Group2_X_Space");
    case kCS_STACK_Group3_X_Space: return tr("kCS_STACK_Group3_X_Space");
    case kCS_STACK_Group4_X_Space: return tr("kCS_STACK_Group4_X_Space");
    case kCS_STACK_Group1_Y_Space: return tr("kCS_STACK_Group1_Y_Space");
    case kCS_STACK_Group2_Y_Space: return tr("kCS_STACK_Group2_Y_Space");
    case kCS_STACK_Group3_Y_Space: return tr("kCS_STACK_Group3_Y_Space");
    case kCS_STACK_Group4_Y_Space: return tr("kCS_STACK_Group4_Y_Space");
    case kCS_STACK_Group1_Z_Space: return tr("kCS_STACK_Group1_Z_Space");
    case kCS_STACK_Group2_Z_Space: return tr("kCS_STACK_Group2_Z_Space");
    case kCS_STACK_Group3_Z_Space: return tr("kCS_STACK_Group3_Z_Space");
    case kCS_STACK_Group4_Z_Space: return tr("kCS_STACK_Group4_Z_Space");
    case kCS_STACK_Group1_Is_SArm: return tr("kCS_STACK_Group1_Is_SArm");
    case kCS_STACK_Group2_Is_SArm: return tr("kCS_STACK_Group2_Is_SArm");
    case kCS_STACK_Group3_Is_SArm: return tr("kCS_STACK_Group3_Is_SArm");
    case kCS_STACK_Group4_Is_SArm: return tr("kCS_STACK_Group4_Is_SArm");
    case kCS_STACK_Group1_Count_Way: return tr("kCS_STACK_Group1_Count_Way");
    case kCS_STACK_Group2_Count_Way: return tr("kCS_STACK_Group2_Count_Way");
    case kCS_STACK_Group3_Count_Way: return tr("kCS_STACK_Group3_Count_Way");
    case kCS_STACK_Group4_Count_Way: return tr("kCS_STACK_Group4_Count_Way");
    case kCS_AXIS_Length_X1: return tr("kCS_AXIS_Length_X1");
    case kCS_AXIS_Length_Y1: return tr("kCS_AXIS_Length_Y1");
    case kCS_AXIS_Length_Z: return tr("kCS_AXIS_Length_Z");
    case kCS_AXIS_Length_X2: return tr("kCS_AXIS_Length_X2");
    case kCS_AXIS_Length_Y2: return tr("kCS_AXIS_Length_Y2");
    case kCS_AXIS_Length_A: return tr("kCS_AXIS_Length_A");
    case kCS_AXIS_Length_B: return tr("kCS_AXIS_Length_B");
    case kCS_AXIS_Length_C: return tr("kCS_AXIS_Length_C");
    case kCS_AXIS_Move_Limit_X1: return tr("kCS_AXIS_Move_Limit_X1");
    case kCS_AXIS_Move_Limit_Y1: return tr("kCS_AXIS_Move_Limit_Y1");
    case kCS_AXIS_Move_Limit_Z: return tr("kCS_AXIS_Move_Limit_Z");
    case kCS_AXIS_Move_Limit_X2: return tr("kCS_AXIS_Move_Limit_X2");
    case kCS_AXIS_Move_Limit_Y2: return tr("kCS_AXIS_Move_Limit_Y2");
    case kCS_AXIS_Move_Limit_A: return tr("kCS_AXIS_Move_Limit_A");
    case kCS_AXIS_Move_Limit_B: return tr("kCS_AXIS_Move_Limit_B");
    case kCS_AXIS_Move_Limit_C: return tr("kCS_AXIS_Move_Limit_C");
    case kCS_AXIS_Min_X1: return tr("kCS_AXIS_Min_X1");
    case kCS_AXIS_Min_Y1: return tr("kCS_AXIS_Min_Y1");
    case kCS_AXIS_Min_Z: return tr("kCS_AXIS_Min_Z");
    case kCS_AXIS_Min_X2: return tr("kCS_AXIS_Min_X2");
    case kCS_AXIS_Min_Y2: return tr("kCS_AXIS_Min_Y2");
    case kCS_AXIS_Min_A: return tr("kCS_AXIS_Min_A");
    case kCS_AXIS_Min_B: return tr("kCS_AXIS_Min_B");
    case kCS_AXIS_Min_C: return tr("kCS_AXIS_Min_C");
    case kCS_AXIS_Max_X1: return tr("kCS_AXIS_Max_X1");
    case kCS_AXIS_Max_Y1: return tr("kCS_AXIS_Max_Y1");
    case kCS_AXIS_Max_Z: return tr("kCS_AXIS_Max_Z");
    case kCS_AXIS_Max_X2: return tr("kCS_AXIS_Max_X2");
    case kCS_AXIS_Max_Y2: return tr("kCS_AXIS_Max_Y2");
    case kCS_AXIS_Max_A: return tr("kCS_AXIS_Max_A");
    case kCS_AXIS_Max_B: return tr("kCS_AXIS_Max_B");
    case kCS_AXIS_Max_C: return tr("kCS_AXIS_Max_C");
    case kCS_AXIS_Rotate_X1: return tr("kCS_AXIS_Rotate_X1");
    case kCS_AXIS_Rotate_Y1: return tr("kCS_AXIS_Rotate_Y1");
    case kCS_AXIS_Rotate_Z: return tr("kCS_AXIS_Rotate_Z");
    case kCS_AXIS_Rotate_X2: return tr("kCS_AXIS_Rotate_X2");
    case kCS_AXIS_Rotate_Y2: return tr("kCS_AXIS_Rotate_Y2");
    case kCS_AXIS_Rotate_A: return tr("kCS_AXIS_Rotate_A");
    case kCS_AXIS_Rotate_B: return tr("kCS_AXIS_Rotate_B");
    case kCS_AXIS_Rotate_C: return tr("kCS_AXIS_Rotate_C");
    case kCS_AXIS_Config_Save: return tr("kCS_AXIS_Config_Save");
    case kCS_PANEL_Key_Tone: return tr("kCS_PANEL_Key_Tone");
    case kCS_PANEL_Touch_Recal: return tr("kCS_PANEL_Touch_Recal");
    case kCS_PANEL_Language: return tr("kCS_PANEL_Language");
    case kCS_PANEL_Datetime: return tr("kCS_PANEL_Datetime");
    case kCS_PANEL_Backlight: return tr("kCS_PANEL_Backlight");
    case kCS_PANEL_Bright: return tr("kCS_PANEL_Bright");
    case kCS_PANEL_Admin_Password: return tr("kCS_PANEL_Admin_Password");
    case kCS_PANEL_Root_Password: return tr("kCS_PANEL_Root_Password");
    case kCS_PANEL_Super_Password: return tr("kCS_PANEL_Super_Password");
    case kCS_PANEL_Function_Extent: return tr("kCS_PANEL_Function_Extent");
    case kCS_PANEL_Register_Extent: return tr("kCS_PANEL_Register_Extent");
    case kCS_PANEL_Config_Save: return tr("kCS_PANEL_Config_Save");
    case kCS_CARE_Item1_Cycle: return tr("kCS_CARE_Item1_Cycle");
    case kCS_CARE_Item2_Cycle: return tr("kCS_CARE_Item2_Cycle");
    case kCS_CARE_Item3_Cycle: return tr("kCS_CARE_Item3_Cycle");
    case kCS_CARE_Item4_Cycle: return tr("kCS_CARE_Item4_Cycle");
    case kCS_CARE_Item5_Cycle: return tr("kCS_CARE_Item5_Cycle");
    case kCS_CARE_Item6_Cycle: return tr("kCS_CARE_Item6_Cycle");
    case kCS_CARE_Item7_Cycle: return tr("kCS_CARE_Item7_Cycle");
    case kCS_Err: return tr("kCS_Err");
    default: return tr("No IMPL");
    }
}
//#AUTO GEN END
