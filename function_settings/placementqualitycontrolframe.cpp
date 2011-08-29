#include "placementqualitycontrolframe.h"
#include "ui_placementqualitycontrolframe.h"

PlacementQualityControlFrame::PlacementQualityControlFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementQualityControlFrame)
{
    ui->setupUi(this);
}

PlacementQualityControlFrame::~PlacementQualityControlFrame()
{
    delete ui;
}
