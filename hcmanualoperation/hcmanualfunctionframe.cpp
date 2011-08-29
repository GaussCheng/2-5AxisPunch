#include "hcmanualfunctionframe.h"
#include "ui_hcmanualfunctionframe.h"

HCManualFunctionFrame::HCManualFunctionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCManualFunctionFrame)
{
    ui->setupUi(this);
}

HCManualFunctionFrame::~HCManualFunctionFrame()
{
    delete ui;
}
