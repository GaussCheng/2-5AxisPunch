#include "placementmaterialhandlepositionframe.h"
#include "ui_placementmaterialhandlepositionframe.h"

PlacementMaterialHandlePositionFrame::PlacementMaterialHandlePositionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementMaterialHandlePositionFrame)
{
    ui->setupUi(this);
}

PlacementMaterialHandlePositionFrame::~PlacementMaterialHandlePositionFrame()
{
    delete ui;
}
