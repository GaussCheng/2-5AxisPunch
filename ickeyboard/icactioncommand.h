#ifndef ICACTIONCOMMAND_H
#define ICACTIONCOMMAND_H

namespace IC{
enum ICActionCommand
{
    CMD_NULL            = 0x0,      //������
    CMD_TurnAuto        = 0x80,     //ת�Զ�
    CMD_TurnManual      = 0x81,     //ת�ֶ�
    CMD_TurnStop        = 0x82,     //תֹͣ
    CMD_TurnTeach       = 0x83,     //ת�̵�
    CMD_TurnZero        = 0x84,     //ԭ��
    CMD_TurnRet         = 0x85,     //����
    CMD_TurntchSub0     = 0x90,     //ת�̵��޼�0
    CMD_TurntchSub1     = 0x91,     //ת�̵��޼�1
    CMD_TurntchSub2     = 0x92,     //ת�̵��޼�2
    CMD_TurntchSub3     = 0x93,     //ת�̵��޼�3
    CMD_TurntchSub4     = 0x94,     //ת�̵��޼�4
    CMD_TurntchSub5     = 0x95,     //ת�̵��޼�5
    CMD_TurntchSub6     = 0x96,     //ת�̵��޼�6
    CMD_TurntchSub7     = 0x97,     //ת�̵��޼�7
    CMD_TurntchSub8     = 0x98,     //ת�̵�

    CMD_Action          = 0x40,     //������

    CMD_TestDone        = 0x20,     //���Խ���
    CMD_TestX           = 0x21,     //����X������
    CMD_TestY           = 0x22,     //����Y������
    CMD_TestZ           = 0x23,     //����Z������
    CMD_TestxRev        = 0x2c,     //����X�ᷴ��
    CMD_TestzRev        = 0x2d,     //����Z�ᷴ��
    CMD_TestyRev        = 0x2e,
    CMD_TestClr         = 0x24,     //���������

    CMD_X1SubLmt        = 0x25,
    CMD_X1AddLmt        = 0x26,
    CMD_Y1SubLmt        = 0x27,
    CMD_Y1AddLmt        = 0x28,
    CMD_ZSubLmt         = 0x29,
    CMD_ZAddLmt         = 0x2a,

    CMD_TuneMold        = 0x30,
    CMD_GoOn            = 0x31,
    CMD_GiveUp          = 0x32,

    CMD_X2SubLmt        = 0x35,
    CMD_X2AddLmt        = 0x36,
    CMD_Y2SubLmt        = 0x37,
    CMD_Y2AddLmt        = 0x38,

    CMD_TestX2          = 0x39,     //����X������
    CMD_TestY2          = 0x3a,     //����Y������
    CMD_TestZ2          = 0x3b,     //����Z������
    CMD_TestX2Rev       = 0x3c,     //����X�ᷴ��
    CMD_TestY2Rev       = 0x3d,     //����Z�ᷴ��
    CMD_TestZ2Rev       = 0x3e,

    CMD_TestA           = 0x50,     //����X������
    CMD_TestB           = 0x51,     //����Y������
    CMD_TestC           = 0x52,     //����Z������
    CMD_TestARev        = 0x53,     //����X�ᷴ��
    CMD_TestBRev        = 0x54,     //����Z�ᷴ��
    CMD_TestCRev        = 0x55,

    CMD_PulseA          = 0x60,     //��������A
    CMD_PulseB          = 0x61,     //��������B
};
}//namespace IC{

#endif // ICACTIONCOMMAND_H
