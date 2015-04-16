#include "ichcmanualoperationpageframe.h"
#include "ui_ichcmanualoperationpageframe.h"

#include <QStackedLayout>
#include <QButtonGroup>

#include "iccommandprocessor.h"
#include "icactioncommand.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "ictimerpool.h"
#include "icparameterssave.h"
#include "icaxiskeyboard.h"
#include "iccommandprocessor.h"
#include "icsystemconfig.h"
#include "icvirtualkey.h"
#include <QMessageBox>

#ifdef HC_SK_8_SC
#include "icaxiskeyboard.h"
#endif

QPalette yOnPalette;
QPalette oriPalette;
QString oriStyle;
QString yOnStyle;
static int currentStep  = 0;
ICHCManualOperationPageFrame::ICHCManualOperationPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCManualOperationPageFrame)
{
    ui->setupUi(this);
#ifndef HC_SK_8_SC
#ifdef HC_SK_8
//    ui->axisBoard->hide();
#endif
#endif
#ifdef HC_AXIS_COUNT_2
    ui->label_14->hide();
    ui->label_15->hide();
    ui->zCurrentPos->hide();
    ui->zPos->hide();
    ui->zRun->hide();
    ui->zSpeed->hide();
    ui->zSpeedLabel->hide();
    ui->label_16->hide();
    ui->label_6->hide();
    ui->label_8->hide();
#endif
    //    ui->xCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
    //    ui->yCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
    //    ui->zCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
    InitInterface();
    modifyDialog_ = new AxisModifyDialog();
    yOnPalette.setBrush(QPalette::Button, QBrush(Qt::green));
    oriPalette = ui->b1->palette();
    oriStyle = ui->b1->styleSheet();
    yOnStyle = "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 6px;"
            "border-color: gray;"
            "background-color: green;"
            "padding-right: 6px;"
            "padding-left:6px;";
//    ui->tSpeed->hide();
//    ui->rsSpeed->hide();
    ui->runButton->hide();
    ui->uncheckRunButton->hide();


    ui->groupBox->hide();
    ui->groupBox_2->hide();
    ui->return0Button->hide();
    ui->singleButton->hide();
    delayValidator = new QIntValidator(0, 30000, this);

    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(delayValidator);
}

ICHCManualOperationPageFrame::~ICHCManualOperationPageFrame()
{
    delete ui;
    delete nullButton_;
    delete modifyDialog_;
    delete delayValidator;
}

void ICHCManualOperationPageFrame::showEvent(QShowEvent *e)
{
//    int currentTuneType = ICKeyboard::Instace()->CurrentTuneSpeedType();
//    if(currentTuneType < 0)
//    {
//        ui->xSpeed->setChecked(false);
//        ui->ySpeed->setChecked(false);
//    }
//    else if(currentTuneType == 0)
//    {
//        ui->ySpeed->setChecked(false);
//        ui->xSpeed->setChecked(true);
//    }
//    else
//    {
//        ui->xSpeed->setChecked(false);
//        ui->ySpeed->setChecked(true);
//    }
    QFrame::showEvent(e);
    ui->delayEdit->blockSignals(true);
    ui->delayEdit->SetThisIntToThisText(ICMold::CurrentMold()->MoldNativeParam(ICMold::ClipDelay));
    ui->delayEdit->blockSignals(false);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
//    currentStep = 0;
    timerID_ = startTimer(100);
    nullButton_->click();
    ui->xPos->clear();
    ui->yPos->clear();
    ui->zPos->clear();
    ui->rPos->clear();
    ui->tPos->clear();
    ui->productEdit->blockSignals(true);
    ui->productEdit->SetThisIntToThisText(ICMold::CurrentMold()->MoldParam(ICMold::Product));
    ui->productEdit->blockSignals(false);
    ReserveProgConfig progConfig;
    progConfig.all =  ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_Config_Resv2).toInt();

