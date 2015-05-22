#ifndef ICSQLTABLEVIEW_H
#define ICSQLTABLEVIEW_H

#include "ictableview.h"
#include <QSqlRecord>
#include <QSqlDatabase>
#include "ICCustomWidgets_global.h"


class QSqlTableModel;

class ICCUSTOMWIDGETSSHARED_EXPORT ICSqlTableView : public ICTableView
{
    Q_OBJECT
public:
    explicit ICSqlTableView(QWidget *parent = 0);
    
    QSqlTableModel* SqlTableModel() const;
    void SetSqlTableModel(QSqlTableModel* model);

    QString TableName() const;
    void SetTableName(const QString& tableName, QSqlDatabase db = QSqlDatabase());

    int RecordCount() const;

    QSqlRecord Record() const;
    QSqlRecord Record(int row) const;

    bool InsertRecord(int row, const QSqlRecord& record);
    bool UpdateRecord(int row, const QSqlRecord& record);
    bool ClearRecords();

    void SetSort(int row, Qt::SortOrder order);

    bool SetHeaderData(int sec, Qt::Orientation orien, const QVariant & value, int role = Qt::EditRole);
    QVariant HeaderData(int sec, Qt::Orientation orien, int role = Qt::DisplayRole) const;
signals:
    
public slots:

    
};

#endif // ICSQLTABLEVIEW_H
