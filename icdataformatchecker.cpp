#include "icdataformatchecker.h"
#include <QStringList>
#include <QDebug>

ICDataFormatChecker::ICDataFormatChecker()
{
}

bool ICDataFormatChecker::CheckRowCount(const QString &dataStream, int rowCount, CompareWay way)
{
    QStringList dataList = dataStream.split("\n", QString::SkipEmptyParts);
    switch(way)
    {
    case kCompareLargger:
        return dataList.size() > rowCount;
    case kCompareSmaller:
        return dataList.size() < rowCount;
    case kCompareEqual:
        return dataList.size() == rowCount;
    case kCompareLarggerOrEqual:
        return dataList.size() >= rowCount;
    case kCompareSmallerOrEqual:
        return dataList.size() <= rowCount;
    default:
        return false;
    }
}

bool ICDataFormatChecker::CheckColumCount(const QString &dataStream, int row, const QString& spliter, int colCount, CompareWay way)
{
    QStringList dataList = dataStream.split("\n", QString::SkipEmptyParts);
    QStringList colList;
    if(row < 0)
    {
        if(dataList.isEmpty())
        {
            return false;
        }
        for(int i = 0; i != dataList.size(); ++i)
        {
            colList = dataList.at(i).split(spliter, QString::SkipEmptyParts);
            switch(way)
            {
            case kCompareLargger:
            {
                if(colList.size() <= colCount)
                {
                    return false;
                }
            }
                break;
            case kCompareSmaller:
            {
                if(colList.size() >= colCount)
                {
                    return false;
                }
            }
                break;
            case kCompareEqual:
            {
                if(colList.size() != colCount)
                {
                    return false;
                }
            }
                break;
            case kCompareLarggerOrEqual:
            {
                if(colList.size() < colCount)
                {
                    return false;
                }
            }
                break;
            case kCompareSmallerOrEqual:
            {
                if(colList.size() > colCount)
                {
                    return false;
                }
            }
                break;
            default:
                return false;
            }

        }//for(int i = 0; i != dataList.size(); ++i)
        return true;
    }
    else
    {
        colList = dataList.at(row).split(spliter, QString::SkipEmptyParts);
        switch(way)
        {
        case kCompareLargger:
            return colList.size() > colCount;
        case kCompareSmaller:
            return colList.size() < colCount;
        case kCompareEqual:
            return colList.size() == colCount;
        case kCompareLarggerOrEqual:
            return colList.size() >= colCount;
        case kCompareSmallerOrEqual:
            return colList.size() <= colCount;
        default:
            return false;
        }
    }
}

bool ICDataFormatChecker::CheckDataVal(const QString &dataStream, int row, int col, const QString &spliter, const QString &val)
{
    QStringList dataList = dataStream.split("\n", QString::SkipEmptyParts);
    if(row < 0)
    {
        row = dataList.size() - 1;
    }
    if(dataList.size() <= row)
    {
        return false;
    }
    QStringList colList = dataList.at(row).split(spliter, QString::SkipEmptyParts);
    if(col < 0)
    {
        col = colList.size() - 1;
    }
    if(colList.size() <= col)
    {
        return false;
    }
    return colList.at(col) == val;
}
