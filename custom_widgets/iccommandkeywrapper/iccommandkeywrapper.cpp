#include <QAbstractButton>
#include "iccommandkeywrapper.h"
#include "iccommandprocessor.h"

ICCommandKeyWrapper::ICCommandKeyWrapper(QAbstractButton *button, int vKey, QObject *parent) :
    QObject(parent),
    wrappedWidget_(button),
    vKey_(vKey)
{
    connect(wrappedWidget_,
            SIGNAL(clicked()),
            this,
            SLOT(keyPressed()));
}

void ICCommandKeyWrapper::keyPressed()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(vKey_);
}
