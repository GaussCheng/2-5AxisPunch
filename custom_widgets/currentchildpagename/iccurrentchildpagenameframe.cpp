#include "iccurrentchildpagenameframe.h"
#include "ui_iccurrentchildpagenameframe.h"

ICCurrentChildPageNameFrame * ICCurrentChildPageNameFrame::instance_ = NULL;

ICCurrentChildPageNameFrame::ICCurrentChildPageNameFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICCurrentChildPageNameFrame)
{
    ui->setupUi(this);
}

ICCurrentChildPageNameFrame::~ICCurrentChildPageNameFrame()
{
    delete ui;
}

void ICCurrentChildPageNameFrame::SetCurrentChildPageName(const QString & currentChildPageName)
{
    ui->currentChildPageNameLbel->setText(currentChildPageName);
}
