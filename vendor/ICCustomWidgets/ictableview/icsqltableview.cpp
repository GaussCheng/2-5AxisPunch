#include "icsqltableview.h"
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlIndex>
#include <QSqlQuery>

ICSqlTableView::ICSqlTableView(QWidget *parent) :
    ICTableView(parent)
{
}

QSqlTableModel* ICSqlTableView::SqlTableModel() const
{
    return qobject_cast<QSqlTableModel*>(model());
}

void ICSqlTableView::SetSqlTableModel(QSqlTableModel *model)
{
    setModel(model);
}

QString ICSqlTableView::TableName() const
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return QString();
    }
    return m->tableName();
}

void ICSqlTableView::SetTableName(const QString &tableName, QSqlDatabase db)
{
    QSqlTableModel* m;
    if(model() == NULL)
    {
        m = new QSqlTableModel(NULL, db);
        SetSqlTableModel(m);
    }
    else
    {
        m = qobject_cast<QSqlTableModel*>(model());
    }
    m->setTable(tableName);
    m->select();


}

int ICSqlTableView::RecordCount() const
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return 0;
    }
    return m->rowCount();
}

QSqlRecord ICSqlTableView::Record() const
{
    return Record(-1);
}

QSqlRecord ICSqlTableView::Record(int row) const
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return QSqlRecord();
    }
    return m->record(row);
}

bool ICSqlTableView::InsertRecord(int row, const QSqlRecord &record)
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return false;
    }
    bool ret = m->insertRecord(row, record);
    return ret;
}

bool ICSqlTableView::UpdateRecord(int row, const QSqlRecord &record)
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return false;
    }
    bool ret = m->setRecord(row, record);
//    bool ret = m->setData(m->index(row,))
    return ret;
}

bool ICSqlTableView::ClearRecords()
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return false;
    }
    if(RecordCount() == 0)
    {
        return true;
    }
//    QSqlTableModel::EditStrategy es = m->editStrategy();
//    m->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    bool ret = m->removeRows(0, RecordCount());
//    m->submitAll();
//    m->setEditStrategy(es);
    QSqlQuery query(QString("delete from %1").arg(m->tableName()));
    bool ret =  query.exec();
    m->select();
    return ret;
}

void ICSqlTableView::SetSort(int row, Qt::SortOrder order)
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return;
    }
    m->setSort(row, order);
    m->select();
}

bool ICSqlTableView::SetHeaderData(int sec, Qt::Orientation orien, const QVariant &value, int role)
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return false;
    }
    return m->setHeaderData(sec, orien, value, role);
}

QVariant ICSqlTableView::HeaderData(int sec, Qt::Orientation orien, int role) const
{
    QSqlTableModel* m = SqlTableModel();
    if(m == NULL)
    {
        return QVariant();
    }
    return m->headerData(sec, orien, role);
}
