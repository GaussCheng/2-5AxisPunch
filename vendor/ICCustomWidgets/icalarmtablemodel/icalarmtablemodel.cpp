#include "icalarmtablemodel.h"

ICAlarmTableModel::ICAlarmTableModel(QObject *parent) :
    QSqlRelationalTableModel(parent),
    levelColumn_(-1)
{
}