//    if(!progConfig.b.r8)
    ui->showMore->setVisible(progConfig.b.r8);

    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->xRun->show();ui->label->show();ui->xPos->show();ui->label_4->show();
        ui->xSpeed->show();ui->xSpeedLabel->show();ui->label_12->show();
    }
    else
    {
        ui->xRun->hide();ui->label->hide();ui->xPos->hide();ui->label_4->hide();
        ui->xSpeed->hide();ui->xSpeedLabel->hide();ui->label_12->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->yRun->show();ui->label_2->show();ui->yPos->show();ui->label_5->show();
        ui->ySpeed->show();ui->ySpeedLabel->show();ui->label_13->show();
    }
    else
    {
        ui->yRun->hide();ui->label_2->hide();ui->yPos->hide();ui->label_5->hide();
        ui->ySpeed->hide();ui->ySpeedLabel->hide();ui->label_13->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->zRun->show();ui->label_6->show();ui->zPos->show();ui->label_15->show();
        ui->zSpeed->show();ui->zSpeedLabel->show();ui->label_16->show();
    }
    else
    {
        ui->zRun->hide();ui->label_6->hide();ui->zPos->hide();ui->label_15->hide();
        ui->zSpeed->hide();ui->zSpeedLabel->hide();ui->label_16->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->rRun->show();ui->label_7->show();ui->rPos->show();ui->label_9->show();
        ui->rSpeed->show();ui->rSpeedLabel->show();ui->label_21->show();
    }
    else
    {
        ui->rRun->hide();ui->label_7->hide();ui->rPos->hide();ui->label_9->hide();
        ui->rSpeed->hide();ui->rSpeedLabel->hide();ui->label_21->hide();
    }
    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Servo)
    {
        ui->tRun->show();ui->label_8->show();ui->tPos->show();ui->label_10->show();
        ui->tSpeed->show();ui->tSpeedLabel->show();ui->label_18->show();
    }
    else
    {
        ui->tRun->hide();ui->label_8->hide();ui->tPos->hide();ui->label_10->hide();
        ui->tSpeed->hide();ui->tSpeedLabel->hide();ui->label_18->hide();
    }
    //    button->setChecked(false);

    //    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
}

void ICHCManualOperationPageFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    killTimer(timerID_);
    //    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
    ICMold::CurrentMold()->SaveMoldParamsFile();
    ICVirtualHost::GlobalVirtualHost()->SetSingleRun(false);
    if(ICKeyboard::Instace()->CurrentSwitchStatus() != ICKeyboard::KS_ManualStatu)
    {
        ui->runButton->setChecked(false);
    }
    ui->xSpeed->setChecked(false);
    ui->ySpeed->setChecked(false);
    ui->zSpeed->setChecked(false);
    ui->rSpeed->setChecked(false);
    ui->tSpeed->setChecked(false);
    ui->serveControl->setChecked(false);
}

void ICHCManualOperationPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        Init_();
    }
        break;
    default:
        break;
    }
}

void ICHCManualOperationPageFrame::timerEvent(QTimerEvent *e)
{
    StatusRefreshed();
}

