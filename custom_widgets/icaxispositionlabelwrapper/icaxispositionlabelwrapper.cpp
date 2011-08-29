#include "icaxispositionlabelwrapper.h"
#include "icaxispositionlabel.h"

ICAxisPositionLabelWrapper::ICAxisPositionLabelWrapper(ICAxisPositionLabel *wrappedWidget)
    :wrappedWidget_(wrappedWidget)
{
}

void ICAxisPositionLabelWrapper::SetValue(const QVariant &value)
{
    Q_ASSERT_X(wrappedWidget_ != NULL, "ICAxisPositionLabelWrapper SetValue", "NULL Pointer");
    wrappedWidget_->SetAxisPosition(QString().sprintf("%.1f", value.toInt() / 10.0));
}
