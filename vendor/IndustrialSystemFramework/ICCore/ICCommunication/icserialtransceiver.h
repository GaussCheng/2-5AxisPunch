#ifndef ICSERIALTRANSCEIVER_H
#define ICSERIALTRANSCEIVER_H

#include "ICCore_global.h"
#ifndef Q_WS_WIN
#include <unistd.h>
#include <termios.h>
#endif
#include <QScopedPointer>
#include "ictransceiverbase.h"

//class QextSerialPort;

class ICCORESHARED_EXPORT ICSerialTransceiver : public ICTransceiverBase
{
public:
    static ICSerialTransceiver* Instance()
    {
        return instance_.data();
    }
    bool IsBlock();
    void SetBlock(bool isBlock);
    virtual void StopCommunicate();
    virtual void StartCommunicate();
    virtual ~ICSerialTransceiver();
protected:
    virtual int ReadImpl(uint8_t *dest, size_t size);
    virtual int WriteImpl(const uint8_t *buffer, size_t size);
private:
    static QScopedPointer<ICSerialTransceiver> instance_;
    ICSerialTransceiver();
    int serialFD_;
    bool isError_;
#ifndef Q_WS_WIN
    termios oldTios_;
    fd_set fds_;
    timeval blockTime_;
#endif
//    QextSerialPort* serialPort_;

};

#endif // ICSERIALTRANSCEIVER_H
