#include <QLineEdit>
#include "iclineeditwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"
#include <qmath.h>

#include <QDebug>

QString IntToString(int num, int decimals)
{
    return QString::number(qreal(num) / qPow(10, decimals), 'f', decimals);
}

int StringToInt(const QString &numString, int decimals)
{
    double num = numString.toDouble();
    double diff = 5 * (num < 0 ? -1 : 1) / qPow(10, decimals + 1);
    num += diff;
    return num * qPow(10, decimals);
}

ICLineEditWrapper::ICLineEditWrapper(QLineEdit *edit,
                                     int addr,
                                     WrapperFor type,
                                     DisplayFormat format,
                                     QObject *parent) :
    QObject(parent),
    wrappedWidget_(edit),
    addr_(addr),
    type_(type),
    format_(format)
{
    if(ICMold::CurrentMold() == NULL)
    {
        return;
    }
    int initValue;
    if(type_ == Mold)
    {
        initValue = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr_));
    }
    else
    {
        initValue = ICVirtualHost::GlobalVirtualHost()->SystemParameter(static_cast<ICVirtualHost::ICSystemParameterAddr>(addr_)).toDouble();
    }
    if(format_ == Integer)
    {
        wrappedWidget_->setText(QString::number(initValue));
    }
    else
    {
//        QString fString = "%." + QString::number(static_cast<int>(format_)) + "f";
//        qreal base = 1;
//        for(int i = 0; i != format_; ++i)
//        {
//            base *= 10;
//        }
//        wrappedWidget_->setText(QString().sprintf(fString.toAscii(), initValue / base));
        wrappedWidget_->setText(IntToString(initValue, format));
    }
    connect(wrappedWidget_,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(EditFinished(QString)));
}

void ICLineEditWrapper::UpdateParam()
{

    disconnect(wrappedWidget_,
               SIGNAL(textChanged(QString)),
               this,
               SLOT(EditFinished(QString)));
    if(type_ == Mold)
    {
        int initValue = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr_));
        if(format_ == Integer)
        {
            wrappedWidget_->setText(QString::number(initValue));
        }
        else
        {
//            QString fString = "%." + QString::number(static_cast<int>(format_)) + "f";
//            qreal base = 1;
//            for(int i = 0; i != format_; ++i)
//            {
//                base *= 10;
//            }
//            wrappedWidget_->setText(QString().sprintf(fString.toAscii(), initValue / base));
            wrappedWidget_->setText(IntToString(initValue, format_));

        }
    }
    connect(wrappedWidget_,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(EditFinished(QString)));
}

void ICLineEditWrapper::EditFinished(const QString &text)
{
    int value = StringToInt(text, format_);
//    if(format_ == Integer)
//    {
//        value = text.toInt();
//    }
//    else
//    {
//        int base = 1;
//        for(int i = 0; i != format_; ++i)
//        {
//            base *= 10;
//        }
//        value = text.toDouble() * base;
//    }
    if(type_ == Mold)
    {
        ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(addr_), value);
    }
    else
    {
        ICVirtualHost::GlobalVirtualHost()->SetSystemParameter(static_cast<ICVirtualHost::ICSystemParameterAddr>(addr_), value);
        //        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        //        int initValue = ICVirtualHost::GlobalVirtualHost()->SystemParameter(static_cast<ICVirtualHost::ICSystemParameterAddr>(addr_)).toDouble();
        //        if(format_ == Integer)
        //        {
        //            wrappedWidget_->setText(QString::number(initValue));
        //        }
        //        else
        //        {
        //            QString fString = "%." + QString::number(static_cast<int>(format_)) + "f";
        //            qreal base = 1;
        //            for(int i = 0; i != format_; ++i)
        //            {
        //                base *= 10;
        //            }
        //            wrappedWidget_->setText(QString().sprintf(fString.toAscii(), initValue / base));
        //        }
    }
}
