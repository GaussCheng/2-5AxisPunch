#include "icconfigmonitor.h"
#include <QtSql/QSqlError>

ICConfigMonitor::ICConfigMonitor(int maxRecord)
    :maxRecords_(maxRecord),
      model_(NULL)
{
}
