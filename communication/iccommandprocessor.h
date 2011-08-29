#ifndef ICCOMMANDPROCESSOR_H
#define ICCOMMANDPROCESSOR_H

#include <QVariant>
#include <QScopedPointer>
#include <QList>
#include <QLabel>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

#include "iccommunicationcommandbase.h"
#include "iccommands.h"


class ICCommandProcessor
{
public:
    static ICCommandProcessor* Instance()
    {
        if(instance_.isNull())
        {
            instance_.reset(new ICCommandProcessor());
        }
        return instance_.data();
    }

    int SlaveID() const { return slaveID_;}
    void SetSlaveID(int slave) { slaveID_ = slave;}

    bool SetTransmitSpecification(const QString &device,
                                  int baundRate,
                                  const QString &parity,
                                  int dataBit, int stopBit);
    QVariant ExecuteCommand(ICCommunicationCommandBase &command)
    {
        QMutexLocker locker(&mutex_);
        return command.SendCommand(modbusParam_.data());
//        return QVariant();
    }

    bool ExecuteHCCommand(int cmd, int keyValue, uint act = 0, uint sum = 0);

    bool ExecuteVirtualKeyCommand(int keyValue) { return ExecuteHCCommand(0x40, keyValue);}

    bool ModifySysParam(int addr, int value);
    bool ModifyMoldParam(int addr, int value) { return ModifySysParam(addr, value);}

    bool TeachParam(int addr, int value);
    bool DoseNeedToGetTeachParam();
    bool IsAlarmed();

    QList<QList<uint> > TeachStep();


private:
    static QScopedPointer<ICCommandProcessor> instance_;
    int slaveID_;

    struct ModbusParamDeleter
     {
         static inline void cleanup(modbus_param_t *modbusParam)
         {
             if(modbusParam != NULL)
             {
                 modbus_close(modbusParam);
                 delete modbusParam;
                 modbusParam = NULL;
             }
         }
     };

    QScopedPointer<modbus_param_t, ModbusParamDeleter> modbusParam_;
    QScopedPointer<ICExectueCommand> executeCommand_;
    QScopedPointer<ICModifySysParamCommand> modifySysParamCommand_;
    QScopedPointer<ICTeachParamCommand> teachParamCommand_;
    QScopedPointer<ICGetErrorCommand> errCommand_;
    QScopedPointer<ICQueryStatus> queryStatus_;
    QScopedPointer<ICTeachStepCommand> teachStepCommand_;
    QMutex mutex_;
    ICCommandProcessor();
};

inline bool ICCommandProcessor::ExecuteHCCommand(int cmd, int keyValue, uint act, uint sum)
{
    executeCommand_->SetSlave(SlaveID());
    executeCommand_->SetCommandCode(cmd);
    executeCommand_->SetKeyValue(keyValue);
    executeCommand_->SetSyncAct(act);
    executeCommand_->SetSycnSum(sum);
    return ExecuteCommand(*(executeCommand_.data())).toBool();
}

inline bool ICCommandProcessor::ModifySysParam(int addr, int value)
{
    modifySysParamCommand_->SetSlave(SlaveID());
    modifySysParamCommand_->SetStartAddr(addr);
    modifySysParamCommand_->SetParamValue(value);
    return ExecuteCommand(*(modifySysParamCommand_.data())).toBool();
}

inline bool ICCommandProcessor::TeachParam(int addr, int value)
{
    teachParamCommand_->SetSlave(SlaveID());
    teachParamCommand_->SetParamValue(value);
    teachParamCommand_->SetStartAddr(addr);
    return ExecuteCommand(*(teachParamCommand_.data())).toBool();
}

inline bool ICCommandProcessor::DoseNeedToGetTeachParam()
{
    errCommand_->SetSlave(SlaveID());
    return ExecuteCommand(*(errCommand_.data())).toBool();
}

inline bool ICCommandProcessor::IsAlarmed()
{
    queryStatus_->SetSlave(SlaveID());
    queryStatus_->SetStartAddr(4);
    queryStatus_->SetNumber(4);
    return ExecuteCommand(*(queryStatus_.data())).value<ICCommunicationCommandBase::ResultVector>().at(1) != 0;
}

inline QList<QList<uint> >ICCommandProcessor::TeachStep()
{
    teachStepCommand_->SetSlave(SlaveID());
    return ExecuteCommand(*(teachStepCommand_.data())).value<ICTeachStepCommand::ActionItem>();
}

#endif // ICCOMMANDPROCESSOR_H
