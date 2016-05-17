#include "icnwm.h"
#include "icnwmprotocol.h"
#include <QFile>
#include "icmold.h"
#include "icmacrosubroutine.h"
#include "icparameterssave.h"
#include "parser.h"


ICNWMPTR ICNWM::instance_ = NULL;
ICNWM::ICNWM()
{
//    connect(&socket_, SIGNAL(connected()),SLOT(OnConnectedHost()));
    isConnected_ = false;

    connect(&server_, SIGNAL(newConnection()), SLOT(OnClientConnected()));
    connect(&signalMapper_, SIGNAL(mapped(QObject*)), SLOT(OnClientRequest(QObject*)));

}

inline QByteArray ReadFileHelper(const QString& file)
{
    QFile f(file);
    QByteArray ret;
    if(f.open(QFile::ReadOnly | QFile::Text))
    {
        ret = f.readAll();
        f.close();
    }
    return ret;
}

void ICNWM::OnConnectedHost(QTcpSocket *socket)
{
    QByteArray content = ReadFileHelper("sysconfig/Multi-axisManipulatorSystem.ini");
    PostPanelConfig(socket, "Multi-axisManipulatorSystem.ini", content);
    content = ReadFileHelper("sysconfig/system.txt");
    PostSystemConfig(socket, "system.txt", content);
    content = ReadFileHelper("sysconfig/DistanceRotation");
    PostAxisDR(socket, "DistanceRotation", content);
    socket->flush();
    content = ReadFileHelper("sysconfig/paramx.txt");
    PostAxisConfig(socket, "paramx.txt", content);
    content = ReadFileHelper("sysconfig/paramy.txt");
    PostAxisConfig(socket, "paramy.txt", content);
    content = ReadFileHelper("sysconfig/paramz.txt");
    PostAxisConfig(socket, "paramz.txt", content);
    content = ReadFileHelper("sysconfig/paramp.txt");
    PostAxisConfig(socket, "paramp.txt", content);
    content = ReadFileHelper("sysconfig/paramq.txt");
    PostAxisConfig(socket, "paramq.txt", content);
    content = ReadFileHelper("sysconfig/parama.txt");
    PostAxisConfig(socket, "parama.txt", content);
    content = ReadFileHelper("sysconfig/paramb.txt");
    PostAxisConfig(socket, "paramb.txt", content);
    content = ReadFileHelper("sysconfig/paramc.txt");
    PostAxisConfig(socket, "paramc.txt", content);
    socket->flush();
    QString moldName = ICParametersSave::Instance()->MoldName("Default");
    moldName.chop(4);
    PostMoldAct(socket, moldName + ".act", ICMold::CurrentMold()->RawMoldContent());
    PostMoldFnc(socket, moldName + ".fnc", ICMold::CurrentMold()->RawMoldFnc());
    PostMoldSubs(socket, moldName, ICMacroSubroutine::Instance()->RawSubs());
//    socket_.write(ICNWMProtocol::DevConnectedPackage(socket_.localAddress().toString(), uuid_.toString()));
//    socket_.disconnectFromHost();
//    socket_.close();
    isConnected_ = true;
    emit DevConnected();
}

void ICNWM::SendStatusToHost(QTcpSocket* socket, const QMap<int, int> status)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::RefreshStatusPackage(status));
}

void ICNWM::SendStatusToHost(QTcpSocket* socket, int status, int value)
{
    QMap<int, int> m;
    m.insert(status, value);
    SendStatusToHost(socket, m);
}

void ICNWM::PostMoldAct(QTcpSocket* socket, const QString &name, const QString &act)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostMoldActPackage(name, act));
}

void ICNWM::PostMoldFnc(QTcpSocket* socket, const QString &name, const QString &fnc)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostMoldFncPackage(name, fnc));
}

void ICNWM::PostMoldSubs(QTcpSocket* socket, const QString &name, const QStringList &subs)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostMoldSubsPackage(name, subs));
}

void ICNWM::PostAxisConfig(QTcpSocket* socket, const QString &name, const QString &configs)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostAxisConfigPackage(name, configs));
}

void ICNWM::PostAxisDR(QTcpSocket* socket, const QString &name, const QString &configs)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostAxisDRPackage(name, configs));
}

void ICNWM::PostSystemConfig(QTcpSocket* socket, const QString &name, const QString &configs)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostSystemConfigPackage(name, configs));
}

void ICNWM::PostPanelConfig(QTcpSocket *socket, const QString &name, const QString &configs)
{
    if(socket->isOpen())
        socket->write(ICNWMProtocol::PostPanelConfigPackage(name, configs));
}


void ICNWM::ServerUp()
{
    if(!server_.isListening())
        server_.listen(QHostAddress::Any, 8888);
}

void ICNWM::OnClientConnected()
{
    QTcpSocket* socket = server_.nextPendingConnection();
    signalMapper_.setMapping(socket, socket);
    connect(socket, SIGNAL(readyRead()), &signalMapper_, SLOT(map()));
}

void ICNWM::OnClientRequest(QObject *socket)
{
    QTcpSocket* so = qobject_cast<QTcpSocket*>(socket);
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(so->readAll(), &ok).toMap();
    if(ok)
    {
        if(result.contains("cmd"))
        {
            QString cmd = result.value("cmd").toString();
            if(cmd == "Init")
            {
                OnConnectedHost(so);
            }
        }
    }
}
