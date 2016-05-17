#ifndef ICNWM_H
#define ICNWM_H

#include <QScopedPointer>
#include <QHostAddress>
#include <QUuid>
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>
#include <QSignalMapper>

class ICNWM;
typedef ICNWM* ICNWMPTR;
typedef QUuid ICUuid;

//struct ICServerAddress
//{
//    QHostAddress hostAddr;
//    quint16 port;
//};

class ICNWM: public QObject
{
    Q_OBJECT
public:
    static ICNWMPTR Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = (new ICNWM());
        }
        return instance_;
    }

    void ConnectToHost( const QString& ip, quint16 port);
    bool IsConnected() const { return isConnected_; }

    void SendStatusToHost(QTcpSocket* socket, const QMap<int , int> status);
    void SendStatusToHost(QTcpSocket* socket, int status, int value);
    void PostMoldAct(QTcpSocket* socket, const QString& name, const QString& act);
    void PostMoldFnc(QTcpSocket* socket, const QString& name, const QString& fnc);
    void PostMoldSubs(QTcpSocket* socket, const QString& name, const QStringList& subs);
    void PostAxisConfig(QTcpSocket* socket, const QString& name, const QString& configs);
    void PostAxisDR(QTcpSocket* socket, const QString& name, const QString& configs);
    void PostSystemConfig(QTcpSocket* socket, const QString& name, const QString& configs);
    void PostPanelConfig(QTcpSocket* socket, const QString& name, const QString& configs);

    void ServerUp();

signals:
    void DevConnected();
private slots:
    void OnConnectedHost(QTcpSocket* socket);
    void OnClientConnected();
    void OnClientRequest(QObject* socket);

private:
    ICNWM();
    static ICNWMPTR instance_;
//    QList<QTcpSocket*> clientList_;
    QTcpSocket socket_;
    QTcpServer server_;
    QSignalMapper signalMapper_;
    ICUuid uuid_;
    bool isConnected_;
//    QString ip_;
//    QString port_;
//    ICServerAddress serverAddr_;

};

#endif // ICNWM_H
