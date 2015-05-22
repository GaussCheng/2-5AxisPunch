#ifndef CONTAINERINIT_H
#define CONTAINERINIT_H


#include "ICUtility_global.h"
#include <QMap>
#include <QMultiMap>
template <typename T, typename U>
ICUTILITYSHARED_EXPORT class CreateMap
{
public:
    CreateMap(const T& key, const U& val)
    {
        map_.insert(key, val);
    }
    CreateMap<T, U>& operator()(const T& key, const U& val)
    {
        map_.insert(key, val);
        return *this;
    }
    operator QMap<T, U>()
    {
        return map_;
    }

private:
    QMap<T, U> map_;
};

template <typename T, typename U>
ICUTILITYSHARED_EXPORT class CreateMultiMap
{
public:
    CreateMultiMap(const T& key, const U& val)
    {
        map_.insert(key, val);
    }
    CreateMultiMap<T, U>& operator()(const T& key, const U& val)
    {
        map_.insert(key, val);
        return *this;
    }
    operator QMultiMap<T, U>()
    {
        return map_;
    }

private:
    QMultiMap<T, U> map_;
};

#endif // CONTAINERINIT_H
