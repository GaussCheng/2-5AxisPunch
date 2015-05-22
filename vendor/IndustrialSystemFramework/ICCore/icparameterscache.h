#ifndef ICPARAMETERSCACHE_H
#define ICPARAMETERSCACHE_H

#include <QHash>
#include <QVariant>
#include "icoptimize.h"
#include "icaddrwrapper.h"
#include "ICCore_global.h"


typedef void (*AddrValueChangeHandler) (uint addr, quint32 value);

class ICCORESHARED_EXPORT ICParametersCache
{
public:
    enum {
        kIsConfigsModified = 0,
        kIsReadAllStatus
    };
    ICParametersCache();
    static void SyncConfigsAddrValueCache();
    static void ClearConfigsAddrValueCache() {configsCache_.clear();}
    static quint32 OriginConfigValue(const ICAddrWrapper* addr);
    static quint32 ConfigValue(const ICAddrWrapper* addr);
    static void UpdateConfigValue(uint addr, quint32 value);

    static QVariant LocalStatus(uint addr) { return localStatusCache_.value(addr, 0);}
    static void SetLocalStatus(uint addr, QVariant value) { localStatusCache_.insert(addr, value);}
    static void AddSpecialConfig(uint addr, quint32 value) { specialValueConfigsCache_.insert(addr, value);}
    static bool IsSpecialConfig(uint addr, quint32 value);

    static void AttachAddrValueChangeHandler(AddrValueChangeHandler handler) { observes_.append(handler);}
    static void RemoveAddrValueChangeHandler(AddrValueChangeHandler handler) { observes_.removeOne(handler);}

private:
    static QHash<uint, quint32> configsCache_;
    static QHash<uint, QVariant> localStatusCache_;
    static QHash<uint, quint32> specialValueConfigsCache_;
    static QList<AddrValueChangeHandler> observes_;
};

inline quint32 ICParametersCache::OriginConfigValue(const ICAddrWrapper *addr)
{
    if(unlikely(addr == 0))
    {
        return 0;
    }
    uint baseAddr = addr->BaseAddr();
    if(likely(configsCache_.contains(baseAddr)))
    {
        return configsCache_.value(baseAddr);
    }
    return 0;
//    quint32 ret = ICDALHelper::GetConfigValue(addr);
//    configsCache_.insert(baseAddr, ret);
//    return ret;
}

inline quint32 ICParametersCache::ConfigValue(const ICAddrWrapper *addr)
{
    if(unlikely(addr == NULL)) return 0;
    quint32 ret = OriginConfigValue(addr);
    return ICAddrWrapper::ExtractValueByAddr(addr, ret);

}



#endif // ICPARAMETERSCACHE_H
