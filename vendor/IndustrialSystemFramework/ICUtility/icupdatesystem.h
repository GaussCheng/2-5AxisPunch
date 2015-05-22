#ifndef ICUPDATESYSTEM_H
#define ICUPDATESYSTEM_H

#include "ICUtility_global.h"
#include <QStringList>
#include <QDir>

class ICUTILITYSHARED_EXPORT ICUpdateSystem
{
public:
    ICUpdateSystem();
    QStringList ScanUpdatePacks() const;
    bool StartUpdate(const QString& packName);
    QString PackProfile(const QString& packName);
    void SetUnpackCommand(const QString& cmd) {unpackCmd_ = cmd;}
    void SetPacksDir(const QString& path) {packPath_.setPath(path);}
    QString ScanPattern() const { return pattern_;}
    void SetScanPattern(const QString& pattern) { pattern_ = pattern;}

private:
    QString unpackCmd_;
    QDir packPath_;
    QString pattern_;
};

#endif // ICUPDATESYSTEM_H
