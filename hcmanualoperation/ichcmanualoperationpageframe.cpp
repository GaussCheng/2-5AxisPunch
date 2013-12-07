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

ICHCManualOperationPageFrame::ICHCManualOperationPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCManualOperationPageFrame)
{
    ui->setupUi(this);
//    ui->xCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
//    ui->yCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
//    ui->zCurrentPos->setAttribute(Qt::WA_PaintOnScreen);
    InitInterface();
    modifyDialog_ = new AxisModifyDialog();
}

ICHCManualOperationPageFrame::~ICHCManualOperationPageFrame()
{
    delete ui;
}

void ICHCManualOperationPageFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnManual, 0);
    timerID_ = startTimer(100);
    nullButton_->click();
    ui->xPos->clear();
    ui->yPos->clear();
    ui->productEdit->blockSignals(true);
    ui->productEdit->SetThisIntToThisText(ICMold::CurrentMold()->MoldParam(ICMold::Product));
    ui->productEdit->blockSignals(false);

//    button->setChecked(false);

//    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
}

void ICHCManualOperationPageFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    killTimer(timerID_);
//    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
    ICMold::CurrentMold()->SaveMoldParamsFile();
}

void ICHCManualOperationPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
//        ui->retranslateUi(this);
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
//    ui->zPos->SetDecimalPlaces(1);
    ui->xPos->setValidator(new QIntValidator(-32760, 32760, this));
    ui->yPos->setValidator(ui->xPos->validator());
//    ui->zPos->setValidator(ui->xPos->validator());
    ui->buttonGroup->setId(ui->point1, 0);
    ui->buttonGroup->setId(ui->point2, 1);
    ui->buttonGroup->setId(ui->point3, 2);
    ui->buttonGroup->setId(ui->point4, 3);
    ui->buttonGroup->setId(ui->point5, 4);
    ui->buttonGroup->setId(ui->point6, 5);
    ui->buttonGroup->setId(ui->point7, 6);
    ui->buttonGroup->setId(ui->point8, 7);
    ui->buttonGroup->setId(ui->point9, 8);
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
        connect(shortcuts.at(i),
                SIGNAL(clicked()),
                &shortcutSignalMapper_,
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
}

void ICHCManualOperationPageFrame::InitSignal()
{

}

static int16_t oldX = -1;
static int16_t oldY = -1;
static int16_t oldZ = -1;
static int16_t oldS = -1;
void ICHCManualOperationPageFrame::StatusRefreshed()
{
    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int16_t pos = host->HostStatus(ICVirtualHost::XPos).toInt();
    if(pos != oldX)
    {
        oldX = pos;
        ui->xCurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
    }
    pos = host->HostStatus(ICVirtualHost::YPos).toInt();
    if(pos != oldY)
    {
        oldY = pos;
        ui->yCurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
    }
//    pos = host->HostStatus(ICVirtualHost::ZPos).toInt();
//    if(pos != oldZ)
//    {
//        oldZ = pos;
//        ui->zCurrentPos->setText(QString().sprintf("%.1f", pos / 10.0));
//    }
    pos = host->HostStatus(ICVirtualHost::DbgX0).toInt();
    if(pos != oldS)
    {
        oldS = pos;
        ui->speed->setText(QString::number(pos));
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
    ui->xPos->blockSignals(true);
    ui->yPos->blockSignals(true);
//    ui->zPos->blockSignals(true);
    ui->xPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 3 )));
    ui->yPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 3 + 1)));
//    ui->zPos->SetThisIntToThisText(currentMold->MoldParam(static_cast<ICMold::ICMoldParam>(id * 3 + 2)));
//    ui->xPos->setEnabled(true);
//    ui->yPos->setEnabled(true);
//    ui->zPos->setEnabled(true);
    ui->xPos->blockSignals(false);
    ui->yPos->blockSignals(false);
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
    cmd.SetIFVal(info.dir);
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
    cmd.SetIFVal(info.dir);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::on_setButton_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
        return;
    }
    ICPoint p;
    p.pointID = ui->buttonGroup->checkedId();
    p.x = ui->xPos->TransThisTextToThisInt();
    p.y = ui->yPos->TransThisTextToThisInt();
//    p.z = ui->zPos->TransThisTextToThisInt();
    modifyDialog_->StartModify(p);
    OnPointSelected(p.pointID);
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    ui->xPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::XPos).toInt());
//    ui->yPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::YPos).toInt());
//    ui->zPos->SetThisIntToThisText(host->HostStatus(ICVirtualHost::ZPos).toInt());
}

void ICHCManualOperationPageFrame::on_xRun_clicked()
{
    if(ui->buttonGroup->checkedId() < 0)
    {
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
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GY);
    cmd.SetPoint(ui->buttonGroup->checkedId());
//    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}

void ICHCManualOperationPageFrame::on_runButton_toggled(bool checked)
{
    if(checked)
    {
        ui->runButton->setText(tr("No Check"));
    }
    else
    {
        ui->runButton->setText(tr("Check"));
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
