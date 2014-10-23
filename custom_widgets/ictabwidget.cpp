#include "ictabwidget.h"
#include <QKeyEvent>

ICTabWidget::ICTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}


void ICTabWidget::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICTabWidget::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}
