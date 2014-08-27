#include "ictableview.h"
#include <QKeyEvent>

ICTableView::ICTableView(QWidget *parent) :
    QTableView(parent)
{
}


void ICTableView::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICTableView::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