void ICHCManualOperationPageFrame::InitInterface()
{
    //    ui->xCurrentPos->setAttribute(Qt::P);
    ui->productEdit->setValidator(new QIntValidator(0, 65530, this));
    ui->xPos->SetDecimalPlaces(1);
    ui->yPos->SetDecimalPlaces(1);
#ifdef HC_AXIS_COUNT_5
    ui->zPos->SetDecimalPlaces(1);
    ui->rPos->SetDecimalPlaces(1);
    ui->tPos->SetDecimalPlaces(1);
#endif
    ui->xPos->setValidator(new QIntValidator(-32760, 32760, this));
    ui->yPos->setValidator(ui->xPos->validator());
    //    ui->zPos->setValidator(ui->xPos->validator());
    ui->buttonGroup->setId(ui->point1, 0);
    ui->buttonGroup->setId(ui->point2, 1);
    ui->buttonGroup->setId(ui->point3, 2);
    ui->buttonGroup->setId(ui->point4, 3);
    ui->buttonGroup->setId(ui->point5, 4);
    ui->buttonGroup->setId(ui->point6, 5);
    nullButton_ = new QPushButton();
    nullButton_->setCheckable(true);
    nullButton_->hide();
    ui->buttonGroup->addButton(nullButton_);
    ui->buttonGroup->setId(nullButton_, -1);

    ui->actionGroup->setId(ui->action1, 0);
    ui->actionGroup->setId(ui->action2, 1);
    ui->actionGroup->setId(ui->action3, 2);
    ui->actionGroup->setId(ui->action4, 3);
    ui->actionGroup->setId(ui->action5, 4);
    ui->actionGroup->setId(ui->action6, 5);
    ui->actionGroup->setId(ui->action7, 6);
    ui->actionGroup->setId(ui->action8, 7);
    ui->actionGroup->setId(ui->action9, 8);
    ui->actionGroup->setId(ui->action10, 9);
    ui->actionGroup->setId(ui->action11, 10);
    ui->actionGroup->setId(ui->action12, 11);
    ui->actionGroup->setId(ui->action13, 12);
    ui->actionGroup->setId(ui->action14, 13);
    ui->actionGroup->setId(ui->action15, 14);
    ui->actionGroup->setId(ui->action16, 15);
    ui->actionGroup->setId(ui->action17, 16);
    ui->actionGroup->setId(ui->action18, 17);
    ui->actionGroup->setId(ui->action19, 18);
    ui->actionGroup->setId(ui->action20, 19);
    ui->actionGroup->setId(ui->action21, 20);
    ui->actionGroup->setId(ui->action22, 21);
    ui->actionGroup->setId(ui->action23, 22);
    ui->actionGroup->setId(ui->action24, 23);
    ui->actionGroup->setId(ui->action25, 24);
    ui->actionGroup->setId(ui->action26, 25);
    ui->actionGroup->setId(ui->action27, 26);
    ui->actionGroup->setId(ui->action28, 27);
    ui->actionGroup->setId(ui->action29, 28);
    ui->actionGroup->setId(ui->action30, 29);
    ui->actionGroup->setId(ui->action31, 30);
    ui->actionGroup->setId(ui->action32, 31);
    ui->actionGroup->setExclusive(false);
    ui->shortcutGroup->setId(ui->b1, 0);
    ui->shortcutGroup->setId(ui->b2, 1);
    ui->shortcutGroup->setId(ui->b3, 2);
    ui->shortcutGroup->setId(ui->b4, 3);
    ui->shortcutGroup->setId(ui->b5, 4);
    ui->shortcutGroup->setId(ui->b6, 5);
#ifdef HC_SK_8
    ui->shortcutGroup->setId(ui->b7, 6);
    ui->shortcutGroup->setId(ui->b8, 7);
    ui->shortcutGroup->setId(ui->b9, 8);
    ui->shortcutGroup->setId(ui->b10, 9);
    ui->shortcutGroup->setId(ui->b11, 10);
    ui->shortcutGroup->setId(ui->b12, 11);
#endif
    ui->shortcutGroup->setExclusive(false);


    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    QList<QAbstractButton*> buttons = ui->buttonGroup->buttons();
    buttons.removeOne(nullButton_);
    const int bsize = buttons.size();
    int tmp;
    QString name;
    for(int i = 0; i != bsize; ++i)
    {
        tmp = ui->buttonGroup->id(buttons.at(i));
        buttonSignalMapper_.setMapping(buttons.at(i), tmp);
        connect(buttons.at(i),
                SIGNAL(clicked()),
                &buttonSignalMapper_,
                SLOT(map()));
        name = config->GetPointsLocaleName(tmp);
        if(!name.isEmpty())
        {
            buttons[i]->setText(name);
        }
        else
        {
            buttons[i]->hide();
        }

    }
    connect(&buttonSignalMapper_,
            SIGNAL(mapped(int)),
            SLOT(OnPointSelected(int)));

    QList<QAbstractButton*> actions = ui->actionGroup->buttons();
    const int asize = actions.size();
    for(int i = 0; i != asize; ++i)
    {
        tmp = ui->actionGroup->id(actions.at(i));
        actionSignalMapper_.setMapping(actions.at(i), tmp);
        connect(actions.at(i),
                SIGNAL(clicked()),
                &actionSignalMapper_,
                SLOT(map()));
        //        qDebug()<<config->GetIOActionLocaleName(tmp);
        name = config->GetIOActionLocaleNameByID(tmp);
        if(!name.isEmpty())
        {
            actions[i]->setText(name);
        }
        else
        {
            actions[i]->hide();
        }
    }
    connect(&actionSignalMapper_,
            SIGNAL(mapped(int)),
            SLOT(OnActionTriggered(int)));

    QList<QAbstractButton*> shortcuts = ui->shortcutGroup->buttons();
    const int ssize = shortcuts.size();
    for(int i = 0; i != ssize; ++i)
    {
        tmp = ui->shortcutGroup->id(shortcuts.at(i));
        shortcutSignalMapper_.setMapping(shortcuts.at(i), tmp);
        releaseShortcutSignalMapper_.setMapping(shortcuts.at(i), tmp);
        connect(shortcuts.at(i),
                SIGNAL(pressed()),
                &shortcutSignalMapper_,
                SLOT(map()));
        connect(shortcuts.at(i),
                SIGNAL(released()),
                &releaseShortcutSignalMapper_,
                SLOT(map()));
        //        qDebug()<<config->GetIOActionLocaleName(tmp);
        name = config->GetIOActionShortcutLocaleNameByID(tmp);
        if(!name.isEmpty())
        {
            shortcuts[i]->setText(name);
        }
        else
        {
            shortcuts[i]->hide();
        }
    }
    connect(&shortcutSignalMapper_,
            SIGNAL(mapped(int)),
            SLOT(OnShortcutTriggered(int)));
    connect(&releaseShortcutSignalMapper_,
            SIGNAL(mapped(int)),
            SLOT(OnShortcutReleased(int)));
}

