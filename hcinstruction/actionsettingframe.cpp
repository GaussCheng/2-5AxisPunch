#include "actionsettingframe.h"
#include "ui_actionsettingframe.h"

#include "routesettingdialog.h"

#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"

ActionSettingFrame::ActionSettingFrame(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ActionSettingFrame)
{
    ui->setupUi(this);

    InitInterface();
//    ui->x1SpeedLineEdit->hide();
}

ActionSettingFrame::~ActionSettingFrame()
{
    delete ui;
}

void ActionSettingFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ActionSettingFrame::InitInterface()
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);

    ui->x1DelayLineEdit->SetDecimalPlaces(1);
    ui->x1DelayLineEdit->setValidator(validator);
    ui->x1PosLineEdit->SetDecimalPlaces(1);
    ui->x1PosLineEdit->setValidator(validator);

    ui->y1DelayLineEdit->SetDecimalPlaces(1);
    ui->y1DelayLineEdit->setValidator(validator);
    ui->y1PosLineEdit->SetDecimalPlaces(1);
    ui->y1PosLineEdit->setValidator(validator);

    ui->zDelayLineEdit->SetDecimalPlaces(1);
    ui->zDelayLineEdit->setValidator(validator);
    ui->zPosLineEdit->SetDecimalPlaces(1);
    ui->zPosLineEdit->setValidator(validator);

#ifdef HC_8AXIS
    ui->x2DelayLineEdit->SetDecimalPlaces(1);
    ui->x2DelayLineEdit->setValidator(validator);
    ui->x2PosLineEdit->SetDecimalPlaces(1);
    ui->x2PosLineEdit->setValidator(validator);

    ui->y2DelayLineEdit->SetDecimalPlaces(1);
    ui->y2DelayLineEdit->setValidator(validator);
    ui->y2PosLineEdit->SetDecimalPlaces(1);
    ui->y2PosLineEdit->setValidator(validator);

    ui->aDelayLineEdit->SetDecimalPlaces(1);
    ui->aDelayLineEdit->setValidator(validator);
    ui->aPosLineEdit->SetDecimalPlaces(1);
    ui->aPosLineEdit->setValidator(validator);

    ui->bDelayLineEdit->SetDecimalPlaces(1);
    ui->bDelayLineEdit->setValidator(validator);
    ui->bPosLineEdit->SetDecimalPlaces(1);
    ui->bPosLineEdit->setValidator(validator);

    ui->cDelayLineEdit->SetDecimalPlaces(1);
    ui->cDelayLineEdit->setValidator(validator);
    ui->cPosLineEdit->SetDecimalPlaces(1);
    ui->cPosLineEdit->setValidator(validator);

    ui->x1SpeedLineEdit->SetDecimalPlaces(0);
    ui->x1SpeedLineEdit->setValidator(validator);
    ui->y1SpeedLineEdit->SetDecimalPlaces(0);
    ui->y1SpeedLineEdit->setValidator(validator);
    ui->zSpeedLineEdit->SetDecimalPlaces(0);
    ui->zSpeedLineEdit->setValidator(validator);
    ui->x2SpeedLineEdit->SetDecimalPlaces(0);
    ui->x2SpeedLineEdit->setValidator(validator);
    ui->y2SpeedLineEdit->SetDecimalPlaces(0);
    ui->y2SpeedLineEdit->setValidator(validator);
    ui->aSpeedLineEdit->SetDecimalPlaces(0);
    ui->aSpeedLineEdit->setValidator(validator);
    ui->bSpeedLineEdit->SetDecimalPlaces(0);
    ui->bSpeedLineEdit->setValidator(validator);
    ui->cSpeedLineEdit->SetDecimalPlaces(0);
    ui->cSpeedLineEdit->setValidator(validator);
#endif


}

void ActionSettingFrame::on_inputButton_clicked()
{
//    if(ICVirtualHost::GlobalVirtualHost()->IsOutputOn(6))
//    {
//        ui->gxComboBox->setCurrentIndex(0);
//    }
//    else
//    {
//        ui->gxComboBox->setCurrentIndex(1);
//    }
    ui->x1PosLineEdit->setText(QString().sprintf("%.1f", oXP_ / 10.0));
    ui->y1PosLineEdit->setText(QString().sprintf("%.1f", oYP_ / 10.0));
    ui->zPosLineEdit->setText(QString().sprintf("%.1f", oZP_ / 10.0));
#ifdef HC_8AXIS
    ui->x2PosLineEdit->setText(QString().sprintf("%.1f", oX2P_ / 10.0));
    ui->y2PosLineEdit->setText(QString().sprintf("%.1f", oY2P_ / 10.0));
    ui->aPosLineEdit->setText(QString().sprintf("%.1f", oAP_ / 10.0));
    ui->bPosLineEdit->setText(QString().sprintf("%.1f", oBP_ / 10.0));
    ui->cPosLineEdit->setText(QString().sprintf("%.1f", oCP_ / 10.0));
#endif

    //do someting
}

void ActionSettingFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefresh()));
}

void ActionSettingFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefresh()));
}

void ActionSettingFrame::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    //    ui->x1PosLineEdit->setText(QString().sprintf("%.1f", oXP_ / 10.0));
    //    ui->y1PosLineEdit->setText(QString().sprintf("%.1f", oYP_ / 10.0));
    //    ui->zPosLineEdit->setText(QString().sprintf("%.1f", oZP_ / 10.0));
    //    ui->settedSpeedLineEdit->setText(ui->speedLabel->text());
    //    ui->gestrueSettedLabel->setText(ui->gestrueLabel->text());
    //    ui->x2SettedLabel->setText(ui->x2Label->text());
    //    ui->y2SettedLabel->setText(ui->y2Label->text());
    //    ICMoldItem item;
    //    int action;
    //    foreach(item, items)
    //    {
    //        if(item.IsAction())
    //        {
    //            action = item.Action();
    //            if(action >= ICMold::G01 && action <= ICMold::GXZ)
    //            {
    //                ui->x1PosLineEdit->SetThisIntToThisText(item.X());
    //                ui->y1PosLineEdit->SetThisIntToThisText(item.Y());
    //                ui->zPosLineEdit->SetThisIntToThisText(item.Z());
    //                ui->settedSpeedLineEdit->SetThisIntToThisText(item.SVal());
    //                ui->x1DelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTPOSEHORI)
    //            {
    //                ui->gestrueSettedLabel->setText(poseH);
    //                ui->gestureDelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTPOSEVERT)
    //            {
    //                ui->gestrueLabel->setText(poseV);
    //                ui->gestureDelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTVICEUP)
    //            {
    //                ui->y2SettedLabel->setText(sAU);
    //                ui->y2DelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTVICEDOWN)
    //            {
    //                ui->y2SettedLabel->setText(sAD);
    //                ui->y2DelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTVICEFORWARD)
    //            {
    //                ui->x2SettedLabel->setText(sAF);
    //                ui->x2DelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //            else if(action == ICMold::ACTVICEBACKWARD)
    //            {
    //                ui->x2SettedLabel->setText(sAB);
    //                ui->x2DelayLineEdit->SetThisIntToThisText(item.DVal());
    //            }
    //        }
    //    }
    //    ui->
}

void ActionSettingFrame::StatusRefresh()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int pos = host->HostStatus(ICVirtualHost::YPos).toInt();
    if(pos != oYP_)
    {
        oYP_ = pos;
    }
    pos = host->HostStatus(ICVirtualHost::ZPos).toInt();
    if(pos != oZP_)
    {
        oZP_ = pos;
    }

    pos = host->HostStatus(ICVirtualHost::XPos).toInt();
    if(pos != oXP_)
    {
        oXP_ = pos;
    }
#ifdef HC_8AXIS
    pos = host->HostStatus(ICVirtualHost::PPos).toInt();
    if(pos != oX2P_)
    {
        oX2P_ = pos;
    }

    pos = host->HostStatus(ICVirtualHost::QPos).toInt();
    if(pos != oY2P_)
    {
        oY2P_ = pos;
    }

    pos = host->HostStatus(ICVirtualHost::APos).toInt();
    if(pos != oAP_)
    {
        oAP_ = pos;
    }

    pos = host->HostStatus(ICVirtualHost::BPos).toInt();
    if(pos != oBP_)
    {
        oBP_ = pos;
    }

    pos = host->HostStatus(ICVirtualHost::CPos).toInt();
    if(pos != oCP_)
    {
        oCP_ = pos;
    }
#endif
}

