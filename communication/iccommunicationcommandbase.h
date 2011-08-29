#ifndef ICCOMMUNICATIONCOMMANDBASE_H
#define ICCOMMUNICATIONCOMMANDBASE_H

#include <QVariant>
#include <QString>
#include <QRegExp>
#include <QMap>
#include <QStringList>
#include <QVector>
#include <QMetaType>
#include "modbus.h"

class ICCommunicationCommandBase
{
public:
    typedef QVector<int> ResultVector;

    ICCommunicationCommandBase();

    QVariant SendCommand(modbus_param_t *modbusParam) { return Send(modbusParam);}

    QString CommandArgs() const { return args_;}
    void SetCommandArgs(const QString &args);

    bool IsError() const { return isSendError_;}

    int Slave() const { return flagValueMap_.value("slave", -1);}
    void SetSlave(int slave) { flagValueMap_.insert("slave", slave);}

    int StartAddr() const { return flagValueMap_.value("startaddr", -1);}
    void SetStartAddr(int addr) { flagValueMap_.insert("startaddr", addr);}

    int Number() const{ return flagValueMap_.value("number", -1);}
    void SetNumber(int number) { flagValueMap_.insert("number", number);}

    int FlagValue(const QString& flag) const { return flagValueMap_.value(flag.toLower(), -1);}
    void SetFlagValue(const QString& flag, int value) { flagValueMap_.insert(flag.toLower(), value);}

    int RetryTimes() const { return tryTimes_;}
    void SetRetryTimes(int times) { tryTimes_ = times; }

protected:
    virtual QVariant Send(modbus_param_t *modbusParam) = 0;

protected:
    void SetSendError(bool statu) { isSendError_ = statu;}
    QRegExp BuildArgRegExp(const QString &flag) const;
    void AddCommandArgFlag(const QString& name) { flagValueMap_.insert(name.toLower(), -1);}
    void AddCommandArgFlags(const QStringList& names);

private:
    QString args_;
    QMap<QString, int> flagValueMap_;
    bool isSendError_;
    int tryTimes_;
};
Q_DECLARE_METATYPE(ICCommunicationCommandBase::ResultVector)

#endif // ICCOMMUNICATIONCOMMANDBASE_H
