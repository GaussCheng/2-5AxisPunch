#include "icalarmsystem.h"
#include <QDateTime>

ICAlarmSystem::ICAlarmSystem(uint maxRecord):maxRecords_(maxRecord),model_(NULL)
{
}

QStringList ICAlarmSystem::AlarmNumsToStringList(const QBitArray &bits) const
{
    QStringList ret;
    for(int i = 0; i != bits.size(); ++i)
    {
        if(unlikely(bits.testBit(i)))
        {
            ret.append(AlarmNumToString(i));
        }
    }
    return ret;
}


int ICAlarmSystem::FindIDRowInModel_(uint id)
{
    if(unlikely(model_ == NULL))
    {
        return -1;
    }
    const int idIndex = IDColumnIndex();
    int left = 0;
    int right = model_->rowCount() - 1;
    int mid;
    do
    {
        mid = ((right - left) >> 1) + left;
        if(id < model_->record(mid).value(idIndex).toUInt())
        {
            left = mid;
        }
        else if(id > model_->record(mid).value(idIndex).toUInt())
        {
            right = mid;
        }
        else
        {
            return mid;
        }
    }while(left != right);
    return - 1;
}
