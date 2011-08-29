#include "iciomonitorpagebase.h"
#include "iciopage.h"

#include <QDebug>

ICIOMonitorPageBase::ICIOMonitorPageBase(QWidget *parent) :
    QStackedWidget(parent),
    pagePointsCount_(8)
{
    inputPage_ = new QStackedWidget();
    outputPage_ = new QStackedWidget();
    electricAdjustmentBitPage_ = new QStackedWidget();
    this->addWidget(inputPage_);
    this->addWidget(outputPage_);
    this->addWidget(electricAdjustmentBitPage_);
}
ICIOMonitorPageBase::~ICIOMonitorPageBase()
{
    delete inputPage_;
    delete outputPage_;
    delete electricAdjustmentBitPage_;
}

void ICIOMonitorPageBase::ShowInputPage()
{
    this->setCurrentWidget(inputPage_);
}

void ICIOMonitorPageBase::ShowOutPage()
{
    this->setCurrentWidget(outputPage_);
}

void ICIOMonitorPageBase::ShowElectricAdjustmentBitPage()
{
    this->setCurrentWidget(electricAdjustmentBitPage_);
}

void ICIOMonitorPageBase::BindingPointToPage_(const QList<ICIOPoint> &points, QStackedWidget *page, int type)
{
    if(page->count() != 0)
    {
        ICIOPage* ioPage;
        for(int i = 0; i != page->count(); ++i)
        {
            ioPage = qobject_cast<ICIOPage*>(page->widget(i));
            ioPage->BindingIOPoints(points.mid(i * PagePointsCount(), PagePointsCount()));
        }
    }
    else
    {
        int pageCount = points.size() / PagePointsCount();
        if(points.size() % PagePointsCount() != 0)
        {
            ++pageCount;
        }
        ICIOPage *pages;
        for(int i = 0; i != pageCount; ++i)
        {
            pages = new ICIOPage(page);
            pages->SetPageType(static_cast<ICIOPage::IOType>(type));
            page->addWidget(pages);
            pages->BindingIOPoints(points.mid(i * PagePointsCount(), PagePointsCount()));
        }
    }
}

void ICIOMonitorPageBase::BindingPointsToInputPage(const QList<ICIOPoint> &points)
{
    BindingPointToPage_(points, qobject_cast<QStackedWidget*>(inputPage_), ICIOPage::Input);
}

void ICIOMonitorPageBase::BindingPointsToOutpuPage(const QList<ICIOPoint> &points)
{
    BindingPointToPage_(points, qobject_cast<QStackedWidget*>(outputPage_), ICIOPage::Output);
}

void ICIOMonitorPageBase::ShowNextPage()
{
    QStackedWidget *currentWidget = qobject_cast<QStackedWidget*>(this->currentWidget());
    if(currentWidget->count() == 0)
    {
        return;
    }
    int currentIndex = currentWidget->currentIndex();
    ++currentIndex;
    currentWidget->setCurrentIndex(currentIndex % currentWidget->count());
}

void ICIOMonitorPageBase::ShowPrePage()
{
    QStackedWidget *currentWidget = qobject_cast<QStackedWidget*>(this->currentWidget());
    if(currentWidget->count() == 0)
    {
        return;
    }
    int currentIndex = currentWidget->currentIndex();
    --currentIndex;
    if(currentIndex < 0)
    {
        currentWidget->setCurrentIndex((currentWidget->count() + currentIndex) % currentWidget->count());
    }
    else
    {
        currentWidget->setCurrentIndex(currentIndex % currentWidget->count());
    }
}
