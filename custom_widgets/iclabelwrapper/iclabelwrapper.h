#ifndef ICLABELWRAPPER_H
#define ICLABELWRAPPER_H

#include "iccommunicationwidget.h"

class QLabel;

class ICLabelWrapper : public ICCommunicationWidget
{
public:
    explicit ICLabelWrapper(QLabel* wrappedWidget);

protected:
    virtual void SetValue(const QVariant &value);

private:
    QLabel* wrappedWidget_;
};

#endif // ICLABELWRAPPER_H
