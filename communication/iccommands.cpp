#include "iccommands.h"
#include <QDebug>

QVariant ICGetErrorCommand::Send(modbus_param_t *modbusParam)
{
    QVariant result(false);
    int ret;

    int tryTimes = 0;
    do
    {
        ret = hc_does_need_teach_step(modbusParam,
                                      Slave(),
                                      0,
                                      4);
        ++tryTimes;
    }while(ret != -1 && ret != 1 && tryTimes < 3 );
    result.setValue(ret == 1);
    return result;

}

QVariant ICWriteParameters::Send(modbus_param_t *modbusParam)
{
    SetNumber(DataSection().size());
    if(Number() > 8)
    {
        return QVariant(false);
    }

    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_init_parameters(modbusParam,
                                 Slave(),
                                 StartAddr(),
                                 DataSection().size() / 2,
                                 DataSection().constData());
        tryedTimes++;
    }while(ret != Number() && tryedTimes < RetryTimes());
//    qDebug()<<"write para"<<ret<<Number();
    return ret == Number();

}

QVariant ICQueryStatus::Send(modbus_param_t *modbusParam)
{
    QVariant result;
    if(Number() > 4)
    {
        result.setValue(ResultVector());
        return result;
    }

//    int tryedTimes = 10;
    int ret;
    uint16_t dest[4];
//    do
//    {
        ret = hc_query_status(modbusParam,
                              Slave(),
                              StartAddr(),
                              Number(),
                              dest);
//        tryedTimes++;
//    }while(ret != Number() && ret != 0x17 && tryedTimes < RetryTimes());
    ResultVector queryResult;
//    needToGetTeach_ = false;
    needToReconfig_ = false;
    if(ret == Number())
    {
        for(int i = 0; i != ret; ++i)
        {
            queryResult.append(dest[i]);
        }
    }
    else if(ret == 0x16)
    {
        needToReconfig_ = true;
    }
//    else if(ret == 0x17)
//    {
//        needToGetTeach_ = true;
//    }
    result.setValue(queryResult);
    return result;
}

ICExectueCommand::ICExectueCommand()
{
    AddCommandArgFlags(QStringList()<<"CommandCode"<<"KeyValue"<<"SyncAct"<<"SyncSum");
}

QVariant ICExectueCommand::Send(modbus_param_t *modbusParam)
{
    QVariant result(false);
    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_command(modbusParam,
                         Slave(),
                         CommandCode(),
                         KeyValue(),
                         SyncAct(),
                         SyncSum());
        ++tryedTimes;
    }while(ret != 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        return result;
    }
    result.setValue(true);
    return result;
}

ICModifySysParamCommand::ICModifySysParamCommand()
{
    AddCommandArgFlag("ParamValue");
}

QVariant ICModifySysParamCommand::Send(modbus_param_t *modbusParam)
{
    QVariant result(false);
    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_modify_sys_parameter(modbusParam,
                                      Slave(),
                                      StartAddr(),
                                      ParamValue());
        ++tryedTimes;
    }while(ret != 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        return result;
    }
    result.setValue(true);
    return result;
}

ICTeachParamCommand::ICTeachParamCommand()
{
    AddCommandArgFlag("ParamValue");
}

QVariant ICTeachParamCommand::Send(modbus_param_t *modbusParam)
{
    QVariant result(false);
    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_teach_parameter(modbusParam,
                                 Slave(),
                                 StartAddr(),
                                 ParamValue());
        ++tryedTimes;
    }while(ret != 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        return result;
    }
    result.setValue(true);
    return result;
}

ICTeachStepCommand::ICTeachStepCommand()
{
    qRegisterMetaType<ActionItem>("ActionItem");
}