void ICHCManualOperationPageFrame::InitSignal()
{

}

void ICHCManualOperationPageFrame::Init_()
{
    ICParametersSave* paraSave = ICParametersSave::Instance();
    QLocale local = QLocale(paraSave->Language());
    QString language = local.name().split("_")[0];

    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    QList<QAbstractButton*> buttons = ui->buttonGroup->buttons();
    buttons.removeOne(nullButton_);
    const int bsize = buttons.size();
    int tmp;
    QString name;
    for(int i = 0; i != bsize; ++i)
    {
        tmp = ui->buttonGroup->id(buttons.at(i));
        name = config->GetPointsLocaleName(tmp,language);
        if(!name.isEmpty())
        {
            buttons[i]->setText(name);
        }

    }

    QList<QAbstractButton*> actions = ui->actionGroup->buttons();
    const int asize = actions.size();
    for(int i = 0; i != asize; ++i)
    {
        tmp = ui->actionGroup->id(actions.at(i));
        name = config->GetIOActionLocaleNameByID(tmp,language);
        if(!name.isEmpty())
        {
            actions[i]->setText(name);
        }

    }

    QList<QAbstractButton*> shortcuts = ui->shortcutGroup->buttons();
    const int ssize = shortcuts.size();
    for(int i = 0; i != ssize; ++i)
    {
        tmp = ui->shortcutGroup->id(shortcuts.at(i));
        name = config->GetIOActionShortcutLocaleNameByID(tmp,language);
        if(!name.isEmpty())
        {
            shortcuts[i]->setText(name);
        }

    }
}

