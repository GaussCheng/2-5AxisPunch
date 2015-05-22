#include "iclog.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

void ICLog::Log(const QString &logContent)
{
    if(LogFileName().isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(LogFileName());
    if(fileInfo.size() / 1024 >= MaxSpace())
    {
        QFile::remove(LogFileName());
    }
    QString writeContent = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") + Sep() + logContent;
    QFile file(LogFileName());
    if(file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out<<writeContent<<endl;
        file.close();
    }

}


QString ICLog::LogContent() const
{
    if(LogFileName().isEmpty())
    {
        return QString();
    }
    QFile file(LogFileName());
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString ret = in.readAll();
        file.close();
        return ret;
    }
    return QString();
}
