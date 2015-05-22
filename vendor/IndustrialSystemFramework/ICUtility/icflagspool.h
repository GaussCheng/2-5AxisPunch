#ifndef ICFLAGSPOOL_H
#define ICFLAGSPOOL_H

#include <QMap>
#include <QMultiMap>
#include <QSharedPointer>
#include <QVariant>
#include "ICUtility_global.h"

class ICUTILITYSHARED_EXPORT ICAbstractFlagObserver
{
public:
    virtual void Update(int flag, QVariant val) = 0;
};

class ICUTILITYSHARED_EXPORT ICFlagsPool
{
public:
    typedef QSharedPointer<ICFlagsPool> ICFlagsPoolPTR;
    typedef QList<ICAbstractFlagObserver*> ICFlagObserverList;
    static ICFlagsPoolPTR GlobalInstance()
    {
        if(instanse_.isNull())
        {
            instanse_ = ICFlagsPoolPTR(new ICFlagsPool());
        }
        return instanse_;
    }
    ~ICFlagsPool(){flagsValueMap_.clear();flagsObserversMap_.clear();}

    bool RegisterFlag(int flag, QVariant init = QVariant());
    bool Attach(int flag, ICAbstractFlagObserver* observer);
    bool Detach(int flag, ICAbstractFlagObserver* observer);
    void SetFlagValue(int flag, QVariant val);
    QVariant GetFlagValue(int flag, bool *isok = NULL) const;

private:
    QMap<int, QVariant> flagsValueMap_;
    QMultiMap<int, ICAbstractFlagObserver*> flagsObserversMap_;

    static ICFlagsPoolPTR instanse_;
    ICFlagsPool();
};

inline bool ICFlagsPool::RegisterFlag(int flag, QVariant init)
{
    if(flagsValueMap_.contains(flag))
    {
        return false;
    }
    flagsValueMap_.insert(flag, init);
    return true;
}

inline bool ICFlagsPool::Attach(int flag, ICAbstractFlagObserver *observer)
{
    if(flagsValueMap_.contains(flag))
    {
        flagsObserversMap_.insert(flag, observer);
        return true;
    }
    return false;
}

inline bool ICFlagsPool::Detach(int flag, ICAbstractFlagObserver *observer)
{
    if(flagsValueMap_.contains(flag) && flagsObserversMap_.contains(flag, observer))
    {
        flagsObserversMap_.remove(flag, observer);
        return true;
    }
    return false;
}

Q_DECLARE_METATYPE(QList<int>)

#endif // ICFLAGSPOOL_H
