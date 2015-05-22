#ifndef ICALARMSYSTEM_H
#define ICALARMSYSTEM_H

#include "ICCore_global.h"
#include <QString>
#include <QStringList>
#include <QBitArray>
#include <QMap>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include "icoptimize.h"

class ICCORESHARED_EXPORT ICAlarmSystem
{
public:
    ICAlarmSystem(uint maxRecord);
    virtual ~ICAlarmSystem(){}

    QString AlarmNumToString(uint num) const { return AlarmNumToStringImpl(num);}
    QStringList AlarmNumsToStringList(const QBitArray& bits) const;
    QStringList SetAlarms(const QBitArray& bits){ return SetAlarmsImpl(bits);}
    QSqlTableModel* AlarmModel() { return model_;}
    void SetAlarmModel(QSqlTableModel* model);
    void SetAlarmTable(const QString& name) { if(likely(model_!= NULL)) model_->setTable(name);}
    void SetMaxRecords(uint size) {maxRecords_ = size;}
    uint MaxRecords() const {return maxRecords_;}

protected:
    //    virtual QSqlRecord CreateAlarmRecord(uint alarmNum) const = 0;
    virtual QString AlarmNumToStringImpl(uint num) const;
    virtual int IDColumnIndex() const = 0;
    //    virtual void AddEndTime(QSqlRecord& record) const = 0;
    virtual QStringList SetAlarmsImpl(const QBitArray& bits) = 0;
    int FindIDRowInModel_(uint id);
private:
private:
    uint maxRecords_;
    QSqlTableModel *model_;
};

inline QString ICAlarmSystem::AlarmNumToStringImpl(uint num) const
{
    if(unlikely(num == 0))
    {
        return QString();
    }
    return "ERR" + QString::number(num);
}

inline void ICAlarmSystem::SetAlarmModel(QSqlTableModel *model)
{
    if(likely(model != NULL))
    {
        model_ = model;
        model_->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model_->setSort(0, Qt::DescendingOrder);
        model_->sort(0, Qt::DescendingOrder);
    }
}

#endif // ICALARMSYSTEM_H
