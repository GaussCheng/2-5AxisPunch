#include "icflagspool.h"

ICFlagsPool::ICFlagsPoolPTR ICFlagsPool::instanse_;
ICFlagsPool::ICFlagsPool()
{
}

void ICFlagsPool::SetFlagValue(int flag, QVariant val)
{
    if(flagsValueMap_.contains(flag))
    {
        flagsValueMap_.insert(flag, val);
        ICFlagObserverList o = flagsObserversMap_.values(flag);
        for(int i = 0; i != o.size(); ++i)
        {
            o[i]->Update(flag, val);
        }
    }
}

QVariant ICFlagsPool::GetFlagValue(int flag, bool *isok) const
{
    if(flagsValueMap_.contains(flag))
    {
        if(isok != NULL)
        {
            *isok = true;
        }
        return flagsValueMap_.value(flag);
    }
    if(isok != NULL)
    {
        *isok = false;
    }
    return 0;
}