QList<ICMoldItem> ActionSettingFrame::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    if(ui->gxButton->isChecked())
    {
//        if(ui->gxComboBox->currentIndex() == 0)
//        {
//            item.SetAction(ICMold::ACTMAINFORWARD);
//        }
//        else
//        {
//            item.SetAction(ICMold::ACTMAINBACKWARD);
//        }
        item.SetAction(ICMold::GX);
        item.SetPos(ui->x1PosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->x1DelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->x1SpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gyButton->isChecked())
    {
        item.SetAction(ICMold::GY);
        item.SetPos(ui->y1PosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->y1DelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->y1SpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gzButton->isChecked())
    {
        item.SetAction(ICMold::GZ);
        item.SetPos(ui->zPosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->zDelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->zSpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }

#ifdef HC_8AXIS
    if(ui->gPButton->isChecked())
    {
        item.SetAction(ICMold::GP);
        item.SetPos(ui->x2PosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->x2DelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->x2SpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gQButton->isChecked())
    {
        item.SetAction(ICMold::GQ);
        item.SetPos(ui->y2PosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->y2DelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->y2SpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gAButton->isChecked())
    {
        item.SetAction(ICMold::GA);
        item.SetPos(ui->aPosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->aDelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->aSpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gBButton->isChecked())
    {
        item.SetAction(ICMold::GB);
        item.SetPos(ui->bPosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->bDelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->bSpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gCButton->isChecked())
    {
        item.SetAction(ICMold::GC);
        item.SetPos(ui->cPosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->cDelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->cSpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
#endif

    return ret;
}

void ActionSettingFrame::on_gxButton_toggled(bool checked)
{
    if(checked)
    {
//        ui->gxComboBox->setEnabled(true);
        ui->x1PosLineEdit->setEnabled(true);
        ui->x1DelayLineEdit->setEnabled(true);
        ui->x1SpeedLineEdit->setEnabled(true);
    }
    else
    {
//        ui->gxComboBox->setEnabled(false);
        ui->x1PosLineEdit->setEnabled(false);
        ui->x1DelayLineEdit->setEnabled(false);
        ui->x1SpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gyButton_toggled(bool checked)
{
    if(checked)
    {
        ui->y1PosLineEdit->setEnabled(true);
        ui->y1DelayLineEdit->setEnabled(true);
        ui->y1SpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->y1PosLineEdit->setEnabled(false);
        ui->y1DelayLineEdit->setEnabled(false);
        ui->y1SpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gzButton_toggled(bool checked)
{
    if(checked)
    {
        ui->zPosLineEdit->setEnabled(true);
        ui->zDelayLineEdit->setEnabled(true);
        ui->zSpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->zPosLineEdit->setEnabled(false);
        ui->zDelayLineEdit->setEnabled(false);
        ui->zSpeedLineEdit->setEnabled(false);
    }
}

#ifdef HC_8AXIS
void ActionSettingFrame::on_gPButton_toggled(bool checked)
{
    if(checked)
    {
        ui->x2PosLineEdit->setEnabled(true);
        ui->x2DelayLineEdit->setEnabled(true);
        ui->x2SpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->x2PosLineEdit->setEnabled(false);
        ui->x2DelayLineEdit->setEnabled(false);
        ui->x2SpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gQButton_toggled(bool checked)
{
    if(checked)
    {
        ui->y2PosLineEdit->setEnabled(true);
        ui->y2DelayLineEdit->setEnabled(true);
        ui->y2SpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->y2PosLineEdit->setEnabled(false);
        ui->y2DelayLineEdit->setEnabled(false);
        ui->y2SpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gAButton_toggled(bool checked)
{
    if(checked)
    {
        ui->aPosLineEdit->setEnabled(true);
        ui->aDelayLineEdit->setEnabled(true);
        ui->aSpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->aPosLineEdit->setEnabled(false);
        ui->aDelayLineEdit->setEnabled(false);
        ui->aSpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gBButton_toggled(bool checked)
{
    if(checked)
    {
        ui->bPosLineEdit->setEnabled(true);
        ui->bDelayLineEdit->setEnabled(true);
        ui->bSpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->bPosLineEdit->setEnabled(false);
        ui->bDelayLineEdit->setEnabled(false);
        ui->bSpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::on_gCButton_toggled(bool checked)
{
    if(checked)
    {
        ui->cPosLineEdit->setEnabled(true);
        ui->cDelayLineEdit->setEnabled(true);
        ui->cSpeedLineEdit->setEnabled(true);
    }
    else
    {
        ui->cPosLineEdit->setEnabled(false);
        ui->cDelayLineEdit->setEnabled(false);
        ui->cSpeedLineEdit->setEnabled(false);
    }
}
#endif
