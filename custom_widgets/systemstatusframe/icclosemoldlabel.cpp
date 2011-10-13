#include "icclosemoldlabel.h"
#include "ictunemolddialog.h"

ICCloseMoldLabel::ICCloseMoldLabel(QWidget *parent) :
    StatusLabel(parent)
{
    dialog_ = new ICTuneMoldDialog();
}

ICCloseMoldLabel::~ICCloseMoldLabel()
{
    delete dialog_;
}

void ICCloseMoldLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
//    OperatingRatioSetDialog::Instance()->show();
    if(currentStatus() != ONSTATUS)
    {
        return;
    }
    dialog_->show();
}
