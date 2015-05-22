#ifndef ICCYCLEARRAY_H
#define ICCYCLEARRAY_H

#include "ICUtility_global.h"

#include <QVarLengthArray>
#include <QDebug>

template<typename T, int capacity>
class ICUTILITYSHARED_EXPORT ICCycleArray
{
public:
    ICCycleArray():capacity_(capacity),begin_(0), end_(0){}
    int Size() const { return datas_.size();}
    int Capacity() const { return capacity_;}

    void Add(T val);
    T At(int index) const { return datas_[(begin_ + index) % datas_.size()];}

    T* RawData(int begin = 0, int size = -1);

    void Clear() { datas_.clear(); begin_ = 0; end_ = 0;}

private:
    int capacity_;
    int begin_;
    int end_;

    QVarLengthArray<T, capacity> datas_;
    QVarLengthArray<T, capacity> rawDatas_;
};

template<typename T, int size>
void ICCycleArray<T, size>::Add(T val)
{
    if(datas_.size() == Capacity())
    {
        end_ = begin_;
        begin_ = (begin_ + 1) % Capacity();
        datas_[end_] = val;
    }
    else
    {
        datas_.append(val);
        ++end_;
    }
}

template<typename T, int capacity>
T* ICCycleArray<T, capacity>::RawData(int begin, int size)
{
    if(size < 0 || (begin + size) > datas_.size() )
    {
        size = datas_.size();
    }
    int actualBegin = begin_ + begin;
    int dataPos;
    if(actualBegin > datas_.size())
    {
        dataPos = (begin_ + begin) % datas_.size();
        return datas_.data() + dataPos;
    }
    dataPos = actualBegin + size;
    if(dataPos < datas_.size())
    {
        return datas_.data() + actualBegin;
    }
    rawDatas_.clear();
    rawDatas_.append(datas_.data() + actualBegin, datas_.size() - actualBegin);
    rawDatas_.append(datas_.data(), size - (datas_.size() - actualBegin));
    return rawDatas_.data();
}

#endif // ICCYCLEARRAY_H
