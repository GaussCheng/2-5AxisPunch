#include "ichcmanualoperationpageframe.h"
#include "ui_ichcmanualoperationpageframe.h"

#include <QStackedLayout>
#include <QButtonGroup>

//#include "hcmanualalternateioframe.h"
#include "hcmanualfixtureframe.h"
//#include "hcmanualfunctionframe.h"
#include "hcmanualsockerframe.h"
#include "hcmanualotherioframe.h"
#include "hcmanualreservepage.h"

#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "ictimerpool.h"

ICHCManualOperationPageFrame::ICHCManualOperationPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCManualOperationPageFrame),
    manualOtherIOPage_(NULL),
    manualFixturePage_(NULL),
    manualSuckerPage_(NULL),
    manualAdjustPage_(NULL),
    reservePage_(NULL),
    serveAxisPage_(NULL),
    centralStackedLayout_(new QStackedLayout),
    currentPose1_(-1),
    currentPose2_(-1),
    currentAction_(-1)
{
    ui->setupUi(this);
    ui->adjustToolButton->hide();
    buttonGroup_ = new QButtonGroup();
    ui->centralFrame->setLayout(centralStackedLayout_);

    InitInterface();
//    InitSignal();
    ui->fixtureToolButton->click();
}

ICHCManualOperationPageFrame::~ICHCManualOperationPageFrame()
{
    delete buttonGroup_;
    delete ui;
}

void ICHCManualOperationPageFrame::showEvent(QShowEvent *e)
{
//    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop, 0);
    if(manualAdjustPage_ != NULL)
    {
        manualAdjustPage_->ClearStatus();
    }
    QFrame::showEvent(e);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StatusRefreshed()),
//            this,
//            SLOT(StatusRefreshed()));
}

void ICHCManualOperationPageFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//               SIGNAL(StatusRefreshed()),
//               this,
//               SLOT(StatusRefreshed()));
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
        ui->reserveToolButton->setText(tr("Reserve"));
#ifdef HC_8AXIS
//        ui->x1AxisButton->setText(tr("X1 Axis"));
//        ui->y1AxisButton->setText(tr("Y1 Axis"));
//        ui->zAxisButton->setText(tr("Z Axis"));
//        ui->x2AxisButton->setText(tr("X2 Axis"));
//        ui->y2AxisButton->setText(tr("Y2 Axis"));
//        ui->aAxisButton->setText(tr("A Axis"));
//        ui->bAxisButton->setText(tr("B Axis"));
//        ui->cAxisButton->setText(tr("C Axis"));
#endif
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
    ui->reserveToolButton->setText(tr("Reserve"));
//    ui->fixtureToolButton->setCheckable(true);
//    ui->suckerToolButton->setCheckable(true);
//    ui->otherToolButton->setCheckable(true);
//    ui->adjustToolButton->setCheckable(true);
    buttonGroup_->addButton(ui->fixtureToolButton);
    buttonGroup_->addButton(ui->suckerToolButton);
    buttonGroup_->addButton(ui->otherToolButton);
    buttonGroup_->addButton(ui->adjustToolButton);
    buttonGroup_->addButton(ui->reserveToolButton);
#ifdef HC_8AXIS
    ui->x1AxisButton->hide();
    ui->y1AxisButton->hide();
    ui->zAxisButton->hide();
    ui->x2AxisButton->hide();
    ui->y2AxisButton->hide();
    ui->aAxisButton->hide();
    ui->bAxisButton->hide();
    ui->cAxisButton->hide();
    connect(ui->aAddButton,
            SIGNAL(released()),
            SLOT(OnButtonReleased()));
    connect(ui->aSubButton,
            SIGNAL(released()),
            SLOT(OnButtonReleased()));
    connect(ui->bAddButton,
            SIGNAL(released()),
            SLOT(OnButtonReleased()));
    connect(ui->bSubButton,
            SIGNAL(released()),
            SLOT(OnButtonReleased()));
//    ui->x1AxisButton->setText(tr("X1 Axis"));
//    ui->y1AxisButton->setText(tr("Y1 Axis"));
//    ui->zAxisButton->setText(tr("Z Axis"));
//    ui->x2AxisButton->setText(tr("X2 Axis"));
//    ui->y2AxisButton->setText(tr("Y2 Axis"));
//    ui->aAxisButton->setText(tr("A Axis"));
//    ui->bAxisButton->setText(tr("B Axis"));
//    ui->cAxisButton->setText(tr("C Axis"));
//    buttonGroup_->addButton(ui->x1AxisButton);
//    buttonGroup_->addButton(ui->y1AxisButton);
//    buttonGroup_->addButton(ui->zAxisButton);
//    buttonGroup_->addButton(ui->x2AxisButton);
//    buttonGroup_->addButton(ui->y2AxisButton);
//    buttonGroup_->addButton(ui->aAxisButton);
//    buttonGroup_->addButton(ui->bAxisButton);
//    buttonGroup_->addButton(ui->cAxisButton);
//    ui->currentPose->hide();
//    ui->currentAction->hide();
#endif
    QList<QAbstractButton*> buttons = buttonGroup_->buttons();
    for(int i = 0; i != buttons.size(); ++i)
    {
        buttons[i]->setCheckable(true);
        connect(buttons.at(i),
                SIGNAL(clicked()),
                this,
                SLOT(ShowOptionPage()));
    }
    buttonGroup_->setExclusive(true);


}

