#include "placementdefectiveproductsframe.h"
#include "ui_placementdefectiveproductsframe.h"

PlacementDefectiveProductsFrame::PlacementDefectiveProductsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementDefectiveProductsFrame)
{
    ui->setupUi(this);
}

PlacementDefectiveProductsFrame::~PlacementDefectiveProductsFrame()
{
    delete ui;
}
