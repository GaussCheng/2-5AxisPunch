#ifndef CONTAINERINIT_H
#define CONTAINERINIT_H

#include <QMap>
#include <QMultiMap>
#include "ICCustomWidgets_global.h"
template <typename T, typename U>
class ICCUSTOMWIDGETSSHARED_EXPORT CreateMap
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
class ICCUSTOMWIDGETSSHARED_EXPORT CreateMultiMap
{
public:
    ~CreateMultiMap(){map_.clear();}
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
