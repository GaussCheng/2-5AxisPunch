#include <QDir>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include "icmacrosubroutine.h"

QScopedPointer<ICMacroSubroutine> ICMacroSubroutine::instance_;
ICMacroSubroutine::ICMacroSubroutine(QObject *parent) :
    QObject(parent)
{
}

bool ICMacroSubroutine::ReadMacroSubroutieFiles(const QString &dir)
{
    QDir fileDir(dir);
    if(!fileDir.exists())
    {
        return false;
    }
    subsDir_ = dir;
//    QStringList fileList = fileDir.entryList(QStringList()<<"sub*");
    QStringList fileList(QStringList()<<"sub0.prg"<<"sub1.prg"<<"sub2.prg"<<"sub3.prg"<<"sub4.prg"<<"sub5.prg"<<"sub6.prg"<<"sub7.prg");
//    fileList.removeOne("sub8.prg");
    QFile file;
    QString fileContent;
    QStringList records;
    QStringList items;
    QString fileName;
    QList<ICMoldItem> sub;
    ICMoldItem subItem;
    foreach(fileName, fileList)
    {
        file.setFileName(fileDir.filePath(fileName));
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            qCritical()<<fileName<<" can't open";
            return false;
        }
        fileContent = file.readAll();
        file.close();

        records = fileContent.split('\n', QString::SkipEmptyParts);
        sub.clear();
        for(int i = 0; i != records.size(); ++i)
        {
            items = records.at(i).split(' ', QString::SkipEmptyParts);
//            items.removeAt(2);
            subItem.SetValue(items.at(0).toUInt(),
                             items.at(1).toUInt(),
                             items.at(2).toUInt(),
                             items.at(3).toUInt(),
                             items.at(4).toUInt(),
                             items.at(5).toUInt(),
                             items.at(6).toUInt(),
                             items.at(7).toUInt(),
                             items.at(8).toUInt(),
                             items.at(9).toUInt());

            sub.append(subItem);
        }//foreach(record, records)
        subroutines_.append(sub);
    }//foreach(fileName, fileList)
    return true;
}

bool ICMacroSubroutine::SaveMacroSubroutieFile(int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::SaveMacroSubRoutineFile", "group is out of range");
    bool ret = false;
    ICMold::MoldReSum(subroutines_[group]);
    if(subsDir_.right(1) == "/")
    {
        subsDir_.chop(1);
    }
    QByteArray toWrite;
    QList<ICMoldItem> items = subroutines_.at(group);
    for(int i = 0; i != items.size(); ++i)
    {
        toWrite += items.at(i).ToString() + "\n";
    }
    QFile file(subsDir_ + "/sub" + QString::number(group) + ".prg");
    if(!file.open(QFile::ReadWrite | QFile::Text))
    {
        return false;
    }
    if(file.readAll() != toWrite)
    {
        QFile::copy(file.fileName(), file.fileName() + "~");
        file.write(toWrite);
        file.close();
        QFile::remove(file.fileName() + "~");
        ret = true;
    }
    return ret;
}

uint ICMacroSubroutine::SyncAct() const
{
    uint ret = 0;
    for(int i = 0; i != subroutines_.size(); ++i)
    {
        for(int j = 0; j != subroutines_.at(i).size(); ++j)
        {
            ret += subroutines_.at(i).at(j).GMVal();
        }
    }
    return ret;
}

uint ICMacroSubroutine::SyncSum() const
{
    uint ret = 0;
    for(int i = 0; i != subroutines_.size(); ++i)
    {
        for(int j = 0; j != subroutines_.at(i).size(); ++j)
        {
            ret += subroutines_.at(i).at(j).Sum();
        }
    }
    return ret;
}
