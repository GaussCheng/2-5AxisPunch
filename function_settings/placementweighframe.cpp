#include "placementweighframe.h"
#include "ui_placementweighframe.h"

PlacementWeighFrame::PlacementWeighFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementWeighFrame)
{
    ui->setupUi(this);
}

PlacementWeighFrame::~PlacementWeighFrame()
{
    delete ui;
}
