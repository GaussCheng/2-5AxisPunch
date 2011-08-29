#include "icmanualoperationpageframe.h"
#include "ui_icmanualoperationpageframe.h"

#include <QStackedLayout>

#include "servoarmcontrolframe.h"
#include "manualalternateioframe.h"
#include "manualfixtureframe.h"
#include "manualfunctionframe.h"
#include "manualsockerframe.h"

ICManualOperationPageFrame::ICManualOperationPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICManualOperationPageFrame),
    centralStackedLayout_(new QStackedLayout)
{
    ui->setupUi(this);

    ui->centralFrame->setLayout(centralStackedLayout_);

    InitInterface();
    InitSignal();
}

ICManualOperationPageFrame::~ICManualOperationPageFrame()
{
    delete ui;
}

void ICManualOperationPageFrame::InitInterface()
{
    servoArmControlPage_ = new ServoArmControlFrame;
    centralStackedLayout_->addWidget(servoArmControlPage_);

    manualFunctionPage_ = new ManualFunctionFrame;
    buttonToPage_[ui->functionToolButton] = manualFunctionPage_;
    centralStackedLayout_->addWidget(manualFunctionPage_);

    manualAlterateIOPage_ = new ManualAlternateIOFrame;
    buttonToPage_[ui->alternateIOToolButton] = manualAlterateIOPage_;
    centralStackedLayout_->addWidget(manualAlterateIOPage_);

    manualFixturePage_ = new ManualFixtureFrame;
    buttonToPage_[ui->fixtureToolButton] = manualFixturePage_;
    centralStackedLayout_->addWidget(manualFixturePage_);

    manualSuckerPage_ = new ManualSockerFrame;
    buttonToPage_[ui->suckerToolButton] = manualSuckerPage_;
    centralStackedLayout_->addWidget(manualSuckerPage_);
}

void ICManualOperationPageFrame::InitSignal()
{
    connect(ui->zAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->xAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->yAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->cAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->aAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->bAxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->x2AxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));
    connect(ui->y2AxisToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowAxisMovementPage()));

    connect(ui->functionToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowOptionPage()));
    connect(ui->alternateIOToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowOptionPage()));
    connect(ui->fixtureToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowOptionPage()));
    connect(ui->suckerToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowOptionPage()));
}

void ICManualOperationPageFrame::ShowAxisMovementPage()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(centralStackedLayout_ != NULL)
    {
        centralStackedLayout_->setCurrentWidget(servoArmControlPage_);
        servoArmControlPage_->SetCurrentAxis(clickedButton->text());
    }
}

void ICManualOperationPageFrame::ShowOptionPage()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    if(centralStackedLayout_ != NULL)
    {
        centralStackedLayout_->setCurrentWidget(buttonToPage_.value(clickedButton));
    }
}
