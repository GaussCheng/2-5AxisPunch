#include "icframe.h"
#include "iccommunicationwidget.h"
#include "icvirtualhost.h"

ICFrame::ICFrame(QWidget *parent) :
        QFrame(parent)
{

}

void ICFrame::UpdateRegisteredWidgets()
{
    foreach(ICCommunicationWidget* widget, registeredWidgets_)
    {
        widget->UpdateValue();
    }
}

void ICFrame::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(UpdateRegisteredWidgets()));
    QFrame::showEvent(e);
}

void ICFrame::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(UpdateRegisteredWidgets()));
    QFrame::hideEvent(e);
}
