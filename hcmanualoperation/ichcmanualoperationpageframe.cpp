#include "ichcmanualoperationpageframe.h"
#include "ui_ichcmanualoperationpageframe.h"

#include <QStackedLayout>
#include <QButtonGroup>

#include "hcservoarmcontrolframe.h"
//#include "hcmanualalternateioframe.h"
#include "hcmanualfixtureframe.h"
//#include "hcmanualfunctionframe.h"
#include "hcmanualsockerframe.h"
#include "hcmanualotherioframe.h"

#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"

ICHCManualOperationPageFrame::ICHCManualOperationPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCManualOperationPageFrame),
    manualOtherIOPage_(NULL),
    manualFixturePage_(NULL),
    manualSuckerPage_(NULL),
    manualAdjustPage_(NULL),
    centralStackedLayout_(new QStackedLayout),
    currentPose_(-1),
    currentAction_(-1)
{
    ui->setupUi(this);
    ui->adjustToolButton->hide();
    buttonGroup_ = new QButtonGroup();
    ui->centralFrame->setLayout(centralStackedLayout_);

    InitInterface();
    InitSignal();
    ui->fixtureToolButton->click();
}

ICHCManualOperationPageFrame::~ICHCManualOperationPageFrame()
{
    delete buttonGroup_;
    delete ui;
}

void ICHCManualOperationPageFrame::showEvent(QShowEvent *e)
{
    if(manualAdjustPage_ != NULL)
    {
        manualAdjustPage_->ClearStatus();
    }
    QFrame::showEvent(e);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
}

void ICHCManualOperationPageFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
}

void ICHCManualOperationPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        ui->fixtureToolButton->setText(tr("Fixture"));
        ui->suckerToolButton->setText(tr("Sucker"));
        ui->otherToolButton->setText(tr("Other"));
        ui->adjustToolButton->setText(tr("Adjust"));
    }
        break;
    default:
        break;
    }
}

void ICHCManualOperationPageFrame::InitInterface()
{
//    servoArmControlPage_ = new HCServoArmControlFrame;
//    centralStackedLayout_->addWidget(servoArmControlPage_);

    ui->fixtureToolButton->setText(tr("Fixture"));
    ui->suckerToolButton->setText(tr("Sucker"));
    ui->otherToolButton->setText(tr("Other"));
    ui->adjustToolButton->setText(tr("Adjust"));
    ui->fixtureToolButton->setCheckable(true);
    ui->suckerToolButton->setCheckable(true);
    ui->otherToolButton->setCheckable(true);
    ui->adjustToolButton->setCheckable(true);
    buttonGroup_->addButton(ui->fixtureToolButton);
    buttonGroup_->addButton(ui->suckerToolButton);
    buttonGroup_->addButton(ui->otherToolButton);
    buttonGroup_->addButton(ui->adjustToolButton);
    buttonGroup_->setExclusive(true);

}

void ICHCManualOperationPageFrame::InitSignal()
{
    connect(ui->otherToolButton,
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
    connect(ui->adjustToolButton,
            SIGNAL(clicked()),
            this,
            SLOT(ShowOptionPage()));

}

//void ICHCManualOperationPageFrame::ShowAxisMovementPage()
//{
//    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

//    Q_ASSERT_X(centralStackedLayout_ != NULL, "ICHCManualOperationPageFrame", "centralStackedLayout is NULL");
//    centralStackedLayout_->setCurrentWidget(servoArmControlPage_);
//    servoArmControlPage_->SetCurrentAxis(clickedButton->text());
//}

void ICHCManualOperationPageFrame::ShowOptionPage()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    Q_ASSERT_X(centralStackedLayout_ != NULL, "ICHCManualOperationPageFrame", "centralStackedLayout is NULL");
    if(manualFixturePage_ == NULL && clickedButton == ui->fixtureToolButton)
    {
        manualFixturePage_ = new HCManualFixtureFrame;
        buttonToPage_.insert(ui->fixtureToolButton, manualFixturePage_);
        centralStackedLayout_->addWidget(manualFixturePage_);
    }
    else if(manualOtherIOPage_ == NULL && clickedButton == ui->otherToolButton)
    {
        manualOtherIOPage_ = new HCManualOtherIOFrame;
        buttonToPage_.insert(ui->otherToolButton, manualOtherIOPage_);
        centralStackedLayout_->addWidget(manualOtherIOPage_);
    }
    else if(manualSuckerPage_ == NULL && clickedButton == ui->suckerToolButton)
    {
        manualSuckerPage_ = new HCManualSockerFrame;
        buttonToPage_.insert(ui->suckerToolButton, manualSuckerPage_);
        centralStackedLayout_->addWidget(manualSuckerPage_);
    }
    else if(manualAdjustPage_ == NULL && clickedButton == ui->adjustToolButton)
    {
        manualAdjustPage_ = new HCManualAdjustFrame;
        buttonToPage_.insert(ui->adjustToolButton, manualAdjustPage_);
        centralStackedLayout_->addWidget(manualAdjustPage_);
    }

    centralStackedLayout_->setCurrentWidget(buttonToPage_.value(clickedButton));
}

void ICHCManualOperationPageFrame::StatusRefreshed()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsInputOn(0))
    {
        if(currentPose_ != 0)
        {
            currentPose_ = 0;
            ui->currentPose->setText(tr("Horizontal"));
        }
    }
    else if(host->IsInputOn(1))
    {
        if(currentPose_ != 1)
        {
            currentPose_ = 1;
            ui->currentPose->setText(tr("Vertical"));
        }
    }
    else
    {
        if(currentPose_ != -1)
        {
            currentPose_ = -1;
            ui->currentPose->setText("");
        }
    }

    if(host->IsAction(12))
    {
        if(currentAction_ != 12)
        {
            currentAction_ = 12;
            ui->currentAction->setText(tr("Horizontal"));
        }
    }
    else if(host->IsAction(13))
    {
        if(currentAction_ != 13)
        {
            currentAction_ = 13;
            ui->currentAction->setText(tr("Vertical"));
        }
    }
    else if(host->IsAction(14))
    {
        if(currentAction_ != 14)
        {
            currentAction_ = 14;
            ui->currentAction->setText(tr("Sub Arm Up"));
        }
    }
    else if(host->IsAction(15))
    {
        if(currentAction_ != 15)
        {
            currentAction_ = 15;
            ui->currentAction->setText(tr("Sub Arm Down"));
        }
    }
    else if(host->IsAction(16))
    {
        if(currentAction_ != 16)
        {
            currentAction_ = 16;
            ui->currentAction->setText(tr("Sub Arm Forward"));
        }
    }
    else if(host->IsAction(17))
    {
        if(currentAction_ != 17)
        {
            currentAction_ = 17;
            ui->currentAction->setText(tr("Sub Arm Backward"));
        }
    }
}
