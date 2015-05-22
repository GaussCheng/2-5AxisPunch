#ifndef ICALARMTABLEDELEGATE_H
#define ICALARMTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>
#include <QColor>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICAlarmTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ICAlarmTableDelegate(QObject *parent = 0);
    QColor LevelColor(int level) const { return levelPaintColorMap_.value(level, Qt::black);}
    void BindLevelColor(int level, QColor color) { levelPaintColorMap_.insert(level, color);}

signals:

public slots:

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QMap<int, QColor> levelPaintColorMap_;
};

#endif // ICALARMTABLEDELEGATE_H
