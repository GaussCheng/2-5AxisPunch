#include "placementfinishedframe.h"
#include "ui_placementfinishedframe.h"

PlacementFinishedFrame::PlacementFinishedFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlacementFinishedFrame)
{
    ui->setupUi(this);
}

PlacementFinishedFrame::~PlacementFinishedFrame()
{
    delete ui;
}