static uint16_t oldX = -1;
static int16_t oldY = 0;
static int16_t oldZ = 0;
static uint16_t oldQ = 0;
static uint16_t oldP = 0;
static int64_t oldS = -1;
//static bool isSingleRun = false;
static int oldStep = -1;
void ICHCManualOperationPageFrame::StatusRefreshed()
{
    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    uint16_t pos = host->HostStatus(ICVirtualHost::XPos).toInt();
//    if(pos != oldX)
//    {
//        oldX = pos;
//        ui->xCurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
//    }
//    int16_t posy = host->HostStatus(ICVirtualHost::YPos).toInt();
//    if(posy != oldY)
//    {
//        oldY = posy;
//        ui->yCurrentPos->setText(QString().sprintf("%.1f", posy / 10.0));
//    }
//#ifdef HC_AXIS_COUNT_5
//    int16_t posz = host->HostStatus(ICVirtualHost::ZPos).toInt();
//    if(posz != oldZ)
//    {
//        oldZ = posz;
//        ui->zCurrentPos->setText(QString().sprintf("%.1f", posz / 10.0));
//    }

//    pos = host->HostStatus(ICVirtualHost::QPos).toInt();
//    if(pos != oldQ)
//    {
//        oldQ = pos;
//        ui->tCurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
//    }

//    pos = host->HostStatus(ICVirtualHost::PPos).toInt();
//    if(pos != oldP)
//    {
//        oldP = pos;
//        ui->x2CurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
//    }
//#endif
    int64_t speed = host->HostStatus(ICVirtualHost::DbgX0).toInt() |
            (host->HostStatus(ICVirtualHost::DbgX1).toUInt() << 16) |
            (((int64_t)(host->HostStatus(ICVirtualHost::DbgY0).toUInt())) << 32);
//    speed = 0;
    if(speed != oldS)
    {
        oldS = speed;
        //        ui->speed->setText(QString::number(pos));
        ui->xSpeedLabel->setText(QString::number((speed >> 8) & 0xFF));
        ui->ySpeedLabel->setText(QString::number(speed & 0xFF));
#ifdef HC_AXIS_COUNT_5
        ui->zSpeedLabel->setText(QString::number((speed >> 16) & 0xFF));
        ui->rSpeedLabel->setText(QString::number((speed >> 40) & 0xFF));
        ui->tSpeedLabel->setText(QString::number((speed >> 32) & 0xFF));
#endif
    }
    bool isSingleRunFinished = host->HostStatus(ICVirtualHost::ActL).toInt() == 0;
    ICMold* mold = ICMold::CurrentMold();
    if(host->IsSingleRun() && isSingleRunFinished)
    {
        ++currentStep;
        currentStep %= mold->MoldContent().size();
        host->SetSingleRun(false);
    }
    ui->singleButton->setEnabled(isSingleRunFinished);
    if(oldStep != currentStep)
    {
        oldStep = currentStep;
        ui->singleButton->setText(QString(tr("Single(%1/%2)")).arg(currentStep).arg(mold->MoldContent().size()));
    }
    QList<QAbstractButton*> buttons = ui->shortcutGroup->buttons();
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info;
    int p;
    if(ui->tabWidget->currentIndex() == 0)
    {
        for(int i = 0; i != buttons.size(); ++i)
        {
            info = config->GetActionShortcutByID(ui->shortcutGroup->id(buttons.at(i)));
            p = (info.type == 2) ? info.pointNum << 1 : info.pointNum;

            if(host->IsOutputOn(p))
            {
//                buttons[i]->setPalette(yOnPalette);
                buttons[i]->setStyleSheet(yOnStyle);
            }
            else
            {
//                buttons[i]->setPalette(oriPalette);
                buttons[i]->setStyleSheet(oriStyle);
            }
        }
    }
    else
    {
        buttons = ui->actionGroup->buttons();
        for(int i = 0; i != buttons.size(); ++i)
        {
            info = config->GetActionByID(ui->actionGroup->id(buttons.at(i)));
            p = (info.type == 2) ? info.pointNum << 1 : info.pointNum;

            if(host->IsOutputOn(p))
            {
//                buttons[i]->setPalette(yOnPalette);
                buttons[i]->setStyleSheet(yOnStyle);
            }
            else
            {
//                buttons[i]->setPalette(oriPalette);
                buttons[i]->setStyleSheet(oriStyle);
            }
        }
    }
}

