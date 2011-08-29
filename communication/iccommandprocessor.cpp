#include "iccommandprocessor.h"
#include "icsystemconfig.h"

#include <QDebug>

QScopedPointer<ICCommandProcessor> ICCommandProcessor::instance_;
ICCommandProcessor::ICCommandProcessor()
    :modbusParam_(new modbus_param_t()),
    executeCommand_(new ICExectueCommand()),
    modifySysParamCommand_(new ICModifySysParamCommand()),
    teachParamCommand_(new ICTeachParamCommand()),
    errCommand_(new ICGetErrorCommand()),
    queryStatus_(new ICQueryStatus()),
    teachStepCommand_(new ICTeachStepCommand())
{
    SetSlaveID(1);
    ICSystemConfig systemConfig;
    ICSerialPortConfig serialConfig = systemConfig.SerialPortConfig();
    SetTransmitSpecification(serialConfig.DeviceName(),
                             serialConfig.BaudRate(),
                             serialConfig.Parity(),
                             serialConfig.DataBit(),
                             serialConfig.StopBit());

//    testCount = 0;
}

bool ICCommandProcessor::SetTransmitSpecification(const QString &device,
                                                  int baundRate,
                                                  const QString &parity,
                                                  int dataBit, int stopBit)
{
    if(!modbusParam_.isNull())
    {
        modbus_close(modbusParam_.data());
    }
    else
    {
        modbusParam_.reset(new modbus_param_t());
    }
    modbus_init_rtu(modbusParam_.data(),
                    device.toAscii(),
                    baundRate,
                    parity.toAscii(),
                    dataBit,
                    stopBit);
//    modbusParam_->debug = true;
    return modbus_connect(modbusParam_.data()) != -1;
}

