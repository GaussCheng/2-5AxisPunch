#ifndef ICCONFIGMONITOR_H
#define ICCONFIGMONITOR_H

#include "ICCore_global.h"
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include "icoptimize.h"

class ICCORESHARED_EXPORT ICConfigMonitor
{
public:
    ICConfigMonitor(int maxRecord = 120);
    virtual ~ICConfigMonitor(){}
    void SetModel(QSqlTableModel* model);
    int MaxRecords() {return maxRecords_;}
    void SetMaxRecords(uint size) {maxRecords_ = size;}

protected:
    virtual int IDColumnIndex() const = 0;
private:
    int maxRecords_;
    QSqlTableModel *model_;
};

inline void ICConfigMonitor::SetModel(QSqlTableModel *model)
{
    model_ = model;
    if(model_  == NULL)
    {
        return;
    }
    model_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_->setSort(0, Qt::DescendingOrder);
    model_->sort(0, Qt::DescendingOrder);
}

#endif // ICCONFIGMONITOR_H
