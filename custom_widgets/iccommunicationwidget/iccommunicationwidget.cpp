#include <QString>
#include "iccommunicationwidget.h"

ICCommunicationWidget::ICCommunicationWidget()
{
}

void ICCommunicationWidget::UpdateValue()
{
    SetValue(virtualHost_->HostStatus(static_cast<ICVirtualHost::ICStatus>(hostStatus_)));
}
