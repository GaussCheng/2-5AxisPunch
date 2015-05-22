#ifndef ICRECORDTABLEMODEL_H
#define ICRECORDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QDir>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICRecordTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ICRecordTableModel(QObject *parent = 0);
    explicit ICRecordTableModel(const QString &pathName, QObject *parent = 0);
    explicit ICRecordTableModel(const QDir &other, QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

protected:
    bool hasChildren(const QModelIndex &parent) const { Q_UNUSED(parent) return false;}
signals:

public slots:
private:
    QDir monitorDir_;
    QString section_[2];
};



#endif // ICRECORDTABLEMODEL_H
