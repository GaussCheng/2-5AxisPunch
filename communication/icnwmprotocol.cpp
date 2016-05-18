#include "icnwmprotocol.h"

static const QString DEV_CONNECTED_TEMPLATE = "{\"commandType\":\"DevConnected\",\"devUUID\":\"%1\",\"ipAddr\":\"%2\"}";
static const QString REFRESH_STATUS_TEMPLATE = "{\"commandType\":\"RefreshStatus\",\"status\":{%1}}";
static const QString POST_MOLD_ACT_TEMPLATE = "{\"commandType\":\"PostMoldAct\",\"name\":\"%1\", \"act\":\"%2\"}";
static const QString POST_MOLD_FNC_TEMPLATE = "{\"commandType\":\"PostMoldFnc\",\"name\":\"%1\", \"fnc\":\"%2\"}";
static const QString POST_MOLD_SUBS_TEMPLATE = "{\"commandType\":\"PostMoldSubs\",\"name\":\"%1\", %2}";
static const QString POST_AXIS_CONFIG_TEMPLATE = "{\"commandType\":\"PostAxisConfig\",\"name\":\"%1\", \"configs\":\"%2\"}";
static const QString POST_AXIS_DR_TEMPLATE = "{\"commandType\":\"PostAxisDR\",\"name\":\"%1\", \"configs\":\"%2\"}";
static const QString POST_SYSTEM_CONFIG_TEMPLATE = "{\"commandType\":\"PostSystemConfig\",\"name\":\"%1\", \"configs\":\"%2\"}";
static const QString POST_PANEL_CONFIG_TEMPLATE = "{\"commandType\":\"PostPanelConfig\",\"name\":\"%1\", \"configs\":\"%2\"}";


ICNWMProtocol::ICNWMProtocol()
{
}

QByteArray ICNWMProtocol::DevConnectedPackage(const QString &ip, const QString &uuid)
{
    QByteArray ret = QString(DEV_CONNECTED_TEMPLATE).arg(uuid).arg(ip).toLatin1();
    return ret;
}

QByteArray ICNWMProtocol::RefreshStatusPackage(const QMap<int, int> status)
{
    QString sm;
    QMap<int, int>::const_iterator p = status.constBegin();
    while(p != status.constEnd())
    {
        sm += QString("\"%0\":%1,").arg(p.key()).arg(p.value());
        ++p;
    }
    sm.chop(1);
    QByteArray ret = QString(REFRESH_STATUS_TEMPLATE).arg(sm).toLatin1();
    return ret;
}

QByteArray ICNWMProtocol::PostMoldActPackage(const QString &name, const QString &act)
{
    QByteArray ret = QString(POST_MOLD_ACT_TEMPLATE).arg(name).arg(act).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostMoldFncPackage(const QString &name, const QString &fnc)
{
    QByteArray ret = QString(POST_MOLD_FNC_TEMPLATE).arg(name).arg(fnc).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostMoldSubsPackage(const QString &name, const QStringList &subs)
{
    QString sub;
    for(int i = 0 ; i != subs.size(); ++i)
    {
        sub += QString("\"sub%1\": \"%2\",").arg(i).arg(subs.at(i));
    }
    sub.chop(1);
    QByteArray ret = QString(POST_MOLD_SUBS_TEMPLATE).arg(name).arg(sub).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostAxisConfigPackage(const QString &name, const QString &configs)
{
    QByteArray ret = QString(POST_AXIS_CONFIG_TEMPLATE).arg(name).arg(configs).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostAxisDRPackage(const QString &name, const QString &configs)
{
    QByteArray ret = QString(POST_AXIS_DR_TEMPLATE).arg(name).arg(configs).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostSystemConfigPackage(const QString &name, const QString &configs)
{
    QByteArray ret = QString(POST_SYSTEM_CONFIG_TEMPLATE).arg(name).arg(configs).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::PostPanelConfigPackage(const QString &name, const QString &configs)
{
    QByteArray ret = QString(POST_PANEL_CONFIG_TEMPLATE).arg(name).arg(configs).toUtf8();
    return ret;
}

QByteArray ICNWMProtocol::RequestInitPackage()
{
    return  "{\"cmd\":\"Init\"}";
}