void ICHCManualOperationPageFrame::InitSignal()
{
//    connect(ui->otherToolButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(ShowOptionPage()));
//    connect(ui->fixtureToolButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(ShowOptionPage()));
//    connect(ui->suckerToolButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(ShowOptionPage()));
//    connect(ui->adjustToolButton,
//            SIGNAL(clicked()),
//            this,
//            SLOT(ShowOptionPage()));

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
    else if(reservePage_ == NULL && clickedButton == ui->reserveToolButton)
    {
        reservePage_ = new HCManualReservePage();
        buttonToPage_.insert(ui->reserveToolButton, reservePage_);
        centralStackedLayout_->addWidget(reservePage_);
    }
    else if(serveAxisPage_ == NULL)
    {
        serveAxisPage_ = new HCServoArmControlFrame();
        centralStackedLayout_->addWidget(serveAxisPage_);
        buttonToPage_.insert(ui->x1AxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->y1AxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->zAxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->x2AxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->y2AxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->aAxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->bAxisButton, serveAxisPage_);
        buttonToPage_.insert(ui->cAxisButton, serveAxisPage_);
        buttonToAxis_.insert(ui->x1AxisButton, ICVirtualHost::ICAxis_AxisX1);
        buttonToAxis_.insert(ui->y1AxisButton, ICVirtualHost::ICAxis_AxisY1);
        buttonToAxis_.insert(ui->zAxisButton, ICVirtualHost::ICAxis_AxisZ);
        buttonToAxis_.insert(ui->x2AxisButton, ICVirtualHost::ICAxis_AxisX2);
        buttonToAxis_.insert(ui->y2AxisButton, ICVirtualHost::ICAxis_AxisY2);
        buttonToAxis_.insert(ui->aAxisButton, ICVirtualHost::ICAxis_AxisA);
        buttonToAxis_.insert(ui->bAxisButton, ICVirtualHost::ICAxis_AxisB);
        buttonToAxis_.insert(ui->cAxisButton, ICVirtualHost::ICAxis_AxisC);
    }
    if(buttonToAxis_.contains(clickedButton))
    {
        serveAxisPage_->SetCurrentAxis(static_cast<ICVirtualHost::ICAxis>(buttonToAxis_.value(clickedButton)));
    }

    centralStackedLayout_->setCurrentWidget(buttonToPage_.value(clickedButton));
}

void ICHCManualOperationPageFrame::StatusRefreshed()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsInputOn(0))
    {
        if(currentPose1_ != 0)
        {
            currentPose1_ = 0;
            ui->currentPose->setText(tr("Horizontal-1"));
        }
    }
    else if(host->IsInputOn(1))
    {
        if(currentPose1_ != 1)
        {
            currentPose1_ = 1;
            ui->currentPose->setText(tr("Vertical-1"));
        }
    }
    else
    {
        if(currentPose1_ != -1)
        {
            currentPose1_ = -1;
            ui->currentPose->setText("");
        }
    }

    if(host->IsInputOn(23))
    {
        if(currentPose2_ != 0)
        {
            currentPose2_ = 0;
            ui->currentPose->setText(ui->currentPose->text() + tr("/Horizontal-2"));
        }
    }
    else if(host->IsInputOn(11))
    {
        if(currentPose2_ != 1)
        {
            currentPose2_ = 1;
            ui->currentPose->setText(ui->currentPose->text() + tr("/Vertical-2"));
        }
    }
    else
    {
        if(currentPose2_ != -1)
        {
            currentPose2_ = -1;
            ui->currentPose->setText(ui->currentPose->text() + "");
        }
    }


    if(host->IsAction(12))
    {
        if(currentAction_ != 12)
        {
            currentAction_ = 12;
            ui->currentAction->setText(tr("Horizontal-1"));
        }
    }
    else if(host->IsAction(13))
    {
        if(currentAction_ != 13)
        {
            currentAction_ = 13;
            ui->currentAction->setText(tr("Vertical-1"));
        }
    }
    else if(host->IsAction(20))
    {
        if(currentAction_ != 20)
        {
            currentAction_ = 20;
            ui->currentAction->setText(tr("Horizontal-2"));
        }
    }
    else if(host->IsAction(21))
    {
        if(currentAction_ != 21)
        {
            currentAction_ = 21;
            ui->currentAction->setText(tr("Vertical-2"));
        }
    }
//    else if(host->IsAction(14))
//    {
//        if(currentAction_ != 14)
//        {
//            currentAction_ = 14;
//            ui->currentAction->setText(tr("Sub Arm Up"));
//        }
//    }
//    else if(host->IsAction(15))
//    {
//        if(currentAction_ != 15)
//        {
//            currentAction_ = 15;
//            ui->currentAction->setText(tr("Sub Arm Down"));
//        }
//    }
//    else if(host->IsAction(16))
//    {
//        if(currentAction_ != 16)
//        {
//            currentAction_ = 16;
//            ui->currentAction->setText(tr("Sub Arm Forward"));
//        }
//    }
//    else if(host->IsAction(17))
//    {
//        if(currentAction_ != 17)
//        {
//            currentAction_ = 17;
//            ui->currentAction->setText(tr("Sub Arm Backward"));
//        }
//    }
}

void ICHCManualOperationPageFrame::on_aAddButton_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_AAdd);
    ICKeyboard::Instace()->SetPressed(true);
}

void ICHCManualOperationPageFrame::OnButtonReleased()
{
    ICKeyboard::Instace()->SetPressed(false);
}

void ICHCManualOperationPageFrame::on_aSubButton_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_ASub);
    ICKeyboard::Instace()->SetPressed(true);
}

void ICHCManualOperationPageFrame::on_bSubButton_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_BSub);
    ICKeyboard::Instace()->SetPressed(true);
}

void ICHCManualOperationPageFrame::on_bAddButton_pressed()
{
    ICKeyboard::Instace()->SetKeyValue(ICKeyboard::VFB_BAdd);
    ICKeyboard::Instace()->SetPressed(true);
}
