#include <QFrame>
#include <QAbstractButton>
#include <QPushButton>
#include <QList>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "iciomonitor.h"
#include "pagetabsbar.h"
#include "pageswitchbar.h"
#include "iciomonitorpagebase.h"


class ICIOMonitor::ICIOMonitorPrivate
{
public:
    ICIOMonitorPrivate(ICIOMonitor* that);
    ~ICIOMonitorPrivate();
    PageTabsBar* pageTabsBar_;
    PageSwitchBar* pageSwitchBar_;
    QStackedWidget* contentWidget_;
    QVBoxLayout *frameLayout_;

    ICIOMonitor* that_;
};

ICIOMonitor::ICIOMonitorPrivate::ICIOMonitorPrivate(ICIOMonitor* that)
{
    that_ = that;
    pageTabsBar_ = new PageTabsBar(that_);
    pageSwitchBar_ = new PageSwitchBar(that_);
    contentWidget_ = new QStackedWidget(that_);
    frameLayout_ = new QVBoxLayout();
    frameLayout_->setContentsMargins(0, 0, 0, 0);
    frameLayout_->setSpacing(0);
    frameLayout_->addWidget(pageTabsBar_);
    frameLayout_->addWidget(pageSwitchBar_);
    frameLayout_->addWidget(contentWidget_);
    that_->setLayout(frameLayout_);

}

ICIOMonitor::ICIOMonitorPrivate::~ICIOMonitorPrivate()
{
    delete pageTabsBar_;
    delete pageSwitchBar_;
    delete contentWidget_;
    delete frameLayout_;
}

ICIOMonitor::ICIOMonitor(QWidget *parent) :
    QFrame(parent),
    pimpl_(new ICIOMonitorPrivate(this))
{
    this->setFrameShape(QFrame::NoFrame);
    connect(pimpl_->pageSwitchBar_,
            SIGNAL(NextButtonClicked()),
            this,
            SLOT(ShowNextPage_()));
    connect(pimpl_->pageSwitchBar_,
            SIGNAL(PreButtonClicked()),
            this,
            SLOT(ShowPrePage_()));
    connect(pimpl_->pageSwitchBar_,
            SIGNAL(SelectedInput()),
            this,
            SLOT(ShowInput_()));
    connect(pimpl_->pageSwitchBar_,
            SIGNAL(SelectedOutput()),
            this,
            SLOT(ShowOutput_()));
    connect(pimpl_->pageTabsBar_,
            SIGNAL(NeedToSwitchPage(QWidget*)),
            this,
            SLOT(SwitchPage(QWidget*)));
}

ICIOMonitor::~ICIOMonitor(){}

void ICIOMonitor::AddPage(ICIOMonitorPageBase *page, const QString &pageTitle)
{
   pimpl_->pageTabsBar_->AddTab(page, pageTitle);
   pimpl_->contentWidget_->addWidget(page);
}

int ICIOMonitor::PageCount() const
{
    return pimpl_->pageTabsBar_->TabCount();
}


void ICIOMonitor::ShowNextPage_()
{
    ICIOMonitorPageBase *page = qobject_cast<ICIOMonitorPageBase*>(pimpl_->contentWidget_->currentWidget());
    page->ShowNextPage();
}

void ICIOMonitor::ShowPrePage_()
{
    ICIOMonitorPageBase *page = qobject_cast<ICIOMonitorPageBase*>(pimpl_->contentWidget_->currentWidget());
    page->ShowPrePage();
}

void ICIOMonitor::ShowInput_()
{
    ICIOMonitorPageBase *page = qobject_cast<ICIOMonitorPageBase*>(pimpl_->contentWidget_->currentWidget());
    page->ShowInputPage();
}

void ICIOMonitor::ShowOutput_()
{
    ICIOMonitorPageBase *page = qobject_cast<ICIOMonitorPageBase*>(pimpl_->contentWidget_->currentWidget());
    page->ShowOutPage();
}

void ICIOMonitor::SwitchPage(QWidget *widget)
{
    ICIOMonitorPageBase *page = qobject_cast<ICIOMonitorPageBase*>(widget);
    if(pimpl_->pageSwitchBar_->State() == PageSwitchBar::Input)
    {
        page->ShowInputPage();
    }
    else if(pimpl_->pageSwitchBar_->State() == PageSwitchBar::Output)
    {
        page->ShowOutPage();
    }
    pimpl_->contentWidget_->setCurrentWidget(widget);
}
