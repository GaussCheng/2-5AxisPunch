#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include "pagetabsbar.h"

PageTabsBar::PageTabsBar(QWidget *parent):
    QFrame(parent),
    frameLayout_(new QHBoxLayout()),
    tabButtonGroup_(new QButtonGroup(this))
{
    this->setFrameShape(QFrame::NoFrame);
    tabSize_.setWidth(70);
    tabSize_.setHeight(50);
    frameLayout_->setContentsMargins(0,0,0, 10);
    this->setLayout(frameLayout_);
    frameLayout_->setSpacing(6);
    frameLayout_->addStretch();
//    frameLayout_->addSpacing(0);

}

PageTabsBar::~PageTabsBar()
{
    qDeleteAll(tabsWidgetMap_.keys());
}

void PageTabsBar::AddTab(QWidget *tabWidget, const QString &tabName)
{
    if(tabsWidgetToTabButtonMap_.contains(tabWidget))
    {
        tabsWidgetToTabButtonMap_.value(tabWidget)->setText(tabName);
    }
    else
    {
        QAbstractButton *button = new QPushButton(tabName);
        button->setCheckable(true);
        button->setMinimumHeight(38);
        button->setMinimumWidth(80);

        if(tabButtonGroup_->buttons().isEmpty())
        {
            button->setChecked(true);
        }
        connect(button,
                SIGNAL(clicked()),
                this,
                SLOT(TabClicked()));
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->resize(tabSize_);
        tabsWidgetMap_.insert(button, tabWidget);
        tabsWidgetToTabButtonMap_.insert(tabWidget, button);
        frameLayout_->insertWidget(tabsWidgetMap_.size() - 1, button);
        tabButtonGroup_->addButton(button);
    }
}

void PageTabsBar::TabClicked()
{
    QAbstractButton *button = qobject_cast<QAbstractButton*>(sender());
    emit NeedToSwitchPage(tabsWidgetMap_.value(button));
}
