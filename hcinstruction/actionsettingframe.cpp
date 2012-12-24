#include "actionsettingframe.h"
#include "ui_actionsettingframe.h"

#include "routesettingdialog.h"

#include "iccommandprocessor.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"

#include "icparameterssave.h"

ActionSettingFrame::ActionSettingFrame(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ActionSettingFrame),
    axisDefine_(-1)
{
    ui->setupUi(this);

    for(int i = 0; i != 8; ++i)
    {
        posValidators_[i].setBottom(0);
    }
    InitInterface();
    axisWidgets_.append(QList<QWidget*>()<<ui->gxButton<<ui->x1DelayLineEdit<<ui->x1PosLineEdit<<ui->x1SpeedLineEdit<<ui->x1Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gyButton<<ui->y1DelayLineEdit<<ui->y1PosLineEdit<<ui->y1SpeedLineEdit<<ui->y1Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gzButton<<ui->zDelayLineEdit<<ui->zPosLineEdit<<ui->zSpeedLineEdit<<ui->zBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gPButton<<ui->x2DelayLineEdit<<ui->x2PosLineEdit<<ui->x2SpeedLineEdit<<ui->x2Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gQButton<<ui->y2DelayLineEdit<<ui->y2PosLineEdit<<ui->y2SpeedLineEdit<<ui->y2Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gAButton<<ui->aDelayLineEdit<<ui->aPosLineEdit<<ui->aSpeedLineEdit<<ui->aBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gBButton<<ui->bDelayLineEdit<<ui->bPosLineEdit<<ui->bSpeedLineEdit<<ui->bBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gCButton<<ui->cDelayLineEdit<<ui->cPosLineEdit<<ui->cSpeedLineEdit<<ui->cBox);
#ifdef Q_WS_X11
    UpdateAxisDefine_();
#endif
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
{     /************************BUG#119*******************************/
    QIntValidator * validator = new QIntValidator(0, 30000, this);

    ui->x1DelayLineEdit->SetDecimalPlaces(2);
    ui->x1DelayLineEdit->setValidator(validator);
    ui->x1PosLineEdit->SetDecimalPlaces(1);
    ui->x1PosLineEdit->setValidator(posValidators_ + 0);

    ui->y1DelayLineEdit->SetDecimalPlaces(2);
    ui->y1DelayLineEdit->setValidator(validator);
    ui->y1PosLineEdit->SetDecimalPlaces(1);
    ui->y1PosLineEdit->setValidator(posValidators_ + 1);

    ui->zDelayLineEdit->SetDecimalPlaces(2);
    ui->zDelayLineEdit->setValidator(validator);
    ui->zPosLineEdit->SetDecimalPlaces(1);
    ui->zPosLineEdit->setValidator(posValidators_ + 2);

#ifdef HC_8AXIS
    ui->x2DelayLineEdit->SetDecimalPlaces(2);
    ui->x2DelayLineEdit->setValidator(validator);
    ui->x2PosLineEdit->SetDecimalPlaces(1);
    ui->x2PosLineEdit->setValidator(posValidators_ + 3);

    ui->y2DelayLineEdit->SetDecimalPlaces(2);
    ui->y2DelayLineEdit->setValidator(validator);
    ui->y2PosLineEdit->SetDecimalPlaces(1);
    ui->y2PosLineEdit->setValidator(posValidators_ + 4);

    ui->aDelayLineEdit->SetDecimalPlaces(2);
    ui->aDelayLineEdit->setValidator(validator);
    ui->aPosLineEdit->SetDecimalPlaces(1);
    ui->aPosLineEdit->setValidator(posValidators_ + 5);

    ui->bDelayLineEdit->SetDecimalPlaces(2);
    ui->bDelayLineEdit->setValidator(validator);
    ui->bPosLineEdit->SetDecimalPlaces(1);
    ui->bPosLineEdit->setValidator(posValidators_ + 6);

    ui->cDelayLineEdit->SetDecimalPlaces(2);
    ui->cDelayLineEdit->setValidator(validator);
    ui->cPosLineEdit->SetDecimalPlaces(1);
    ui->cPosLineEdit->setValidator(posValidators_ + 7);

    QIntValidator *speed = new QIntValidator(0, 100, this);
    ui->x1SpeedLineEdit->SetDecimalPlaces(0);
    ui->x1SpeedLineEdit->setValidator(speed);
    ui->y1SpeedLineEdit->SetDecimalPlaces(0);
    ui->y1SpeedLineEdit->setValidator(speed);
    ui->zSpeedLineEdit->SetDecimalPlaces(0);
    ui->zSpeedLineEdit->setValidator(speed);
    ui->x2SpeedLineEdit->SetDecimalPlaces(0);
    ui->x2SpeedLineEdit->setValidator(speed);
    ui->y2SpeedLineEdit->SetDecimalPlaces(0);
    ui->y2SpeedLineEdit->setValidator(speed);
    ui->aSpeedLineEdit->SetDecimalPlaces(0);
    ui->aSpeedLineEdit->setValidator(speed);
    ui->bSpeedLineEdit->SetDecimalPlaces(0);
    ui->bSpeedLineEdit->setValidator(speed);
    ui->cSpeedLineEdit->SetDecimalPlaces(0);
    ui->cSpeedLineEdit->setValidator(speed);
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
    ui->gxButton->setChecked(false);
    ui->gyButton->setChecked(false);
    ui->gzButton->setChecked(false);
    ui->gPButton->setChecked(false);
    ui->gQButton->setChecked(false);
    ui->gAButton->setChecked(false);
    ui->gBButton->setChecked(false);
    ui->gCButton->setChecked(false);
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefresh()));
}

