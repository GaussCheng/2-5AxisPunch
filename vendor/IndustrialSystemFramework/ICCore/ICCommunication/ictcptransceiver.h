#ifndef ICTCPTRANSCEIVER_H
#define ICTCPTRANSCEIVER_H

#include "ictransceiverbase.h"
#include <QObject>
#include "ICCore_global.h"
class QTcpSocket;
class QTcpServer;

class ICCORESHARED_EXPORT _ConnectionHelper: public QObject
{
    Q_OBJECT
public:
    _ConnectionHelper();
    ~_ConnectionHelper();
    QTcpSocket* tcpSocket_;
    QTcpServer* tcpServer_;

private slots:
    void OnNewConnection();
};

class ICCORESHARED_EXPORT ICTcpTransceiver : public ICTransceiverBase
{
public:
    ICTcpTransceiver();
    virtual ~ICTcpTransceiver();

    bool IsBlock() { return isBlock_;}
    void SetBlock(bool isBlock) {isBlock_ = isBlock;}
    virtual void StopCommunicate();
    virtual void StartCommunicate();
protected:

    virtual int ReadImpl(uint8_t *dest, size_t size);
    virtual int WriteImpl(const uint8_t *buffer, size_t size);
private:
    _ConnectionHelper*  connHelper_;
    bool isBlock_;
};
#endif // ICTCPTRANSCEIVER_H
