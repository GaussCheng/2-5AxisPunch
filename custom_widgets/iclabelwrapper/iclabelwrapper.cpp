#include <QLabel>
#include "iclabelwrapper.h"

ICLabelWrapper::ICLabelWrapper(QLabel *wrappedWidget)
    :wrappedWidget_(wrappedWidget)
{
}

void ICLabelWrapper::SetValue(const QVariant &value)
{
    Q_ASSERT_X(wrappedWidget_ != NULL, "SetValue", "Null pointer");
    wrappedWidget_->setText(QString().sprintf("%.1f", value.toInt() / 10.0));
}
