#ifndef ICNWMPROTOCOL_H
#define ICNWMPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QMap>
#include <QStringList>

class ICNWMProtocol
{
public:
    ICNWMProtocol();

    static QByteArray DevConnectedPackage(const QString& ip, const QString& uuid);
    static QByteArray RefreshStatusPackage(const QMap<int, int> status);
    static QByteArray PostMoldActPackage(const QString& name, const QString& act);
    static QByteArray PostMoldFncPackage(const QString& name, const QString& fnc);
    static QByteArray PostMoldSubsPackage(const QString& name, const QStringList& subs);
    static QByteArray PostAxisConfigPackage(const QString& name, const QString& configs);
    static QByteArray PostSystemConfigPackage(const QString& name, const QString& configs);
    static QByteArray PostAxisDRPackage(const QString& name, const QString& configs);
    static QByteArray PostPanelConfigPackage(const QString& name, const QString& configs);

};

#endif // ICNWMPROTOCOL_H
