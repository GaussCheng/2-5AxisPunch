#include "icalarmtabledelegate.h"
#include "icalarmtablemodel.h"
#include <QPainter>
#include <QDebug>

ICAlarmTableDelegate::ICAlarmTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    BindLevelColor(0, Qt::black);
    BindLevelColor(1, Qt::yellow);
    BindLevelColor(2, QColor(255, 0, 255));
}

void ICAlarmTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const ICAlarmTableModel* model = qobject_cast<const ICAlarmTableModel*>(index.model());
    if(model != NULL)
    {
        painter->save();
//        QString level =  model->filter();
//        QStringList ql =level.split(' ');
//        painter->setPen(LevelColor(ql.at(ql.size() -1).toInt()));
        painter->setPen(LevelColor(model->IndexLevel(index)));
//        painter->setPen(Qt::white);
        painter->drawText(option.rect, option.displayAlignment, index.data().toString());
        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

}