void ActionSettingFrame::showEvent(QShowEvent *e)
{
    UpdateAxisDefine_();
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    posMaxs_[0] = host->SystemParameter(ICVirtualHost::SYS_X_Maxium).toInt();
    posMaxs_[1] = host->SystemParameter(ICVirtualHost::SYS_Y_Maxium).toInt();
    posMaxs_[2] = host->SystemParameter(ICVirtualHost::SYS_Z_Maxium).toInt();
    posMaxs_[3] = host->SystemParameter(ICVirtualHost::SYS_P_Maxium).toInt();
    posMaxs_[4] = host->SystemParameter(ICVirtualHost::SYS_Q_Maxium).toInt();
    posMaxs_[5] = host->SystemParameter(ICVirtualHost::SYS_A_Maxium).toInt();
    posMaxs_[6] = host->SystemParameter(ICVirtualHost::SYS_B_Maxium).toInt();
    posMaxs_[7] = host->SystemParameter(ICVirtualHost::SYS_C_Maxium).toInt();
    for(int i = 0; i != 8; ++i)
    {
        posValidators_[i].setTop(posMaxs_[i]);
    }
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefresh()));
}

void ActionSettingFrame::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
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
    if(ui->gxButton->isChecked() && (!ui->gxButton->isHidden()))
    {
        //        if(ui->gxComboBox->currentIndex() == 0)
        //        {
        //            item.SetAction(ICMold::ACTMAINFORWARD);
        //        }
        //        else
        //        {
        //            item.SetAction(ICMold::ACTMAINBACKWARD);
        //        }
        if(ui->x1Box->isHidden())
        {
            item.SetAction(ICMold::GX);
            item.SetPos(ui->x1PosLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->x1SpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->x1Box->currentIndex() == 0 ? ICMold::ACTMAINFORWARD : ICMold::ACTMAINBACKWARD);
        }
        item.SetDVal(ui->x1DelayLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gyButton->isChecked() && (!ui->gyButton->isHidden()))
    {
        if(ui->y1Box->isHidden())
        {
            item.SetAction(ICMold::GY);
            item.SetPos(ui->y1PosLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->y1SpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->y1Box->currentIndex() == 0 ? ICMold::ACTMAINUP : ICMold::ACTMAINDOWN);
        }
        item.SetDVal(ui->y1DelayLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gzButton->isChecked() && (!ui->gzButton->isHidden()))
    {
        if(ui->zBox->isHidden())
        {
            item.SetAction(ICMold::GZ);
            item.SetPos(ui->zPosLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->zSpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->zBox->currentIndex() == 0 ? ICMold::ACTCOMEIN : ICMold::ACTGOOUT);
        }
        item.SetDVal(ui->zDelayLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }

#ifdef HC_8AXIS
    if(ui->gPButton->isChecked() && (!ui->gPButton->isHidden()))
    {
        if(ui->x2Box->isHidden())
        {
            item.SetAction(ICMold::GP);
            item.SetPos(ui->x2PosLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->x2SpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->x2Box->currentIndex() == 0 ? ICMold::ACTVICEFORWARD : ICMold::ACTVICEBACKWARD);
        }
        item.SetDVal(ui->x2DelayLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gQButton->isChecked() && (!ui->gQButton->isHidden()))
    {
        if(ui->y2Box->isHidden())
        {
            item.SetAction(ICMold::GQ);
            item.SetPos(ui->y2PosLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->y2SpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->y2Box->currentIndex() == 0 ? ICMold::ACTVICEUP : ICMold::ACTVICEDOWN);
        }
        item.SetDVal(ui->y2DelayLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gAButton->isChecked() && (!ui->gAButton->isHidden()))
    {
        if(ui->aBox->isHidden())
        {
            item.SetAction(ICMold::GA);
            item.SetPos(ui->aPosLineEdit->TransThisTextToThisInt());
            item.SetDVal(ui->aDelayLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->aSpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->aBox->currentIndex() == 0 ? ICMold::ACT_PoseVert2 : ICMold::ACT_PoseHori2);
        }
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gBButton->isChecked() && (!ui->gBButton->isHidden()))
    {
        item.SetAction(ICMold::GB);
        item.SetPos(ui->bPosLineEdit->TransThisTextToThisInt());
        item.SetDVal(ui->bDelayLineEdit->TransThisTextToThisInt());
        item.SetSVal(ui->bSpeedLineEdit->TransThisTextToThisInt());
        item.SetIFVal(0);
        item.SetIFPos(0);
        ret.append(item);
    }
    if(ui->gCButton->isChecked() && (!ui->gCButton->isHidden()))
    {
        if(ui->cBox->isHidden())
        {
            item.SetAction(ICMold::GC);
            item.SetPos(ui->cPosLineEdit->TransThisTextToThisInt());
            item.SetDVal(ui->cDelayLineEdit->TransThisTextToThisInt());
            item.SetSVal(ui->cSpeedLineEdit->TransThisTextToThisInt());
        }
        else
        {
            item.SetAction(ui->cBox->currentIndex() == 0 ? ICMold::ACTPOSEVERT : ICMold::ACTPOSEHORI);
        }
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
        ui->x1Box->setEnabled(true);
    }
    else
    {
        //        ui->gxComboBox->setEnabled(false);
        ui->x1PosLineEdit->setEnabled(false);
        ui->x1DelayLineEdit->setEnabled(false);
        ui->x1SpeedLineEdit->setEnabled(false);
        ui->x1Box->setEnabled(false);
    }
}

void ActionSettingFrame::on_gyButton_toggled(bool checked)
{
    if(checked)
    {
        ui->y1PosLineEdit->setEnabled(true);
        ui->y1DelayLineEdit->setEnabled(true);
        ui->y1SpeedLineEdit->setEnabled(true);
        ui->y1Box->setEnabled(true);
    }
    else
    {
        ui->y1PosLineEdit->setEnabled(false);
        ui->y1DelayLineEdit->setEnabled(false);
        ui->y1SpeedLineEdit->setEnabled(false);
        ui->y1Box->setEnabled(false);
    }
}

void ActionSettingFrame::on_gzButton_toggled(bool checked)
{
    if(checked)
    {
        ui->zPosLineEdit->setEnabled(true);
        ui->zDelayLineEdit->setEnabled(true);
        ui->zSpeedLineEdit->setEnabled(true);
        ui->zBox->setEnabled(true);
    }
    else
    {
        ui->zPosLineEdit->setEnabled(false);
        ui->zDelayLineEdit->setEnabled(false);
        ui->zSpeedLineEdit->setEnabled(false);
        ui->zBox->setEnabled(false);
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
        ui->x2Box->setEnabled(true);
    }
    else
    {
        ui->x2PosLineEdit->setEnabled(false);
        ui->x2DelayLineEdit->setEnabled(false);
        ui->x2SpeedLineEdit->setEnabled(false);
        ui->x2Box->setEnabled(false);
    }
}

void ActionSettingFrame::on_gQButton_toggled(bool checked)
{
    if(checked)
    {
        ui->y2PosLineEdit->setEnabled(true);
        ui->y2DelayLineEdit->setEnabled(true);
        ui->y2SpeedLineEdit->setEnabled(true);
        ui->y2Box->setEnabled(true);
    }
    else
    {
        ui->y2PosLineEdit->setEnabled(false);
        ui->y2DelayLineEdit->setEnabled(false);
        ui->y2SpeedLineEdit->setEnabled(false);
        ui->y2Box->setEnabled(false);
    }
}

void ActionSettingFrame::on_gAButton_toggled(bool checked)
{
    if(checked)
    {
        ui->aPosLineEdit->setEnabled(true);
        ui->aDelayLineEdit->setEnabled(true);
        ui->aSpeedLineEdit->setEnabled(true);
        ui->aBox->setEnabled(true);
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
        ui->bBox->setEnabled(true);
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
        ui->cBox->setEnabled(true);
    }
    else
    {
        ui->cPosLineEdit->setEnabled(false);
        ui->cDelayLineEdit->setEnabled(false);
        ui->cSpeedLineEdit->setEnabled(false);
    }
}

void ActionSettingFrame::ShowWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->show();;
    }
}

void ActionSettingFrame::HideWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->hide();;
    }
}

