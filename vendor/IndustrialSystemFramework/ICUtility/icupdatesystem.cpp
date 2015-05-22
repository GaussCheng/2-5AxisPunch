#include "icupdatesystem.h"
#include <stdlib.h>
#include <errno.h>
#include <QDebug>
#include <QProcess>
#include "icutility.h"

ICUpdateSystem::ICUpdateSystem():
    unpackCmd_("decrypt.sh")
{
#ifdef Q_WS_X11
    SetPacksDir(QDir::homePath() + "/HCUpdate_UpdateTest");
#else
    SetPacksDir("/mnt/udisk/");
#endif
}

QStringList ICUpdateSystem::ScanUpdatePacks() const
{
    return packPath_.entryList(QStringList()<<ScanPattern(), QDir::Files);
}

QString ICUpdateSystem::PackProfile(const QString &packName)
{
    QFile readMe(packPath_.absoluteFilePath(packName + "_README"));
    if(readMe.open(QFile::ReadOnly))
    {
        return QString::fromUtf8(readMe.readAll());
    }
    return QString();
}

bool ICUpdateSystem::StartUpdate(const QString &packName)
{
    if(packPath_.exists(packName))
       {
   //        QString dataDir("/opt/Qt/UpdateApps");
           QFile file(packPath_.absoluteFilePath(packName));
           QDir tmpDir = QDir::temp();
           QString tmpFile = tmpDir.absoluteFilePath(packName);
           ICUtility::system(QString("rm " + tmpFile).toAscii());
           if(file.copy(tmpFile))
           {
               ICUtility::system((unpackCmd_ + " " + tmpFile).toAscii());
               QStringList tars = tmpDir.entryList(QStringList()<<"*.tar");
               if(tars.isEmpty()) return false;
               tmpFile = tmpDir.absoluteFilePath(tars.at(0));
   //            tmpFile.chop(4);
   //            system(QString("cd %1 && tar -xf %2").arg(QDir::tempPath()).arg(tmpFile).toAscii());
               ICUtility::system(QString("rm -R %1/HCUpdateTmp").arg(QDir::tempPath()).toLatin1());
               ICUtility::system(QString("mkdir -p %1/HCUpdateTmp && cd %1 && tar -xf %2 -C %1/HCUpdateTmp").arg(QDir::tempPath()).arg(tmpFile).toLatin1());
   //            tmpFile.chop(4);
               tmpDir.cd("HCUpdateTmp");
               QStringList tarDirs = tmpDir.entryList(QStringList()<<"HC*");
               if(tarDirs.isEmpty()) return false;
               tmpDir.cdUp();
               tmpFile = tmpDir.absoluteFilePath("HCUpdateTmp/" + tarDirs.at(0));
               ICUtility::system(QString("chmod 777 %1/ -R").arg(tmpFile).toLatin1());
               ICUtility::system(QString("cd %1 && ./UpdateSystem.sh").arg(tmpFile).toLatin1());
   //            system(QString("cp %1/%2/UpdateSystem.sh %1/UpdateApps && sync && umount /mnt/udisk && reboot").arg(dataDir).arg(tmpFile.mid(4)).toLatin1());
   //            qDebug()<<"Switch to update:"<<QProcess::startDetached(QString("%1/UpdateSystem.sh %1").arg(QDir::temp().absoluteFilePath(tmpFile)).toAscii());
               return true;
               //return system(QString("cd %1 && chmod +x ./UpdateSystem.sh &&./UpdateSystem.sh").arg(QDir::temp().absoluteFilePath(tmpFile)).toAscii()) > 0;
           }
       }
       return false;

}
