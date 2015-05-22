#ifndef ICRECORDSYSTEM_H
#define ICRECORDSYSTEM_H

#include "ICCore_global.h"
#include<QString>
#include<QStringList>
#include<QtSql/QSqlTableModel>

class ICCORESHARED_EXPORT ICRecordSystem
{
public:
    ICRecordSystem();
    virtual ~ICRecordSystem(){}
    QString Where() const { return where_;}
    void SetWhere(const QString& name) { where_ = name;}
    QSqlTableModel* Model() { return model_;}
    void SetModel(QSqlTableModel* model) { model_ = model;}
    QString LastError() const{return LastErrorImpl();}
    bool NewRecord(const QString& name) { return NewRecordImpl(name);}
    bool CopyRecord(const QString&  src, const QString target) { return CopyRecordImpl(src, target);}
    bool DeleteRecord(const QString& name) { return DeleteRecordImpl(name);}
    bool LoadRecord(const QString& name) {return LoadRecordImpl(name);}
    bool ImportRecord(const QString& name) { return ImportRecordImpl(name);}
    bool ImportRecords(const QStringList & records);
    bool ExportRecord(const QString& name) { return ExportRecordImpl(name);}
    bool ExportRecords(const QStringList & records);

    QString RecordName(int row) const { return RecordNameImpl(row);}
protected:
    virtual bool NewRecordImpl(const QString& name) = 0;
    virtual bool CopyRecordImpl(const QString&  src, const QString target) = 0;
    virtual bool DeleteRecordImpl(const QString& name) = 0;
    virtual bool LoadRecordImpl(const QString& name) = 0;
    virtual bool ImportRecordImpl(const QString& name) = 0;
    virtual bool ExportRecordImpl(const QString& name) = 0;
    virtual QString LastErrorImpl() const = 0;
    virtual QString RecordNameImpl(int row) const = 0;
private:
    QString where_;
    QSqlTableModel *model_;
};

#endif // ICRECORDSYSTEM_H
