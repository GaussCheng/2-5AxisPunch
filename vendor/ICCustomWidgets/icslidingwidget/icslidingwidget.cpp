#include "icslidingwidget.h"
#ifndef QT_NO_ANIMATION
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#endif
#include <QEvent>

ICSlidingWidget::ICSlidingWidget(QWidget *parent) :
    QWidget(parent),
    currentIndex_(-1),
    isSliding_(false)
{
#ifndef QT_NO_ANIMATION
    animation1_ = new QPropertyAnimation();
    animation2_ = new QPropertyAnimation();
    animationGroup_ = new QParallelAnimationGroup();
#endif
    stackedLayout_ = new QStackedLayout();
    stackedLayout_->setStackingMode(QStackedLayout::StackAll);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setLayout(stackedLayout_);

    connect(stackedLayout_,
            SIGNAL(currentChanged(int)),
            SIGNAL(currentChanged(int)));
    connect(stackedLayout_,
            SIGNAL(widgetRemoved(int)),
            SIGNAL(widgetRemoved(int)));
#ifndef QT_NO_ANIMATION
    connect(animationGroup_,
            SIGNAL(finished()),
            SLOT(OnFinished()));
#endif
}

ICSlidingWidget::~ICSlidingWidget()
{
#ifndef QT_NO_ANIMATION
    delete animation1_;
    delete animation2_;
    delete animationGroup_;
#endif
    delete stackedLayout_;
}


int ICSlidingWidget::AddWidget(QWidget *widget)
{
    int ret = stackedLayout_->addWidget(widget);
    if(stackedLayout_->count() != 1)
    {
        widget->hide();
    }
    return ret;
}

void ICSlidingWidget::SetCurrentWidget(QWidget *widget, ICSlidingWay way)
{
    if(CurrentWidget() == widget)
    {
        return;
    }
    //    int index = widgets_.indexOf(widget);
    int index = stackedLayout_->indexOf(widget);
    Q_ASSERT_X(index != -1, "ICSlidingWidget::SetCurrentWidget", "Not contain widget");
    if(index == -1)
    {
        return;
    }
    SetSliding(true);
    lastWidget_ = CurrentWidget();
    widget->show();
    currentIndex_ = index;
#ifndef QT_NO_ANIMATION
    animationGroup_->stop();
    while(animationGroup_->animationCount() != 0)
    {
        animationGroup_->removeAnimation(animationGroup_->animationAt(0));
    }
#endif
    switch(way)
    {
#ifndef QT_NO_ANIMATION
    case SlidingWay_SlidingUp:
    {
        animation1_->setTargetObject(CurrentWidget());
        animation1_->setPropertyName("geometry");
        animation1_->setStartValue(CurrentWidget()->rect());
        animation1_->setEndValue(QRect(CurrentWidget()->rect().x(),
                                       this->rect().y() - CurrentWidget()->height(),
                                       CurrentWidget()->width(),
                                       CurrentWidget()->height()));

        animation2_->setTargetObject(widget);
        animation2_->setPropertyName("geometry");
        animation2_->setStartValue(QRect(this->rect().x(),
                                         this->rect().bottom(),
                                         widget->width(),
                                         widget->height()));
        animation2_->setEndValue(QRect(this->rect().x(),
                                       this->rect().y(),
                                       widget->width(),
                                       widget->height()));
        animationGroup_->addAnimation(animation1_);
        animationGroup_->addAnimation(animation2_);
        animationGroup_->start();
    }
    break;
    case SlidingWay_SlidingDown:
    {
        animation1_->setTargetObject(CurrentWidget());
        animation1_->setPropertyName("geometry");
        animation1_->setStartValue(CurrentWidget()->rect());
        animation1_->setEndValue(QRect(CurrentWidget()->rect().x(),
                                       this->rect().bottom() + 2,
                                       CurrentWidget()->width(),
                                       CurrentWidget()->height()));

        animation2_->setTargetObject(widget);
        animation2_->setPropertyName("geometry");
        animation2_->setStartValue(QRect(this->rect().x(),
                                         this->rect().top() - widget->height(),
                                         widget->width(),
                                         widget->height()));
        animation2_->setEndValue(QRect(this->rect().x(),
                                       this->rect().y(),
                                       widget->width(),
                                       widget->height()));
        animationGroup_->addAnimation(animation1_);
        animationGroup_->addAnimation(animation2_);
        animationGroup_->start();
    }
    break;
    case SlidingWay_SlidingLeft:
    {
        animation1_->setTargetObject(CurrentWidget());
        animation1_->setPropertyName("geometry");
        animation1_->setStartValue(CurrentWidget()->rect());
        animation1_->setEndValue(QRect(CurrentWidget()->rect().x() - CurrentWidget()->width(),
                                       this->rect().y(),
                                       CurrentWidget()->width(),
                                       CurrentWidget()->height()));

        animation2_->setTargetObject(widget);
        animation2_->setPropertyName("geometry");
        animation2_->setStartValue(QRect(this->rect().right(),
                                         this->rect().y(),
                                         widget->width(),
                                         widget->height()));
        animation2_->setEndValue(QRect(this->rect().x(),
                                       this->rect().y(),
                                       widget->width(),
                                       widget->height()));
        animationGroup_->addAnimation(animation1_);
        animationGroup_->addAnimation(animation2_);
        animationGroup_->start();
    }
    break;
    case SlidingWay_SlidingRight:
    {
        animation1_->setTargetObject(CurrentWidget());
        animation1_->setPropertyName("geometry");
        animation1_->setStartValue(CurrentWidget()->rect());
        animation1_->setEndValue(QRect(CurrentWidget()->rect().right() + 2,
                                       this->rect().y(),
                                       CurrentWidget()->width(),
                                       CurrentWidget()->height()));

        animation2_->setTargetObject(widget);
        animation2_->setPropertyName("geometry");
        animation2_->setStartValue(QRect(this->rect().left() - widget->width(),
                                         this->rect().y(),
                                         widget->width(),
                                         widget->height()));
        animation2_->setEndValue(QRect(this->rect().x(),
                                       this->rect().y(),
                                       widget->width(),
                                       widget->height()));
        animationGroup_->addAnimation(animation1_);
        animationGroup_->addAnimation(animation2_);
        animationGroup_->start();
    }
    break;
#endif
    default:
    {
        lastWidget_->hide();
        stackedLayout_->setCurrentIndex(index);
        SetSliding(false);
    }
    }
}

void ICSlidingWidget::OnFinished()
{
    lastWidget_->hide();
    stackedLayout_->setCurrentIndex(currentIndex_);
    SetSliding(false);
}

