#include "icmachinestructpage.h"
#include "ui_icmachinestructpage.h"

ICMachineStructPage::ICMachineStructPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineStructPage)
{
    ui->setupUi(this);
}

ICMachineStructPage::~ICMachineStructPage()
{
    delete ui;
}

void ICMachineStructPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
