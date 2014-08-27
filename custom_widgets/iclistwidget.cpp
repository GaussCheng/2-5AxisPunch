#include "iclistwidget.h"
#include <QKeyEvent>

ICListWidget::ICListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void ICListWidget::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICListWidget::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
