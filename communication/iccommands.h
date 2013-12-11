#ifndef ICCOMMANDS_H
#define ICCOMMANDS_H

#include <QVariant>

#include "iccommunicationcommandbase.h"
#include "modbus.h"

class ICGetErrorCommand: public ICCommunicationCommandBase
{
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICWriteParameters: public ICCommunicationCommandBase
{
public:
    QVector<uint8_t> DataSection() const { return dataSection_;}
    void SetDataSection(const QVector<uint8_t> &dataSection) { dataSection_ = dataSection;}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
private:
    QVector<uint8_t> dataSection_;
};

class ICQueryStatus: public ICCommunicationCommandBase
{
public:
//    bool NeedToGetTeach() const { return needToGetTeach_;}
    bool NeedToReconfig() const { return needToReconfig_;}
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
private:
//    bool needToGetTeach_;
    bool needToReconfig_;
};

class ICExectueCommand: public ICCommunicationCommandBase
{
public:
    ICExectueCommand();
    int KeyValue() const { return FlagValue("KeyValue");}
    void SetKeyValue(int keyValue) { SetFlagValue("KeyValue", keyValue); }

    int SyncAct() const { return FlagValue("SyncAct");}
    void SetSyncAct(uint act) { SetFlagValue("SyncAct", act);}

    int SyncSum() const { return FlagValue("SyncSum");}
    void SetSycnSum(uint sum) { SetFlagValue("SyncSum", sum);}

    int CommandCode() const { return FlagValue("CommandCode");}
    void SetCommandCode(int cmd) { SetFlagValue("CommandCode", cmd);}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICModifySysParamCommand: public ICCommunicationCommandBase
{
public:
    ICModifySysParamCommand();
    int ParamValue() const { return FlagValue("ParamValue");}
    void SetParamValue(int value) { SetFlagValue("ParamValue", value);}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICTeachParamCommand: public ICCommunicationCommandBase
{
public:
    ICTeachParamCommand();
    int ParamValue() const { return FlagValue("ParamValue");}
    void SetParamValue(int value) { SetFlagValue("ParamValue", value);}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICTeachStepCommand: public ICCommunicationCommandBase
{
public:
    typedef QList<QList<uint> > ActionItem;
    ICTeachStepCommand();
//    int StepValue() const { return FlagValue("StepValue");}
//    void SetStepValue(int value) { SetFlagValue("StepValue", value);}
    ActionItem CurrentAction() const;

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
private:
    ActionItem actionItem_;
};

class ICAutoAdjustCommand: public ICCommunicationCommandBase
{
public:
    ICAutoAdjustCommand();

    int Sequence() const { return FlagValue("Sequence");}
    void SetSequence(int value) { SetFlagValue("Sequence", value);}

    int DelayTime() const { return FlagValue("DelayTime");}
    void SetDelayTime(int value) { SetFlagValue("DelayTime", value);}

    int Speed() const { return FlagValue("Speed");}
    void SetSpeed(int value) { SetFlagValue("Speed", value);}

    int DPos() const { return FlagValue("DPos");}
    void SetDPos(int value) { SetFlagValue("DPos", value);}

    int GMValue() const { return FlagValue("GMValue");}
    void SetGMValue(int value) { SetFlagValue("GMValue", value);}

    int CheckSum() const { return FlagValue("CheckSum");}
    void SetCheckSum(int value) { SetFlagValue("CheckSum", value);}
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
private:
};

class ICGetAxisConfigsCommand: public ICCommunicationCommandBase
{
public:
    ICGetAxisConfigsCommand();
    int Axis() const { return FlagValue("Axis");}
    void SetAxis(int value) { SetFlagValue("Axis", value);}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);

};

class ICSetAxisConfigsCommand: public ICCommunicationCommandBase
{
public:
    ICSetAxisConfigsCommand();
    int Axis() const { return FlagValue("Axis");}
    void SetAxis(int value) { SetFlagValue("Axis", value);}
    QVector<uint> DataBuffer() const { return dataBuffer_;}
    void SetDataBuffer(const QVector<uint> & dataBuffer) {dataBuffer_ = dataBuffer;}


protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
private:
    QVector<uint> dataBuffer_;
};

class ICSelecteConfigCommand: public ICCommunicationCommandBase
{
public:
    ICSelecteConfigCommand();
    int Selected() const { return FlagValue("Selected");}
    void SetSelected(int sel) { SetFlagValue("Selected", sel);}
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICUpdateHostRequestCommand: public ICCommunicationCommandBase
{
public:
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICUpdateHostTransferCommand: public ICCommunicationCommandBase
{
public:
    QByteArray DataBuffer() const { return dataBuffer_;}
    void SetDataBuffer(const QByteArray& data) { dataBuffer_ = data;}

    QByteArray SentBuffer() const { return sentBuffer_;}
    QByteArray RecvBuffer() const { return recvBuffer_;}
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);

private:
    QByteArray dataBuffer_;
    QByteArray sentBuffer_;
    QByteArray recvBuffer_;
};

class ICUpdateHostFinishCommand: public ICCommunicationCommandBase
{
public:
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICUpdateHostRestartCommand: public ICCommunicationCommandBase
{
public:
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICUpdateHostQueryCommand: public ICCommunicationCommandBase
{
public:
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICUpdateHostStart: public ICCommunicationCommandBase
{
public:
protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

class ICManualRun: public ICCommunicationCommandBase
{
public:
    int StepNum() { return FlagValue("StepNum");}
    void SetStepNum(int num) { SetFlagValue("StepNum", num);}

    int GM() const { return FlagValue("GM");}
    void SetGM(int value) { SetFlagValue("GM", value);}

    int Point() const { return FlagValue("Point");}
    void SetPoint(int value) { SetFlagValue("Point", value);}

    int Pos() const { return FlagValue("Pos");}
    void SetPos(int value) { SetFlagValue("Pos", value);}

    int IFVal() const { return FlagValue("IFVal");}
    void SetIFVal(int value) { SetFlagValue("IFVal", value);}

protected:
    virtual QVariant Send(modbus_param_t *modbusParam);
};

Q_DECLARE_METATYPE(ICTeachStepCommand::ActionItem);

#endif // ICCOMMANDS_H
