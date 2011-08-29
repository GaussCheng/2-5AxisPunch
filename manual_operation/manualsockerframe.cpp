#include "manualsockerframe.h"
#include "ui_manualsockerframe.h"

ManualSockerFrame::ManualSockerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ManualSockerFrame)
{
    ui->setupUi(this);
}

ManualSockerFrame::~ManualSockerFrame()
{
    delete ui;
}
