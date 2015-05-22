#include "icupdatepackmodel.h"
#include <QDateTime>

ICUpdatePackModel::ICUpdatePackModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    monitorDir_.setFilter(QDir::Files);
}


ICUpdatePackModel::ICUpdatePackModel(const QString &pathName, QObject *parent):
    QAbstractTableModel(parent),
    monitorDir_(QDir(pathName))
{
    monitorDir_.setFilter(QDir::Files);
//    fetchMore(index(0, 0));
}

ICUpdatePackModel::ICUpdatePackModel(const QDir &other, QObject *parent):
    QAbstractTableModel(parent),
    monitorDir_(other)
{
    monitorDir_.setFilter(QDir::Files);
//    fetchMore(index(0, 0));
}

QVariant ICUpdatePackModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    if(index.column() == 0)
    {
        return fileInfoList_.at(index.row()).fileName();
    }
    else
    {
        return fileInfoList_.at(index.row()).created().toString(Qt::DefaultLocaleLongDate);
    }
}

QVariant ICUpdatePackModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool ICUpdatePackModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
        emit headerDataChanged(orientation, section, section);
        return true;
    }
}

int ICUpdatePackModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    if(!monitorDir_.exists())
    {
        return 0;
    }
    return fileInfoList_.size();
}

int ICUpdatePackModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return 2;
}

void ICUpdatePackModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent)
    fileInfoList_ = monitorDir_.entryInfoList(QStringList()<<ScanPattern(), QDir::Files);
    reset();
}
