#ifndef ICBACKUPUTILITY_H
#define ICBACKUPUTILITY_H


#include <QString>
#include <QStringList>

class ICBackupUtility
{
public:
    ICBackupUtility();
    explicit ICBackupUtility(const QString& src, const QString& dest):
        srcDir_(src),
        destDir_(dest)
    {}
    bool BackupDir(const QStringList& nameFilter = QStringList()) const;
    bool BackupDir(const QString& src,
                   const QString& dest,
                   const QStringList& nameFilter = QStringList());
    bool RestoreDir(const QStringList& nameFilter = QStringList()) const;
    bool RestoreDir(const QString& src, const QString& dest, const QStringList& nameFilter = QStringList());
    QString SrcDir() const { return srcDir_;}
    QString DestDir() const { return destDir_;}
    QStringList SrcDirFiles() const;
    QStringList DestDirFiles(bool isShowHidden = false) const;
    void SetBackupSrcDir(const QString& src) { srcDir_ = src;}
    void SetBackupDestDir(const QString& dest);

private:
    void _CleanDestDir_(const QStringList& nameFilter) const;
    bool _IsDirValid_() const { return !(SrcDir().isEmpty() || DestDir().isEmpty());}
    bool _CopyFiles_(const QStringList& nameFilter) const;
    QString srcDir_;
    QString destDir_;
};

#endif // ICBACKUPUTILITY_H
