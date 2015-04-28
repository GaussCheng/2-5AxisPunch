#include "icpointpproperty.h"
#include "ui_icpointpproperty.h"

ICPointpProperty::ICPointpProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICPointpProperty)
{
    ui->setupUi(this);
}

ICPointpProperty::~ICPointpProperty()
{
    delete ui;
}
