#ifndef ICUPDATEPACKMODEL_H
#define ICUPDATEPACKMODEL_H

#include <QAbstractTableModel>
#include <QDir>
#if defined(ICUTILITY_LIBRARY)
#include "ICCustomWidgets_global.h"
#else
#define ICCUSTOMWIDGETSSHARED_EXPORT
#endif

class ICCUSTOMWIDGETSSHARED_EXPORT ICUpdatePackModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ICUpdatePackModel(QObject *parent = 0);
    explicit ICUpdatePackModel(const QString &pathName, QObject *parent = 0);
    explicit ICUpdatePackModel(const QDir &other, QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    virtual void fetchMore ( const QModelIndex & parent );
    QString ScanPattern() const { return pattern_;}
    void SetScanPattern(const QString& pattern) { pattern_ = pattern;}
protected:
    bool hasChildren(const QModelIndex &parent) const { Q_UNUSED(parent) return false;}
signals:

public slots:
private:
    QDir monitorDir_;
    QString section_[2];
    QFileInfoList fileInfoList_;
    QString pattern_;
};

#endif // ICUPDATEPACKMODEL_H
