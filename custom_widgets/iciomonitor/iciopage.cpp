#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "iciopage.h"
#include "icvirtualhost.h"

ICIOPage::ICIOPage(QWidget *parent) :
        QWidget(parent),
        inputs_(64, false),
        outputs_(64, false),
        offPixmap_(":/resource/ledgray(16).png"),
        inputOnPixmap_(":/resource/ledred(16).png"),
        outputOnPixmap_(":/resource/ledgreen(16).png")
{
    frameLayout_ = new QVBoxLayout();
    frameLayout_->setContentsMargins(2, 2, 2, 2);
    frameLayout_->setSpacing(15);
    this->setLayout(frameLayout_);
}
ICIOPage::~ICIOPage()
{
    delete frameLayout_;
}

void ICIOPage::BindingIOPoints(const QList<ICIOPoint> &points)
{
    const int size = points.size();
    QLabel *nums;
    QLabel *leds;
    QLabel *descr;
    QHBoxLayout* itemLayout_;

    ICIOPoint point;
    if(!descrLabels_.isEmpty())
    {
        for(int i = 0; i != size; ++i)
        {
            descrLabels_.at(i)->setText(points.at(i).PointDescription());
        }
    }
    else
    {
        for(int i = 0; i != size; ++i)
        {
            point = points.at(i);
            nums = new QLabel(point.PointNum());
            descr = new QLabel(point.PointDescription());
            leds = new QLabel();
            itemLayout_ = new QHBoxLayout();
            leds->setPixmap(offPixmap_);
            ledToPoint_.insert(leds, point);
            descrLabels_.append(descr);

            nums->setFixedWidth(50);
            descr->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            descr->setFixedWidth(110);

            itemLayout_->addWidget(nums);
            itemLayout_->addWidget(descr);
            itemLayout_->addWidget(leds);
            itemLayout_->addStretch();
            frameLayout_->addLayout(itemLayout_);
        }
        frameLayout_->addStretch();
    }
}

void ICIOPage::UpdateIO()
{
    QMap<QLabel*, ICIOPoint>::iterator p = ledToPoint_.begin();
    ICVirtualHost* virtualHost = ICVirtualHost::GlobalVirtualHost();
    int pos;
    if(PageType() == Input)
    {
        while(p != ledToPoint_.end())
        {
            pos = p.value().HostIOPos();
            if(virtualHost->IsInputOn(pos))
            {
                if(!inputs_.at(pos))
                {
                    inputs_.setBit(pos);
                    p.key()->setPixmap(inputOnPixmap_);
                }
            }
            else
            {
                if(inputs_.at(pos))
                {
                    inputs_.clearBit(pos);
                    p.key()->setPixmap(offPixmap_);
                }
            }
            ++p;
        }
    }
    else
    {
        while(p != ledToPoint_.end())
        {
            pos = p.value().HostIOPos();
            if(virtualHost->IsOutputOn(pos))
            {
                if(!outputs_.at(pos))
                {
                    outputs_.setBit(pos);
                    p.key()->setPixmap(outputOnPixmap_);
                }
            }
            else
            {
                if(outputs_.at(pos))
                {
                    outputs_.clearBit(pos);
                    p.key()->setPixmap(offPixmap_);
                }
            }
            ++p;
        }
    }
}

void ICIOPage::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(UpdateIO()));
}

void ICIOPage::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(UpdateIO()));
}
