#include "ictablewidget.h"
#include <QKeyEvent>

ICTableWidget::ICTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

void ICTableWidget::keyPressEvent(QKeyEvent *event)
{
    event->ignore();

}

void ICTableWidget::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore();
}
