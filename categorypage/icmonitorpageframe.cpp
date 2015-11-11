#include <QApplication>
#include "icmonitorpageframe.h"
#include "ui_icmonitorpageframe.h"
#include "iciopoint.h"
#include "iciomonitorpagebase.h"
#include "config.h"
#include "icsystemconfig.h"
#include "icparameterssave.h"

ICMonitorPageFrame::ICMonitorPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICMonitorPageFrame)
{
    ui->setupUi(this);
    otherPageLeft_ = new ICIOMonitorPageBase();
    otherPageRight_ = new ICIOMonitorPageBase();
    euPageLeft_ = new ICIOMonitorPageBase();
    euPageRight_ = new ICIOMonitorPageBase();
    mPageLeft_ = new ICIOMonitorPageBase();
    mPageRight_=new ICIOMonitorPageBase();
    Init_();
}

ICMonitorPageFrame::~ICMonitorPageFrame()
{
    delete ui;
}

void ICMonitorPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        Init_();
        break;
    default:
        break;
    }
}

void ICMonitorPageFrame::Init_()
{
    ICParametersSave* paraSave = ICParametersSave::Instance();
    QLocale local = QLocale(paraSave->Language());
    QString prefix = local.name().split("_")[0];

    QList<ICIOPoint> points;
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    QList<ICUserIOInfo> xInfos = config->AllXInfos();
    const int xSize = xInfos.size();
    for(int i = 0; i != xSize; ++i)
    {
        points.append(ICIOPoint(xInfos.at(i).code, xInfos.at(i).GetLocaleName(prefix), i));
    }
    ui->icMonitorFrameleft->AddPage(otherPageLeft_, tr("Other"));
    ui->icMonitorFrameRight->AddPage(otherPageRight_, tr("Other"));
    otherPageLeft_->BindingPointsToInputPage(points);
    otherPageRight_->BindingPointsToInputPage(points);
    points.clear();

    QList<ICUserIOInfo> yInfos = config->AllYInfos();
    const int ySize = yInfos.size();
    for(int i = 0; i != ySize; ++i)
    {
        points.append(ICIOPoint(yInfos.at(i).code, yInfos.at(i).GetLocaleName(prefix), i));
    }
    otherPageLeft_->BindingPointsToOutpuPage(points);
    otherPageRight_->BindingPointsToOutpuPage(points);

#ifdef TEACH_PAGE
    ui->icMonitorFrameleft->AddPage(euPageLeft_, tr("EU"));
    ui->icMonitorFrameRight->AddPage(euPageRight_, tr("EU"));

    points.clear();
    QList<ICUserIOInfo> euXInfos = config->AllEuXInfos();
//    const int euXSize = euXInfos.size();
//    for(int i = 0; i != euXSize; ++i)
//    {
    points.append(ICIOPoint(euXInfos.at(0).code, euXInfos.at(0).GetLocaleName(prefix), 64));
    points.append(ICIOPoint(euXInfos.at(1).code, euXInfos.at(1).GetLocaleName(prefix), 67));
    points.append(ICIOPoint(euXInfos.at(2).code, euXInfos.at(2).GetLocaleName(prefix), 66));
    points.append(ICIOPoint(euXInfos.at(3).code, euXInfos.at(3).GetLocaleName(prefix), 71));
    points.append(ICIOPoint(euXInfos.at(4).code, euXInfos.at(4).GetLocaleName(prefix), 70));
    points.append(ICIOPoint(euXInfos.at(5).code, euXInfos.at(5).GetLocaleName(prefix), 68));
    points.append(ICIOPoint(euXInfos.at(6).code, euXInfos.at(6).GetLocaleName(prefix), 65));
    points.append(ICIOPoint(euXInfos.at(7).code, euXInfos.at(7).GetLocaleName(prefix), 69));
    points.append(ICIOPoint(euXInfos.at(8).code, euXInfos.at(8).GetLocaleName(prefix), 78));
    points.append(ICIOPoint(euXInfos.at(9).code, euXInfos.at(9).GetLocaleName(prefix), 77));
    points.append(ICIOPoint(euXInfos.at(10).code, euXInfos.at(10).GetLocaleName(prefix), 72));
    points.append(ICIOPoint(euXInfos.at(11).code, euXInfos.at(11).GetLocaleName(prefix), 73));
    points.append(ICIOPoint(euXInfos.at(12).code, euXInfos.at(12).GetLocaleName(prefix), 74));
    points.append(ICIOPoint(euXInfos.at(13).code, euXInfos.at(13).GetLocaleName(prefix), 75));
    points.append(ICIOPoint(euXInfos.at(14).code, euXInfos.at(14).GetLocaleName(prefix), 76));
    points.append(ICIOPoint(euXInfos.at(15).code, euXInfos.at(15).GetLocaleName(prefix), 79));

//    }
//    points.append(ICIOPoint("", tr("Mold Opened"), 35));
//    points.append(ICIOPoint("", tr("Mold Closed"), 34));
//    points.append(ICIOPoint("", tr("Eject Forward"), 39));
//    points.append(ICIOPoint("", tr("Eject Backward"), 38));
////    points.append(ICIOPoint("", tr("Mode open In 2"), 46)); //开模2完
//    points.append(ICIOPoint("", tr("Core 1 In"), 46)); //入芯1到位
//    points.append(ICIOPoint("", tr("Core 1 Out"), 45));
//    points.append(ICIOPoint("", tr("Core 2 In"), 44));
//    points.append(ICIOPoint("", tr("Core 2 Out"), 43));

////    points.append(ICIOPoint("", tr("Safe Door 2"), 33));//安全门2
//    points.append(ICIOPoint("", tr("IMM Reject"), 33));//不良品
//    points.append(ICIOPoint("", tr("ZC5"), 42));
//    points.append(ICIOPoint("", tr("ZC6"), 41));
//    points.append(ICIOPoint("", tr("ZC7"), 40));
//    points.append(ICIOPoint("", tr("Safe Door"), 32));
//    points.append(ICIOPoint("", tr("ES"), 47));
//    points.append(ICIOPoint("", tr("Mold Inter"), 36));
//    points.append(ICIOPoint("", tr("IMM Auto"), 37));

    euPageLeft_->BindingPointsToInputPage(points);
    euPageRight_->BindingPointsToInputPage(points);

    points.clear();
    QList<ICUserIOInfo> euYInfos = config->AllEuYInfos();
//    const int euYSize = euYInfos.size();
//    for(int i = 0; i != euYSize; ++i)
//    {
    points.append(ICIOPoint(euYInfos.at(0).code, euYInfos.at(0).GetLocaleName(prefix), 64));
    points.append(ICIOPoint(euYInfos.at(1).code, euYInfos.at(1).GetLocaleName(prefix), 70));
    points.append(ICIOPoint(euYInfos.at(2).code, euYInfos.at(2).GetLocaleName(prefix), 71));
    points.append(ICIOPoint(euYInfos.at(3).code, euYInfos.at(3).GetLocaleName(prefix), 65));
    points.append(ICIOPoint(euYInfos.at(4).code, euYInfos.at(4).GetLocaleName(prefix), 69));
    points.append(ICIOPoint(euYInfos.at(5).code, euYInfos.at(5).GetLocaleName(prefix), 72));
    points.append(ICIOPoint(euYInfos.at(6).code, euYInfos.at(6).GetLocaleName(prefix), 78));
    points.append(ICIOPoint(euYInfos.at(7).code, euYInfos.at(7).GetLocaleName(prefix), 66));
    points.append(ICIOPoint(euYInfos.at(8).code, euYInfos.at(8).GetLocaleName(prefix), 67));
    points.append(ICIOPoint(euYInfos.at(9).code, euYInfos.at(9).GetLocaleName(prefix), 68));
    points.append(ICIOPoint(euYInfos.at(10).code, euYInfos.at(10).GetLocaleName(prefix), 76));
    points.append(ICIOPoint(euYInfos.at(11).code, euYInfos.at(11).GetLocaleName(prefix), 73));
    points.append(ICIOPoint(euYInfos.at(12).code, euYInfos.at(12).GetLocaleName(prefix), 74));
    points.append(ICIOPoint(euYInfos.at(13).code, euYInfos.at(13).GetLocaleName(prefix), 75));
    points.append(ICIOPoint(euYInfos.at(14).code, euYInfos.at(14).GetLocaleName(prefix), 77));
    points.append(ICIOPoint(euYInfos.at(15).code, euYInfos.at(15).GetLocaleName(prefix), 79));






//    }
//    points.append(ICIOPoint("", tr("Mold Open Permit"), 38)); //允许开模
//    points.append(ICIOPoint("", tr("Mold Close Permit"), 32));
//    points.append(ICIOPoint("", tr("Eject Forward Permit"), 33));
////    points.append(ICIOPoint("", tr("Thimble Permit 1"), 33));
//    points.append(ICIOPoint("", tr("Eject Backward Permit"), 37));
////    points.append(ICIOPoint("", tr("Thimble Permit 2"), 37));
////    points.append(ICIOPoint("", tr("Mode Close Permit 2"), 40));
//    points.append(ICIOPoint("", tr("Core 1 In Permit"), 40));
//    points.append(ICIOPoint("", tr("Core 1 Out Permit"), 46));
//    points.append(ICIOPoint("", tr("Core 2 In Permit"), 41));
//    points.append(ICIOPoint("", tr("Core 2 Out Permit"), 45));

//    points.append(ICIOPoint("", tr("Robot"), 35));
//    points.append(ICIOPoint("", tr("En C6"), 44));
//    points.append(ICIOPoint("", tr("En C7"), 43));
//    points.append(ICIOPoint("", tr("En A8"), 42));
//    points.append(ICIOPoint("", tr("Blank 0"), 34));
//    points.append(ICIOPoint("", tr("Blank 1"), 36));
//    points.append(ICIOPoint("", tr("Blank2"), 39));
//    points.append(ICIOPoint("", tr("Blank3"), 47));

    euPageLeft_->BindingPointsToOutpuPage(points);
    euPageRight_->BindingPointsToOutpuPage(points);

    points.clear();
    QStringList items;
    items<<"M10"<<"M11"<<"M12"<<"M13"<<"M14"
        <<"M15"<<"M16"<<"M17"<<"M20"<<"M21"
       <<"M22"<<"M23"<<"M24"<<"M25"<<"M26"
      <<"M27"<<"M30"<<"M31"<<"M32"<<"M33"
     <<"M34"<<"M35"<<"M36"<<"M37"<<"M40"
    <<"M41"<<"M42"<<"M43"<<"M44"<<"M45"
    <<"M46"<<"M47";
    for(int i = 0; i != items.size(); ++i)
    {
        points.append(ICIOPoint(items.at(i), items.at(i), i + 80));
    }
    mPageLeft_->BindingPointsToOutpuPage(points);
    mPageRight_->BindingPointsToOutpuPage(points);
    ui->icMonitorFrameleft->AddPage(mPageLeft_, tr("M"));
    ui->icMonitorFrameRight->AddPage(mPageRight_, tr("M"));
#endif
}