void ActionSettingFrame::UpdateAxisDefine_()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int currentAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
/*********************BUG#125*********************************/
        if(ICParametersSave::Instance()->IsSingleArm())
        {
            ui->gPButton->setCheckable(false);
            ui->gQButton->setCheckable(false);
        }
/*************************************************************/
    if(axisDefine_ != currentAxis)
    {
        axisDefine_ = currentAxis;
//        for(int i = 0 ; i != axisWidgets_.size(); ++i)
//        {
//            HideWidgets_(axisWidgets_[i]);
//        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gxButton->setChecked(false);
            HideWidgets_(axisWidgets_[0]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[0]);
            ui->x1PosLineEdit->hide();
            ui->x1SpeedLineEdit->hide();
            //            ui->x1pLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[0]);
            ui->x1Box->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gyButton->setChecked(false);
            HideWidgets_(axisWidgets_[1]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[1]);
            ui->y1PosLineEdit->hide();
            ui->y1SpeedLineEdit->hide();
            //            ui->y1pLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[1]);
            ui->y1Box->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gzButton->setChecked(false);
            HideWidgets_(axisWidgets_[2]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[2]);
            ui->zPosLineEdit->hide();
            ui->zSpeedLineEdit->hide();
            //            ui->zpLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[2]);
            ui->zBox->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gPButton->setChecked(false);
            HideWidgets_(axisWidgets_[3]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[3]);
            ui->x2PosLineEdit->hide();
            ui->x2SpeedLineEdit->hide();
            //            ui->x2pLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[3]);
            ui->x2Box->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gQButton->setChecked(false);
            HideWidgets_(axisWidgets_[4]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[4]);
            ui->y2PosLineEdit->hide();
            ui->y2SpeedLineEdit->hide();
            //            ui->y2pLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[4]);
            ui->y2Box->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gAButton->setChecked(false);
            HideWidgets_(axisWidgets_[5]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[5]);
            ui->aPosLineEdit->hide();
            ui->aSpeedLineEdit->hide();
            //            ui->apLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[5]);
            ui->aBox->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gBButton->setChecked(false);
            HideWidgets_(axisWidgets_[6]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[6]);
            ui->bPosLineEdit->hide();
            ui->bSpeedLineEdit->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[6]);
            ui->bBox->hide();
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_None)
        {
            ui->gCButton->setChecked(false);
            HideWidgets_(axisWidgets_[7]);
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[7]);
            ui->cPosLineEdit->hide();
            ui->cSpeedLineEdit->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[7]);
            ui->cBox->hide();
        }
    }

}
#endif