//void ICHCManualOperationPageFrame::on_xPos_textChanged(const QString &arg1)
//{
//    int currentPoint = ui->buttonGroup->checkedId();
//    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(currentPoint * 3), ui->xPos->TransThisTextToThisInt());
//}

//void ICHCManualOperationPageFrame::on_yPos_textChanged(const QString &arg1)
//{
//    int currentPoint = ui->buttonGroup->checkedId();
//    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(currentPoint * 3 + 1), ui->yPos->TransThisTextToThisInt());
//}

//void ICHCManualOperationPageFrame::on_zPos_textChanged(const QString &arg1)
//{
//    int currentPoint = ui->buttonGroup->checkedId();
//    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(currentPoint * 3 + 2), ui->zPos->TransThisTextToThisInt());
//}

void ICHCManualOperationPageFrame::OnPointSelected(int id)
{
    ICMold* currentMold  = ICMold::CurrentMold();
    //    ui->xPos->blockSignals(true);
    //    ui->yPos->blockSignals(true);
    //    ui->zPos->blockSignals(true);
    ui->xPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 6 )));
    ui->yPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 6 + 1)));
#ifdef HC_AXIS_COUNT_5
    ui->zPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 6 + 2)));
    ui->rPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 6 + 3)));
    ui->tPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 6 + 4)));

#endif
    //    ui->xPos->setEnabled(true);
    //    ui->yPos->setEnabled(true);
    //    ui->zPos->setEnabled(true);
    //    ui->xPos->blockSignals(false);
    //    ui->yPos->blockSignals(false);
    //    ui->zPos->blockSignals(false);
}

void ICHCManualOperationPageFrame::OnActionTriggered(int id)
{
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info = config->GetActionByID(id);
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GOutY + info.type);
    cmd.SetPoint(info.pointNum);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int p = (cmd.GM() == ICMold::GTwoXTwoY) ? info.pointNum * 2 : info.pointNum;
    if(host->IsOutputOn(p))
    {
        cmd.SetIFVal(0);
    }
    else
    {
        cmd.SetIFVal(1);
    }
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::OnShortcutTriggered(int id)
{
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info = config->GetActionShortcutByID(id);
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GOutY + info.type);
    cmd.SetPoint(info.pointNum);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int p = (cmd.GM() == ICMold::GTwoXTwoY) ? info.pointNum * 2 : info.pointNum;
    if(host->IsOutputOn(p))
    {
        cmd.SetIFVal(0);
    }
    else
    {
        cmd.SetIFVal(1);
    }
    //    cmd.SetIFVal(info.dir);
    if(!ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
        QMessageBox::information(this,
                                 "tr",
                                 "err");
    }
}

void ICHCManualOperationPageFrame::OnShortcutReleased(int id)
{
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info = config->GetActionShortcutByID(id);
    if(info.type != 0) return;
    if(info.pointNum != 10 && info.pointNum != 11) return;
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GOutY + info.type);
    cmd.SetPoint(info.pointNum);
    cmd.SetIFVal(0);
    if(!ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
        QMessageBox::information(this,
                                 "tr",
                                 "err");
    }
}

void ICHCManualOperationPageFrame::on_setButton_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    ICMold* currentMold = ICMold::CurrentMold();
    ICPoint p;
    p.pointID = ui->buttonGroup->checkedId();
    p.x = ui->xPos->TransThisTextToThisInt();
    p.y = ui->yPos->TransThisTextToThisInt();
#ifdef HC_AXIS_COUNT_5
    p.z = ui->zPos->TransThisTextToThisInt();
    p.p = currentMold->MoldParam((static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 3)));
    p.q = currentMold->MoldParam((static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 4)));
