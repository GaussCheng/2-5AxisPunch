#include "icnwm.h"
#include "icnwmprotocol.h"
#include <QFile>
#include "icmold.h"
#include "icmacrosubroutine.h"
#include "icparameterssave.h"


ICNWMPTR ICNWM::instance_ = NULL;
ICNWM::ICNWM()
{
    connect(&socket_, SIGNAL(connected()),SLOT(OnConnectedHost()));
    isConnected_ = false;
}

void ICNWM::DevConnect(ICUuid uuid, const QString &ip, quint16 port)
{
    socket_.close();
    socket_.connectToHost(ip, port);
    uuid_ = uuid;
//    ip_ = ip;
//    port_ = port;
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

void ICNWM::OnConnectedHost()
{
    QByteArray content = ReadFileHelper("sysconfig/Multi-axisManipulatorSystem.ini");
    PostPanelConfig("Multi-axisManipulatorSystem.ini", content);
    content = ReadFileHelper("sysconfig/system.txt");
    PostSystemConfig("system.txt", content);
    content = ReadFileHelper("sysconfig/DistanceRotation");
    PostAxisDR("DistanceRotation", content);
    socket_.flush();
    content = ReadFileHelper("sysconfig/paramx.txt");
    PostAxisConfig("paramx.txt", content);
    content = ReadFileHelper("sysconfig/paramy.txt");
    PostAxisConfig("paramy.txt", content);
    content = ReadFileHelper("sysconfig/paramz.txt");
    PostAxisConfig("paramz.txt", content);
    content = ReadFileHelper("sysconfig/paramp.txt");
    PostAxisConfig("paramp.txt", content);
    content = ReadFileHelper("sysconfig/paramq.txt");
    PostAxisConfig("paramq.txt", content);
    content = ReadFileHelper("sysconfig/parama.txt");
    PostAxisConfig("parama.txt", content);
    content = ReadFileHelper("sysconfig/paramb.txt");
    PostAxisConfig("paramb.txt", content);
    content = ReadFileHelper("sysconfig/paramc.txt");
    PostAxisConfig("paramc.txt", content);
    socket_.flush();
    QString moldName = ICParametersSave::Instance()->MoldName("Default");
    moldName.chop(4);
    PostMoldAct(moldName + ".act", ICMold::CurrentMold()->RawMoldContent());
    PostMoldFnc(moldName + ".fnc", ICMold::CurrentMold()->RawMoldFnc());
    PostMoldSubs(moldName, ICMacroSubroutine::Instance()->RawSubs());
//    socket_.write(ICNWMProtocol::DevConnectedPackage(socket_.localAddress().toString(), uuid_.toString()));
//    socket_.disconnectFromHost();
//    socket_.close();
    isConnected_ = true;
    emit DevConnected();
}

void ICNWM::SendStatusToHost(const QMap<int, int> status)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::RefreshStatusPackage(status));
}

void ICNWM::SendStatusToHost(int status, int value)
{
    QMap<int, int> m;
    m.insert(status, value);
    SendStatusToHost(m);
}

void ICNWM::PostMoldAct(const QString &name, const QString &act)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostMoldActPackage(name, act));
}

void ICNWM::PostMoldFnc(const QString &name, const QString &fnc)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostMoldFncPackage(name, fnc));
}

void ICNWM::PostMoldSubs(const QString &name, const QStringList &subs)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostMoldSubsPackage(name, subs));
}

void ICNWM::PostAxisConfig(const QString &name, const QString &configs)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostAxisConfigPackage(name, configs));
}

void ICNWM::PostAxisDR(const QString &name, const QString &configs)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostAxisDRPackage(name, configs));
}

void ICNWM::PostSystemConfig(const QString &name, const QString &configs)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostSystemConfigPackage(name, configs));
}

void ICNWM::PostPanelConfig(const QString &name, const QString &configs)
{
    if(socket_.isOpen())
        socket_.write(ICNWMProtocol::PostPanelConfigPackage(name, configs));
}
