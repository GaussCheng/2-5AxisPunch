#include "manualalternateioframe.h"
#include "ui_manualalternateioframe.h"

ManualAlternateIOFrame::ManualAlternateIOFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ManualAlternateIOFrame)
{
    ui->setupUi(this);
}

ManualAlternateIOFrame::~ManualAlternateIOFrame()
{
    delete ui;
}