void ActionSettingFrame::KeyToActionCheck(int key)
{
    switch(key)
    {
    case ICKeyboard::VFB_X1Add:
    case ICKeyboard::VFB_X1Sub:
        ui->x1Box->setCurrentIndex(key == ICKeyboard::VFB_X1Add ? 0:1);
        ui->gxButton->setChecked(true);
        break;
    case ICKeyboard::VFB_Y1Add:
    case ICKeyboard::VFB_Y1Sub:
        ui->y1Box->setCurrentIndex(key == ICKeyboard::VFB_Y1Add ? 1:0);
        ui->gyButton->setChecked(true);
        break;
    case ICKeyboard::VFB_ZAdd:
    case ICKeyboard::VFB_ZSub:
        ui->zBox->setCurrentIndex(key == ICKeyboard::VFB_ZAdd ? 1:0);
        ui->gzButton->setChecked(true);
        break;
    case ICKeyboard::VFB_X2Add:
    case ICKeyboard::VFB_X2Sub:
        ui->x2Box->setCurrentIndex(key == ICKeyboard::VFB_X2Add ? 0:1);
        ui->gPButton->setChecked(true);
        break;
    case ICKeyboard::VFB_Y2Add:
    case ICKeyboard::VFB_Y2Sub:
        ui->y2Box->setCurrentIndex(key == ICKeyboard::VFB_Y2Add ?1:0);
        ui->gQButton->setChecked(true);
        break;
    case ICKeyboard::VFB_AAdd:
    case ICKeyboard::VFB_ASub:
        ui->aBox->setCurrentIndex(key == ICKeyboard::VFB_AAdd ? 0:1);
        ui->gAButton->setChecked(true);
        break;
    case ICKeyboard::VFB_BAdd:
    case ICKeyboard::VFB_BSub:
        ui->bBox->setCurrentIndex(key == ICKeyboard::VFB_BAdd ? 0:1);
        ui->gBButton->setChecked(true);
        break;
    case ICKeyboard::VFB_CAdd:
    case ICKeyboard::VFB_CSub:
        ui->cBox->setCurrentIndex(key == ICKeyboard::VFB_CAdd ? 0:1);
        ui->gCButton->setChecked(true);
        break;
    case ICKeyboard::VFB_Pose_Horizontal:
    case ICKeyboard::VFB_Pose_Vertical:
        ui->cBox->setCurrentIndex(key == ICKeyboard::VFB_Pose_Horizontal ? 1:0);
        ui->gCButton->setChecked(true);
        break;
    default:QMessageBox::information(this,"hello","world");break;
    }
}