#endif
    modifyDialog_->StartModify(p);
    OnPointSelected(p.pointID);
    ui->xPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(p.pointID * 6)));
    ui->yPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 1)));
#ifdef HC_AXIS_COUNT_5
    ui->zPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 2)));
    ui->rPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 3)));
    ui->tPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(p.pointID * 6 + 4)));
#endif
}

void ICHCManualOperationPageFrame::on_xRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GX);
    cmd.SetPoint(ui->buttonGroup->checkedId());
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::on_yRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GY);
    cmd.SetPoint(ui->buttonGroup->checkedId());
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

#ifdef HC_AXIS_COUNT_5
void ICHCManualOperationPageFrame::on_zRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GZ);
    cmd.SetPoint(ui->buttonGroup->checkedId());
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::on_rRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GP);
    cmd.SetPoint(ui->buttonGroup->checkedId());
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::on_tRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GQ);
    cmd.SetPoint(ui->buttonGroup->checkedId());
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

#ifdef HC_SK_8_SC
void ICHCManualOperationPageFrame::on_axisBoard_clicked()
{
    ICAxisKeyboard::Instance()->show();
}
#endif

#endif

void ICHCManualOperationPageFrame::on_runButton_toggled(bool checked)
{
    if(checked)
    {
//        ui->runButton->setText(tr("No Check"));
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_F6);
    }
    else
    {
//        ui->runButton->setText(tr("Check"));
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_CYCLE);
    }
}

void ICHCManualOperationPageFrame::on_productEdit_textChanged(const QString &arg1)
{
    ICMold::CurrentMold()->SetMoldParam(ICMold::Product, ui->productEdit->TransThisTextToThisInt());

}

void ICHCManualOperationPageFrame::on_productClear_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_PRODUCT_CLEAR);
    ICVirtualHost::GlobalVirtualHost()->SetFinishProductCount(0);
}

void ICHCManualOperationPageFrame::on_singleButton_clicked()
{
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    ICMold* mold = ICMold::CurrentMold();
    if(mold->MoldContent().empty()) return;
    if(currentStep >= mold->MoldContent().size()) return;
    ICMoldItem item = mold->MoldContent().at(currentStep);
    cmd.SetSlave(1);
    if(item.GMVal() == ICMold::GARC)
    {
        cmd.SetGM(item.IFPos());
    }
    else
    {
        cmd.SetGM(item.GMVal());
    }
    cmd.SetNumber(currentStep);
    cmd.SetPoint(item.SubNum());
    cmd.SetPos(item.Pos());
    cmd.SetIFVal(item.IFVal());
    //    cmd.SetIFVal(1);
    if(ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
        //        ++currentStep;
        //        currentStep %= mold->MoldContent().size();
        ICVirtualHost::GlobalVirtualHost()->SetHostStatus(ICVirtualHost::ActL, 1);
        ICVirtualHost::GlobalVirtualHost()->SetSingleRun(true);
        ui->singleButton->setEnabled(false);
    }

}


void ICHCManualOperationPageFrame::on_return0Button_clicked()
{
    currentStep = 0;
    oldStep = -1;
//    ui->singleButton->setText(QString(tr("Single(%1/%2)")).arg(currentStep).arg(mold->MoldContent().size()));
}

void ICHCManualOperationPageFrame::on_serveControl_toggled(bool checked)
{
    if(checked)
    {
        ui->serveControl->setText(tr("Servo OFF"));
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_SERVO_OFF);
    }
    else
    {
        ui->serveControl->setText(tr("Servo On"));
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_SERVO_ON);
    }
}

void ICHCManualOperationPageFrame::on_xSpeed_toggled(bool checked)
{
    if(checked)
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(0);
        ui->ySpeed->blockSignals(true);
        ui->ySpeed->setChecked(false);
        ui->ySpeed->blockSignals(false);
#ifdef HC_AXIS_COUNT_5
        ui->zSpeed->blockSignals(true);
        ui->zSpeed->setChecked(false);
        ui->zSpeed->blockSignals(false);
        ui->rSpeed->blockSignals(true);
        ui->rSpeed->setChecked(false);
        ui->rSpeed->blockSignals(false);
        ui->tSpeed->blockSignals(true);
        ui->tSpeed->setChecked(false);
        ui->tSpeed->blockSignals(false);
#endif
    }
    else
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(-1);
    }
}