QVariant ICTeachStepCommand::Send(modbus_param_t *modbusParam)
{
    actionItem_.clear();
    int tryedTimes = 0;
    int stepCount = 0;
    int ret;
    int step = 0;
    QList<uint> oneItem;
    uint8_t dest[8];
    QVariant result;
    result.setValue(actionItem_);
    do
    {
        do
        {
            ret = hc_teach_step(modbusParam,
                                Slave(),
                                step,
                                dest);
            ++tryedTimes;
        }while(ret == -1 && tryedTimes < RetryTimes());
        if(ret == -1)
        {
            qDebug("ret == -1");
            actionItem_.clear();
            return result;
        }
//        int end = dest[8];
//        qDebug()<<"before"<<end;
//        if(end >= 20)
//        {
//            qDebug()<<"after = "<<end;
//            break;
//        }
        if(step % 2 == 0)
        {
            oneItem << ((dest[1] << 8) | dest[0])
                    <<dest[2]
                    <<dest[3]
                    <<((dest[5] << 8) | dest[4])
                    <<((dest[7] << 8) | dest[6]);
        }
        else
        {
            oneItem<<((dest[1] << 8) | dest[0])
                    <<dest[2]
                    <<((dest[5] << 8) | dest[4])
                    <<((dest[7] << 8) | dest[6])
                    <<dest[3];
            actionItem_.append(oneItem);
            oneItem.clear();
        }
        ++step;
        ++stepCount;
//        if(dest[0] > 20)
//        {
//            exit(-2);
//        }
    }while(stepCount <= 30);
    result.setValue(actionItem_);
    return result;
}

ICAutoAdjustCommand::ICAutoAdjustCommand()
{
    AddCommandArgFlags(QStringList()<<"DelayTime"
                       <<"Speed"
                       <<"Sequence"
                       <<"DPos"
                       <<"GMValue"
                       <<"CheckSum");
}

QVariant ICAutoAdjustCommand::Send(modbus_param_t *modbusParam)
{
    QVariant result(false);
    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_auto_adj(modbusParam,
                          Slave(),
                          Sequence(),
                          DelayTime(),
                          Speed(),
                          DPos(),
                          GMValue(),
                          CheckSum());
        ++tryedTimes;
    }while(ret != 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
//        qDebug("auto false");
        return result;
    }
//    qDebug("auto true");
    result.setValue(true);
    return result;
}

ICGetAxisConfigsCommand::ICGetAxisConfigsCommand()
{
    AddCommandArgFlag("Axis");
}

QVariant ICGetAxisConfigsCommand::Send(modbus_param_t *modbusParam)
{
    int tryedTimes = 0;
    int ret;
    int axis = Axis();
    axis = axis << 8;
    axis &= 0xFFFFFE;
    uint16_t dest0[4];
    uint16_t dest1[4];
    ResultVector result;
    QVariant vR;
    do
    {
        ret = hc_get_axis_parameter(modbusParam,
                                    Slave(),
                                    axis,
                                    dest0);
        if(ret != -1)
        {
            axis |= 1;
            ret = hc_get_axis_parameter(modbusParam,
                                        Slave(),
                                        axis,
                                        dest1);
        }
        ++tryedTimes;
    }while(ret < 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        vR.setValue(result);
        return vR;
    }
    result.append(dest0[0]);
    result.append(dest0[1]);
    result.append(dest0[2]);
    result.append(dest0[3]);
    result.append(dest1[0]);
    result.append(dest1[1]);
    result.append(dest1[2]);
    vR.setValue(result);
    return vR;
}

ICSetAxisConfigsCommand::ICSetAxisConfigsCommand()
{
    AddCommandArgFlags(QStringList()<<"Axis");
}

QVariant ICSetAxisConfigsCommand::Send(modbus_param_t *modbusParam)
{
    int tryedTimes = 0;
    int ret;
    uint16_t sendData[] = {dataBuffer_.at(0),
                           dataBuffer_.at(1),
                           dataBuffer_.at(2),
                           dataBuffer_.at(3)};
    uint16_t sendData1[] = {dataBuffer_.at(4),
                            dataBuffer_.at(5),
                            dataBuffer_.at(6),
                            0};
    do
    {
        ret = hc_set_axis_parameter(modbusParam,
                                    Slave(),
                                    0,
                                    Axis(),
                                    sendData);
//        qDebug()<<"ret="<<ret;
        if(ret == 1)
        {
            ret = hc_set_axis_parameter(modbusParam,
                                        Slave(),
                                        1,
                                        Axis(),
                                        sendData1);
        }
        ++tryedTimes;
    }while(ret < 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        return false;
    }
    return true;
}

ICSelecteConfigCommand::ICSelecteConfigCommand()
{
    AddCommandArgFlag("Selected");
}

QVariant ICSelecteConfigCommand::Send(modbus_param_t *modbusParam)
{
    int tryedTimes = 0;
    int ret;
    do
    {
        ret = hc_select_axis_config(modbusParam, Slave(), Selected());
        ++tryedTimes;
    }while(ret < 0 && tryedTimes < RetryTimes());
    if(ret < 0)
    {
        return false;
    }
    return true;
}
