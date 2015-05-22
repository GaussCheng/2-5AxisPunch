#include "icparameterscache.h"

QHash<uint, quint32> ICParametersCache::configsCache_;
QHash<uint, QVariant> ICParametersCache::localStatusCache_;
QHash<uint, quint32> ICParametersCache::specialValueConfigsCache_;
QList<AddrValueChangeHandler> ICParametersCache::observes_;
ICParametersCache::ICParametersCache()
{
}


void ICParametersCache::SyncConfigsAddrValueCache()
{
    return;
//     QVector<QPair<quint32, quint32> > tmp = ICDALHelper::GetAllMoldConfig(ICAppSettings().CurrentMoldConfig()) +
//            ICDALHelper::GetAllSystemConfig(ICAppSettings().CurrentSystemConfig());
//    for(int i = 0; i != tmp.size(); ++i)
//    {
//        configsCache_.insert(tmp.at(i).first, tmp.at(i).second);
//    }
}

bool ICParametersCache::IsSpecialConfig(uint addr, quint32 value)
{
    if(!specialValueConfigsCache_.contains(addr))
    {
        return false;
    }
    return specialValueConfigsCache_.value(addr) == value;
}

void ICParametersCache::UpdateConfigValue(uint addr, quint32 value)
{
     configsCache_.insert(addr, value);
     for(int i = 0; i != observes_.size(); ++i)
     {
         observes_.at(i)(addr, value);
     }
     SetLocalStatus(kIsConfigsModified, true);
}

