#ifndef ICALARMTABLEMODEL_H
#define ICALARMTABLEMODEL_H

#include <QSqlRelationalTableModel>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICAlarmTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit ICAlarmTableModel(QObject *parent = 0);
    int IndexLevel(const QModelIndex& index) const ;
    int LevelColumn() const { return levelColumn_;}
    void SetLevelColumn(int index) { levelColumn_ = index;}

signals:

public slots:

private:
    int levelColumn_;

};

inline int ICAlarmTableModel::IndexLevel(const QModelIndex &index) const
{
    if(levelColumn_ < 0)
    {
        return -1;
    }
    return data(index.sibling(index.row(), LevelColumn())).toInt();
}

#endif // ICALARMTABLEMODEL_H
