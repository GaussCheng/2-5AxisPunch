#include <QLabel>

#include "icvirtualhost.h"
#include "iciopointwrapper.h"

ICIOPointWrapper::ICIOPointWrapper(QLabel *label, int pos, Type type, QObject *parent) :
    QObject(parent),
    wrappedWidget_(label),
    pos_(pos),
    type_(type),
    offPixmap_(":/statusleds/ledgray(16).png"),
    inputPixmap_(":/statusleds/ledred(16).png"),
    outputPixmap_(":/statusleds/ledgreen(16).png")
{
}

void ICIOPointWrapper::UpdateStatus()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(type_ == Input)
    {
        if(host->IsInputOn(pos_))
        {
            wrappedWidget_->setPixmap(inputPixmap_);
        }
        else
        {
            wrappedWidget_->setPixmap(offPixmap_);
        }
    }
    else
    {
        if(host->IsOutputOn(pos_))
        {
            wrappedWidget_->setPixmap(outputPixmap_);
        }
        else
        {
            wrappedWidget_->setPixmap(offPixmap_);
        }
    }
}
