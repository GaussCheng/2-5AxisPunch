#ifndef ICAXISPOSITIONLABELWRAPPER_H
#define ICAXISPOSITIONLABELWRAPPER_H

#include "iccommunicationwidget.h"
class ICAxisPositionLabel;

class ICAxisPositionLabelWrapper:public ICCommunicationWidget
{
public:
    ICAxisPositionLabelWrapper(ICAxisPositionLabel* wrappedWidget);

protected:
    virtual void SetValue(const QVariant &value);

private:
    ICAxisPositionLabel* wrappedWidget_;
};

#endif // ICAXISPOSITIONLABELWRAPPER_H
