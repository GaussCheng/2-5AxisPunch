#include <QLineEdit>
#include "iclineeditwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"
#include <QDebug>

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
        QString fString = "%." + QString::number(static_cast<int>(format_)) + "f";
        qreal base = 1;
        for(int i = 0; i != format_; ++i)
        {
            base *= 10;
        }
        wrappedWidget_->setText(QString().sprintf(fString.toAscii(), initValue / base));
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
            QString fString = "%." + QString::number(static_cast<int>(format_)) + "f";
            qreal base = 1;
            for(int i = 0; i != format_; ++i)
            {
                base *= 10;
            }
            wrappedWidget_->setText(QString().sprintf(fString.toAscii(), initValue / base));
        }
    }
    connect(wrappedWidget_,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(EditFinished(QString)));
}

void ICLineEditWrapper::EditFinished(const QString &text)
{
    int value;
    if(format_ == Integer)
    {
        value = text.toInt();
    }
    else
    {
        int base = 1;
        for(int i = 0; i != format_; ++i)
        {
            base *= 10;
        }
        value = text.toDouble() * base;
    }
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
