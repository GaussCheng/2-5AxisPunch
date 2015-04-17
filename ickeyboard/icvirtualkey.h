#ifndef ICVIRTUALKEY_H
#define ICVIRTUALKEY_H

namespace IC{
enum ICVirtualKey
{
    VKEY_NULL   = 0xC0,
    VKEY_START,
    VKEY_STOP,
    VKEY_UP,
    VKEY_DOWN,
    VKEY_F1,
    VKEY_F2,
    VKEY_F3,
    VKEY_F4,
    VKEY_F5,
    VKEY_F6,
    VKEY_X1SUB,
    VKEY_X1ADD,
    VKEY_Y1SUB,
    VKEY_Y1ADD,
    VKEY_ZSUB,
    VKEY_ZADD,
    VKEY_HORI,
    VKEY_VERT,
    VKEY_SP1,
    VKEY_SP2,
    VKEY_X2SUB,
    VKEY_X2ADD,
    VKEY_Y2SUB,
    VKEY_Y2ADD,
    VKEY_ORIGIN,
    VKEY_RETURN,
    VKEY_ASUB,
    VKEY_AADD,
    VKEY_BSUB,
    VKEY_BADD,
    VKEY_CSUB,
    VKEY_CADD,
    VKEY_CYCLE = 0xDF,

    VKEY_BIASX  = 0xE0,
    VKEY_BIASY,
    VKEY_BIASZ,
    VKEY_CURVEX,
    VKEY_CURVEY,
    VKEY_CURVEZ,

    VKEY_JOGEn = 0xE6,  //使能
    VKEY_JOG0,        //AubBack+
    VKEY_JOG1,      //AubBack-
    VKEY_JOG2,      //Forward+
    VKEY_JOG3,      //Forward-
    VKEY_JOG4,      //Up+
    VKEY_PRODUCT_CLEAR,
    VKEY_Y_SPEED_UP,
    VKEY_Y_SPEED_DOWN,
    VKEY_Z_SPEED_UP,
//    VKEY_JOG5,      //Up-
//    VKEY_JOG6,      //Down+
//    VKEY_JOG7,      //Down-


    VKEY_Z_SPEED_DOWN = 0xF0,
    VKEY_R_SPEED_UP,
    VKEY_R_SPEED_DOWN,
    VKEY_T_SPEED_UP,
    VKEY_T_SPEED_DOWN,
    VKEY_RESERVE6_ON,
    VKEY_RESERVE1_OFF = 0xF8,
    VKEY_RESERVE2_OFF,
    VKEY_RESERVE3_OFF,
    VKEY_RESERVE4_OFF,
    VKEY_RESERVE5_OFF,
    VKEY_RESERVE6_OFF,


    VKEY_INS    = 0x81,
    VKEY_DEL,
    VKEY_MODIFY,
    VKEY_ACTOK,
    VKEY_CLIPOK,
    VKEY_IMMOK,
    VKEY_ESC,

    VKEY_CLSMDON,
    VKEY_CLSMDOFF,
    VKEY_EJECTON,
    VKEY_EJECTOFF,
    VKEY_LAYOUTON,
    VKEY_LAYOUTOFF,

    VKEY_CLIP1ON    = 0x90,
    VKEY_CLIP2ON,
    VKEY_CLIP3ON,
    VKEY_CLIP4ON,
    VKEY_CLIP5ON,
    VKEY_CLIP6ON,
    VKEY_CLIP7ON,
    VKEY_CLIP8ON,
    VKEY_CLIP1OFF,
    VKEY_CLIP2OFF,
    VKEY_CLIP3OFF,
    VKEY_CLIP4OFF,
    VKEY_CLIP5OFF,
    VKEY_CLIP6OFF,
    VKEY_CLIP7OFF,
    VKEY_CLIP8OFF,


    VKEY_SET_ORIGIN = 0xA0,
    VKEY_ORIGIN_X1,
    VKEY_ORIGIN_Y1,
    VKEY_ORIGIN_Z,
    VKEY_ORIGIN_X2,
    VKEY_ORIGIN_Y2,
    VKEY_SERVO_ON,
    VKEY_SERVO_OFF,
    VKEY_ALLOWTACK,//允许取物
    VKEY_ALLOWPUT,//允许置物

    VKEY_LHORIGIN = 0xAA,
    VKEY_RHORIGIN = 0xAB,
    VKEY_SINGLECYCLE //单循环

};

}//namespace IC{

#endif // ICVIRTUALKEY_H
