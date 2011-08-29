#include "pageswitchbar.h"
#include "ui_pageswitchbar.h"

PageSwitchBar::PageSwitchBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PageSwitchBar)
{
    ui->setupUi(this);
    connect(ui->nextPage,
            SIGNAL(clicked()),
            SIGNAL(NextButtonClicked()));
    connect(ui->prePage,
            SIGNAL(clicked()),
            SIGNAL(PreButtonClicked()));
}

PageSwitchBar::~PageSwitchBar()
{
    delete ui;
}

void PageSwitchBar::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PageSwitchBar::on_ioSelector_currentIndexChanged(int index)
{
    if(index == 0)
    {
        emit SelectedInput();
    }
    else if(index == 1)
    {
        emit SelectedOutput();
    }
}

int PageSwitchBar::State() const
{
    return static_cast<CurrentState>(ui->ioSelector->currentIndex());
}