void ICHCManualOperationPageFrame::on_ySpeed_toggled(bool checked)
{
    if(checked)
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(1);
        ui->xSpeed->blockSignals(true);
        ui->xSpeed->setChecked(false);
        ui->xSpeed->blockSignals(false);
#ifdef HC_AXIS_COUNT_5
        ui->zSpeed->blockSignals(true);
        ui->zSpeed->setChecked(false);
        ui->zSpeed->blockSignals(false);
        ui->rSpeed->blockSignals(true);
        ui->rSpeed->setChecked(false);
        ui->rSpeed->blockSignals(false);
        ui->tSpeed->blockSignals(true);
        ui->tSpeed->setChecked(false);
        ui->tSpeed->blockSignals(false);
#endif
    }
    else
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(-1);
    }
}

#ifdef HC_AXIS_COUNT_5
void ICHCManualOperationPageFrame::on_zSpeed_toggled(bool checked)
{
    if(checked)
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(2);
        ui->xSpeed->blockSignals(true);
        ui->xSpeed->setChecked(false);
        ui->xSpeed->blockSignals(false);
        ui->ySpeed->blockSignals(true);
        ui->ySpeed->setChecked(false);
        ui->ySpeed->blockSignals(false);
        ui->rSpeed->blockSignals(true);
        ui->rSpeed->setChecked(false);
        ui->rSpeed->blockSignals(false);
        ui->tSpeed->blockSignals(true);
        ui->tSpeed->setChecked(false);
        ui->tSpeed->blockSignals(false);
    }
    else
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(-1);
    }
}

void ICHCManualOperationPageFrame::on_rSpeed_toggled(bool checked)
{
    if(checked)
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(3);
        ui->xSpeed->blockSignals(true);
        ui->xSpeed->setChecked(false);
        ui->xSpeed->blockSignals(false);
        ui->ySpeed->blockSignals(true);
        ui->ySpeed->setChecked(false);
        ui->ySpeed->blockSignals(false);
        ui->zSpeed->blockSignals(true);
        ui->zSpeed->setChecked(false);
        ui->zSpeed->blockSignals(false);
        ui->tSpeed->blockSignals(true);
        ui->tSpeed->setChecked(false);
        ui->tSpeed->blockSignals(false);
    }
    else
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(-1);
    }
}

void ICHCManualOperationPageFrame::on_tSpeed_toggled(bool checked)
{
    if(checked)
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(4);
        ui->xSpeed->blockSignals(true);
        ui->xSpeed->setChecked(false);
        ui->xSpeed->blockSignals(false);
        ui->ySpeed->blockSignals(true);
        ui->ySpeed->setChecked(false);
        ui->ySpeed->blockSignals(false);
        ui->zSpeed->blockSignals(true);
        ui->zSpeed->setChecked(false);
        ui->zSpeed->blockSignals(false);
        ui->rSpeed->blockSignals(true);
        ui->rSpeed->setChecked(false);
        ui->rSpeed->blockSignals(false);
    }
    else
    {
        ICKeyboard::Instace()->SetCurrentTuneSpeedType(-1);
    }
}
#endif

void ICHCManualOperationPageFrame::on_showMore_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void ICHCManualOperationPageFrame::on_returnManual_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void ICHCManualOperationPageFrame::on_delayEdit_textChanged(const QString &text)
{
    int delay = ui->delayEdit->TransThisTextToThisInt();
    ICMold::CurrentMold()->SetMoldNativeParam(ICMold::ClipDelay,delay);
    ICMold::CurrentMold()->SaveMoldConfigFile();
    emit ChangeDelay(delay);

}
