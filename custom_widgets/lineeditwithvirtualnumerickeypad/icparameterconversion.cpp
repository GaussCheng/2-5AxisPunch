#include "icparameterconversion.h"

#include <QStringList>
#include <QDebug>

ICParameterConversion::ICParameterConversion()
{
}

QString ICParameterConversion::TransThisIntToThisText(int inputNum, int decimals)
{
//    QString result;
//    switch(decimals)
//    {
//    case 0:
//        result = QString::number(inputNum);
//        break;
//    default:
//        {
//            int dividend = Pow(10, decimals);
//            int intPart = inputNum / dividend;
//            int decimalPart = inputNum % dividend;
//            result = QString::number(intPart) + '.' + QString::number(decimalPart);
//        }
//        break;
//    }
    if(decimals == 0)
    {
        return QString::number(inputNum);
    }
    else
    {
        QString format = QString("%.%1f").arg(decimals);
        return QString().sprintf(format.toAscii(), inputNum / static_cast<qreal>(Pow(10, decimals)));

    }
}

int ICParameterConversion::TransTextToThisInt(const QString &numString, int decimals)
{
    int result = -1;
        switch(decimals)
        {
        case 0:
            bool ok;
            result = numString.toInt(&ok);
            if(!ok)
            {
                return -1;
            }
            break;
        default:
            {
            bool ok;
            double tmp = numString.toDouble(&ok);
            if(!ok)
            {
                return -1;
            }
            int powerResult = Pow(10 ,decimals);
            result = tmp * powerResult;
    //        return result;
    //            bool ok;
    //            QStringList numList = numString.split('.');
    //            result = numList.at(0).toInt(&ok) * powerResult;
    //            if(!ok)
    //            {
    //                return -1;
    //            }
    //            if(numList.count() == 2)
    //                result += numList.at(1).toInt();
            }
        }

        return result;
}

int ICParameterConversion::Pow(int variable, int power)
{
    int result = 1;
    for(int i = 0; i < power; ++i)
    {
        result *= variable;
    }
    return result;
}
