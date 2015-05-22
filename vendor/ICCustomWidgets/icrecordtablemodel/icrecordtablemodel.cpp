#include "icrecordtablemodel.h"
#include <QDateTime>
#include <QDebug>

ICRecordTableModel::ICRecordTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    monitorDir_.setFilter(QDir::Files);
}

ICRecordTableModel::ICRecordTableModel(const QString &pathName, QObject *parent):
    QAbstractTableModel(parent),
    monitorDir_(QDir(pathName))
{
    monitorDir_.setFilter(QDir::Files);
//    monitorDir_.setNameFilters(QStringList()<<"[123456789");
}

ICRecordTableModel::ICRecordTableModel(const QDir &other, QObject *parent):
    QAbstractTableModel(parent),
    monitorDir_(other)
{
    monitorDir_.setFilter(QDir::Files);
//    monitorDir_.setNameFilters(QStringList()<<"[123456789]");
}

QVariant ICRecordTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    QString ret;
    if(index.column() == 0)
    {
        ret = monitorDir_.entryInfoList().at(index.row()).fileName();
        if(ret.contains(QRegExp("\\W")))
        {
            return QVariant();
        }
    }
    else
    {
        ret = monitorDir_.entryInfoList().at(index.row()).fileName();
        if(ret.contains(QRegExp("\\W")))
        {
            return QVariant();
        }
        ret = monitorDir_.entryInfoList().at(index.row()).created().toString(Qt::DefaultLocaleLongDate);
    }
    return ret;
}

QVariant ICRecordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Vertical)
    {
        return section + 1;
    }
    else
    {
        if(section > 1)
        {
            return QVariant();
        }
        return section_[section];
    }
}

bool ICRecordTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(role)
    if(orientation == Qt::Vertical)
    {
        return QAbstractTableModel::setHeaderData(section, orientation, value, role);
    }
    else
    {
        if(section > 1)
        {
            return false;
        }
        section_[section] = value.toString();
        emit headerDataChanged(Qt::Horizontal,section,section);
        return true;
    }
}

int ICRecordTableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    if(!monitorDir_.exists())
    {
        return 0;
    }
    return monitorDir_.count();
}

int ICRecordTableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return 2;
}
