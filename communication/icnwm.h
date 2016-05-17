#ifndef ICNWM_H
#define ICNWM_H

#include <QScopedPointer>
#include <QHostAddress>
#include <QUuid>
#include <QTcpSocket>
#include <QObject>

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

    void DevConnect(ICUuid uuid, const QString& ip, quint16 port);
    bool IsConnected() const { return isConnected_; }

    void SendStatusToHost(const QMap<int , int> status);
    void SendStatusToHost(int status, int value);
    void PostMoldAct(const QString& name, const QString& act);
    void PostMoldFnc(const QString& name, const QString& fnc);
    void PostMoldSubs(const QString& name, const QStringList& subs);
    void PostAxisConfig(const QString& name, const QString& configs);
    void PostAxisDR(const QString& name, const QString& configs);
    void PostSystemConfig(const QString& name, const QString& configs);
    void PostPanelConfig(const QString& name, const QString& configs);

signals:
    void DevConnected();
private slots:
    void OnConnectedHost();

private:
    ICNWM();
    static ICNWMPTR instance_;
    QTcpSocket socket_;
    ICUuid uuid_;
    bool isConnected_;
//    QString ip_;
//    QString port_;
//    ICServerAddress serverAddr_;

};

#endif // ICNWM_H
