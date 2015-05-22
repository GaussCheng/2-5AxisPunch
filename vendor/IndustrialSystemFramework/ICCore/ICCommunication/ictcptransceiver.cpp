#include "ictcptransceiver.h"
#include <QTcpServer>
#include <QTcpSocket>

ICTcpTransceiver::ICTcpTransceiver()
{
    connHelper_ = new _ConnectionHelper();
    SetBlock(false);
}
ICTcpTransceiver::~ICTcpTransceiver()
{
    delete connHelper_;
}
int ICTcpTransceiver::ReadImpl(uint8_t *dest, size_t size)
{
    if(connHelper_->tcpSocket_ == NULL)
    {
        return 0;
    }
    if(IsBlock())
    {
        connHelper_->tcpSocket_->waitForReadyRead(-1);
    }
    if(!connHelper_->tcpSocket_->isReadable())
    {
        qDebug()<<connHelper_->tcpSocket_->errorString();
        return 0;
    }
    qDebug()<<"bytes available:"<<connHelper_->tcpSocket_->bytesAvailable();
    int ret = connHelper_->tcpSocket_->read((char *)dest,size) ;
//    qDebug()<<"ret:"<<ret;
    return ret;
}
int ICTcpTransceiver::WriteImpl(const uint8_t *buffer, size_t size)
{
    if(connHelper_->tcpSocket_ == NULL)
    {
        return 0;
    }
    return connHelper_->tcpSocket_->write((char *)buffer,size);
}
void ICTcpTransceiver::StopCommunicate()
{
    if(connHelper_->tcpSocket_ != NULL)
    {
        delete connHelper_->tcpSocket_;
        connHelper_->tcpSocket_ = NULL;
    }
    connHelper_->tcpServer_->close();
}

void ICTcpTransceiver::StartCommunicate()
{
    connHelper_->tcpServer_->listen(QHostAddress::Any, 9760);
}

_ConnectionHelper::_ConnectionHelper()
{
    tcpSocket_ = NULL;
    tcpServer_ = new QTcpServer();
    tcpServer_->listen(QHostAddress::Any, 9760);
    connect(tcpServer_,
            SIGNAL(newConnection()),
            SLOT(OnNewConnection()));
}

_ConnectionHelper::~_ConnectionHelper()
{
    tcpServer_->close();
    delete tcpServer_;
}

void _ConnectionHelper::OnNewConnection()
{
    qDebug("TCP Connect");
    if(tcpSocket_ != NULL)
    {
        tcpSocket_->close();
        delete tcpSocket_;
        tcpSocket_ = NULL;
    }
    tcpSocket_ = tcpServer_->nextPendingConnection();
    tcpSocket_->setSocketOption(QAbstractSocket::LowDelayOption, true);
}

