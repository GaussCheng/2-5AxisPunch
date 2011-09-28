#include <QDebug>
#include "iccommunicationcommandbase.h"

ICCommunicationCommandBase::ICCommunicationCommandBase()
    :isSendError_(true),
    tryTimes_(50)
{
//    AddCommandArgFlag("slave");
//    AddCommandArgFlag("startaddr");
//    AddCommandArgFlag("number");
    AddCommandArgFlags(QStringList()<<"slave"<<"startaddr"<<"number");
}

void ICCommunicationCommandBase::SetCommandArgs(const QString &args)
{
    args_ = args;
    QList<QString> flags = flagValueMap_.keys();
    QRegExp regExp;
    int pos = -1;
    for(int i = 0; i != flags.size(); ++i)
    {
        regExp = BuildArgRegExp(flags.at(i));
        pos = regExp.indexIn(args_);
        flagValueMap_.insert(flags.at(i), (pos == -1)? -1: regExp.cap(2).toInt());
    }
}

QRegExp ICCommunicationCommandBase::BuildArgRegExp(const QString &flag) const
{
    QRegExp ret("(" + flag + ":)([0-9]*)([;]?)");
    ret.setCaseSensitivity(Qt::CaseInsensitive);
    return ret;
}

void ICCommunicationCommandBase::AddCommandArgFlags(const QStringList &names)
{
    for(int i = 0; i!= names.size(); ++i)
    {
        AddCommandArgFlag(names.at(i));
    }
}
