#ifndef ICFRAMEADAPTER_H
#define ICFRAMEADAPTER_H

#include "icvirtualhost.h"

class QWidget;
class QString;
class ICCommunicationWidget
{
public:
    ICCommunicationWidget();
    virtual ~ICCommunicationWidget(){}

    int HostStatus() const { return hostStatus_;}
    void BindingHostStatus(int status, ICVirtualHost* virtualHost = icGlobalVirtuallHost) {hostStatus_ = status; virtualHost_ = virtualHost;}
    void UpdateValue();

protected:
    virtual void SetValue(const QVariant& value) = 0;
private:
    int hostStatus_;
    ICVirtualHost* virtualHost_;
};

#endif // ICFRAMEADAPTER_H
