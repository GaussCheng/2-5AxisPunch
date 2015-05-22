#ifndef ICLOG_H
#define ICLOG_H

#include "ICCore_global.h"
#include <QString>

class ICCORESHARED_EXPORT ICLog
{
public:
    explicit ICLog():sep_("<:>"), maxSpace_(1024){}
    explicit ICLog(const QString& logFile):
        logFile_(logFile), sep_("<:>"), maxSpace_(1024){}
    QString LogFileName() const { return logFile_;}
    void SetLogFileName(const QString& logFile) { logFile_ = logFile;}
    int MaxSpace() const { return maxSpace_;}
    void SetMaxSpace(int maxSpace) { maxSpace_ = maxSpace; }
    void Log(const QString& logContent);
    QString LogContent() const;
    QString Sep() const { return sep_;}
    void SetSep(const QString& sep) { sep_ = sep;}

private:
    QString logFile_;
    QString sep_;
    int maxSpace_;
};


#endif // ICLOG_H
