#include "icaxisrunningstatusframe.h"
#include "ui_icaxisrunningstatusframe.h"
//#include "icaxispositionlabelwrapper.h"
#include "icvirtualhost.h"

//ICAxisRunningStatusFrame * ICAxisRunningStatusFrame::instance_ = NULL;

ICAxisRunningStatusFrame::ICAxisRunningStatusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICAxisRunningStatusFrame)
{
    ui->setupUi(this);

    InitInterface();
}

ICAxisRunningStatusFrame::~ICAxisRunningStatusFrame()
{
//    qDeleteAll(wrappers_);
    delete ui;
}

void ICAxisRunningStatusFrame::InitInterface()
{
    QString degreeName('°' & 0xff);
    ui->xAxisPositionLabel->SetAxisStatusAttribute("X", "mm");
    ui->yAxisPositionLabel->SetAxisStatusAttribute("Y", "mm");
    ui->zAxisPositionLabel->SetAxisStatusAttribute("Z", "mm");
//    ui->cAxisPositionLabel->SetAxisStatusAttribute("C", degreeName);
//    ui->aAxisPositionLabel->SetAxisStatusAttribute("A", degreeName);
//    ui->bAxisPositionLabel->SetAxisStatusAttribute("B", degreeName);
//    ui->x2AxisPositionLabel->SetAxisStatusAttribute("X2", "mm");
//    ui->y2AxisPositionLabel->SetAxisStatusAttribute("Y2", "mm");

//    QLayout * layout = ui->axisHorizontalLayout;
//    for(int i = 0; i < layout->count(); ++i)
//    {
//        layout->itemAt(i)->widget()->setStyleSheet("border-radius: 6px; border: 3px solid #abc;");
//    }

//    this->SetXAxisPosition("500");
//    ICAxisPositionLabelWrapper* wrapper = new ICAxisPositionLabelWrapper(ui->xAxisPositionLabel);
//    wrapper->BindingHostStatus(ICVirtualHost::XPos);
//    this->RegisterToRealTimeRefresh(wrapper);
//    wrappers_.append(wrapper);

//    wrapper = new ICAxisPositionLabelWrapper(ui->yAxisPositionLabel);
//    wrapper->BindingHostStatus(ICVirtualHost::YPos);
//    this->RegisterToRealTimeRefresh(wrapper);
//    wrappers_.append(wrapper);

//    wrapper = new ICAxisPositionLabelWrapper(ui->zAxisPositionLabel);
//    wrapper->BindingHostStatus(ICVirtualHost::ZPos);
//    this->RegisterToRealTimeRefresh(wrapper);
//    wrappers_.append(wrapper);
}

void ICAxisRunningStatusFrame::SetZAxisPosition(const QString & position)
{
    ui->zAxisPositionLabel->SetAxisPosition(position);
}

void ICAxisRunningStatusFrame::SetYAxisPosition(const QString & position)
{
    ui->yAxisPositionLabel->SetAxisPosition(position);
}

void ICAxisRunningStatusFrame::SetXAxisPosition(const QString & position)
{
    ui->xAxisPositionLabel->SetAxisPosition(position);
}

//void ICAxisRunningStatusFrame::SetCAxisPosition(const QString & position)
//{
//    ui->cAxisPositionLabel->SetAxisPosition(position);
//}

//void ICAxisRunningStatusFrame::SetAAxisPosition(const QString & position)
//{
//    ui->aAxisPositionLabel->SetAxisPosition(position);
//}

//void ICAxisRunningStatusFrame::SetBAxisPosition(const QString & position)
//{
//    ui->bAxisPositionLabel->SetAxisPosition(position);
//}

//void ICAxisRunningStatusFrame::SetY2AxisPosition(const QString & position)
//{
//    ui->y2AxisPositionLabel->SetAxisPosition(position);
//}

//void ICAxisRunningStatusFrame::SetX2AxisPosition(const QString & position)
//{
//    ui->x2AxisPositionLabel->SetAxisPosition(position);
//}
