#include "icrecordsystem.h"

ICRecordSystem::ICRecordSystem()
{
}

bool ICRecordSystem::ImportRecords(const QStringList &records)
{
    QString name;
    bool isOk = true;
    foreach(name, records)
    {
        isOk &= ImportRecord(name);
    }
    return isOk;
}

bool ICRecordSystem::ExportRecords(const QStringList &records)
{
    QString name;
    bool isOk = true;
    foreach(name, records)
    {
        isOk &= ExportRecord(name);
    }
    return isOk;
}
