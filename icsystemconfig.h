#ifndef ICSYSTEMCONFIG_H
#define ICSYSTEMCONFIG_H

#include <QObject>

class QSettings;

class ICSerialPortConfig
{
public:
    QString DeviceName() const { return deviceName_;}
    void SetDevice(const QString &device) { deviceName_ = device;}
    int BaudRate() const { return baudRate_;}
    void SetBaudRate(int baudRate) { baudRate_ = baudRate;}
    QString Parity() const { return parity_;}
    void SetParity(const QString & parity) { parity_ = parity;}
    int DataBit() const { return dataBit_;}
    void SetDataBit(int dataBit) { dataBit_ = dataBit;}
    int StopBit() const { return stopBit_;}
    void SetStopBit(int stopBit) { stopBit_ = stopBit;}
private:
    QString deviceName_;
    int baudRate_;
    QString parity_;
    int dataBit_;
    int stopBit_;
};

class ICSystemConfig : public QObject
{
    Q_OBJECT
public:
    explicit ICSystemConfig(QObject *parent = 0);

    ICSerialPortConfig SerialPortConfig() const;
    void SetSerialPortConfig(ICSerialPortConfig config);

    QString MoldName() const;
    void SetMoldName(const QString &name);

signals:

public slots:

private:
    const QString CommunicationConfigGroup;
    const QString MoldNameConfigGroup;
    mutable QSettings *sysSetting_;

};

#endif // ICSYSTEMCONFIG_H
