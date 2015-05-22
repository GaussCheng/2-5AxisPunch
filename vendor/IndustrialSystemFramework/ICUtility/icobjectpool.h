#ifndef ICOBJECTPOOL_H
#define ICOBJECTPOOL_H

#include "ICUtility_global.h"
#include <malloc.h>
#include <QList>

template <class T>
class ICUTILITYSHARED_EXPORT ICObjectPool
{
public:
    ~ICObjectPool()
    {

        for(int i = 0; i != dataList_.size(); ++i)
        {
            free(dataList_[i]);
        }
    }

    void* MallocData()
    {
        if(!dataList_.isEmpty())
        {
            QList<void *>::iterator it = dataList_.begin();
            void *p = *it;
            dataList_.pop_front();
            return p;
        }
        else
        {
            void *p = malloc(sizeof(T));
            return p;
        }
    }

    void FreeData(void *p)
    {
        dataList_.push_back(p);
    }

private:
    QList<void *> dataList_;
};

#endif // ICOBJECTPOOL_H
