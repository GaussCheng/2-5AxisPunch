#include "manualfunctionframe.h"
#include "ui_manualfunctionframe.h"

ManualFunctionFrame::ManualFunctionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ManualFunctionFrame)
{
    ui->setupUi(this);
}

ManualFunctionFrame::~ManualFunctionFrame()
{
    delete ui;
}
