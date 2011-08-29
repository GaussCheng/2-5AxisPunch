#ifndef ICACTIONCOMMAND_H
#define ICACTIONCOMMAND_H

namespace IC{
enum ICActionCommand
{
    CMD_NULL            = 0x0,      //无命令
    CMD_TurnAuto        = 0x80,     //转自动
    CMD_TurnManual      = 0x81,     //转手动
    CMD_TurnStop        = 0x82,     //转停止
    CMD_TurnTeach       = 0x83,     //转教导
    CMD_TurnZero        = 0x84,     //原点
    CMD_TurnRet         = 0x85,     //复归
    CMD_TurntchSub0     = 0x90,     //转教导巨集0
    CMD_TurntchSub1     = 0x91,     //转教导巨集1
    CMD_TurntchSub2     = 0x92,     //转教导巨集2
    CMD_TurntchSub3     = 0x93,     //转教导巨集3
    CMD_TurntchSub4     = 0x94,     //转教导巨集4
    CMD_TurntchSub5     = 0x95,     //转教导巨集5
    CMD_TurntchSub6     = 0x96,     //转教导巨集6
    CMD_TurntchSub7     = 0x97,     //转教导巨集7
    CMD_TurntchSub8     = 0x98,     //转教导

    CMD_Action          = 0x40,     //动作类

    CMD_TestDone        = 0x20,     //测试结束
    CMD_TestX           = 0x21,     //测试X轴脉冲
    CMD_TestY           = 0x22,     //测试Y轴脉冲
    CMD_TestZ           = 0x23,     //测试Z轴脉冲
    CMD_TestxRev        = 0x2c,     //测试X轴反向
    CMD_TestzRev        = 0x2d,     //测试Z轴反向
    CMD_TestyRev        = 0x2e,
    CMD_TestClr         = 0x24,     //清测试脉冲

    CMD_X1SubLmt        = 0x25,
    CMD_X1AddLmt        = 0x26,
    CMD_Y1SubLmt        = 0x27,
    CMD_Y1AddLmt        = 0x28,
    CMD_ZSubLmt         = 0x29,
    CMD_ZAddLmt         = 0x2a,

    CMD_Insert          = 0x31,     //插入
    CMD_Delete          = 0x32,     //删除
    CMD_Teach           = 0x33,     //教导回传

    CMD_PulseA          = 0x60,     //脉冲命令A
    CMD_PulseB          = 0x61,     //脉冲命令B
};
}//namespace IC{

#endif // ICACTIONCOMMAND_H
