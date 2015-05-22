#include "ictablewidget.h"
#include <QKeyEvent>

ICTableWidget::ICTableWidget(QWidget *parent) :
    QTableWidget(parent)
{

}

ICTableWidget::~ICTableWidget()
{

}

void ICTableWidget::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICTableWidget::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
