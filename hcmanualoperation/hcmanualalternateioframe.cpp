#include "hcmanualalternateioframe.h"
#include "ui_hcmanualalternateioframe.h"

HCManualAlternateIOFrame::HCManualAlternateIOFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCManualAlternateIOFrame)
{
    ui->setupUi(this);
}

HCManualAlternateIOFrame::~HCManualAlternateIOFrame()
{
    delete ui;
}
