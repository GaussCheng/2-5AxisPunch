#include "icplacementsettingframe.h"
#include "ui_icplacementsettingframe.h"

#include <QStackedLayout>

#include "placementdefectiveproductsframe.h"
#include "placementfinishedframe.h"
#include "placementmaterialhandlepositionframe.h"
#include "placementotherframe.h"
#include "placementqualitycontrolframe.h"
#include "placementweighframe.h"

ICPlacementSettingFrame::ICPlacementSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICPlacementSettingFrame),
    settingStackedLayout_(new QStackedLayout)
{
    ui->setupUi(this);

    ui->placementSettingrame->setLayout(settingStackedLayout_);

    InitInterface();
    InitSignal();
}

ICPlacementSettingFrame::~ICPlacementSettingFrame()
{
    delete ui;
}

void ICPlacementSettingFrame::InitInterface()
{
    finishedSettingPage_ = new PlacementFinishedFrame;
    settingButtonToPage_[ui->finishedToolButton] = finishedSettingPage_;
    settingStackedLayout_->addWidget(finishedSettingPage_);

    defectiveProductsSettingPage_ = new PlacementDefectiveProductsFrame;
    settingButtonToPage_[ui->defectiveProductsToolButton] = defectiveProductsSettingPage_;
    settingStackedLayout_->addWidget(defectiveProductsSettingPage_);

    qualityControlPage_ = new PlacementQualityControlFrame;
    settingButtonToPage_[ui->qualityControlToolButton] = qualityControlPage_;
    settingStackedLayout_->addWidget(qualityControlPage_);

    weighSettingPage_ = new PlacementWeighFrame;
    settingButtonToPage_[ui->weighToolButton] = weighSettingPage_;
    settingStackedLayout_->addWidget(weighSettingPage_);

    materialHandlePositionPage_ = new PlacementMaterialHandlePositionFrame;
    settingButtonToPage_[ui->materialHandlePositionToolButton] = materialHandlePositionPage_;
    settingStackedLayout_->addWidget(materialHandlePositionPage_);

    otherSettingPage_ = new PlacementOtherFrame;
    settingButtonToPage_[ui->otherToolButton] = otherSettingPage_;
    settingStackedLayout_->addWidget(otherSettingPage_);
}

void ICPlacementSettingFrame::InitSignal()
{
    connect(ui->finishedToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->defectiveProductsToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->materialHandlePositionToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->qualityControlToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->weighToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->otherToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
}

void ICPlacementSettingFrame::SettingButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(settingStackedLayout_ != NULL)
    {
        settingStackedLayout_->setCurrentWidget(settingButtonToPage_.value(clickedButton));
    }
}
