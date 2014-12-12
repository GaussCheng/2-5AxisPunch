#include "icmachineconfigpage.h"
#include "ui_icmachineconfigpage.h"
#include "iclineeditwrapper.h"
#include "icvirtualhost.h"

ICMachineConfigPage::ICMachineConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineConfigPage)
{
    ui->setupUi(this);
    axisWidgets_.append(QList<QWidget*>()<<ui->x1ALabel<<ui->x1MLabel<<ui->xADEdit<<ui->x1SLabel<<ui->xMaxSpeedEdit);
    axisWidgets_.append(QList<QWidget*>()<<ui->y1MLabel<<ui->y1ALabel<<ui->y1SLabel<<ui->yADEdit<<ui->yMaxSpeedEdit);
    axisWidgets_.append(QList<QWidget*>()<<ui->zALabel<<ui->zADEdit<<ui->zMaxSpeedEdit<<ui->zMLabel<<ui->zSLabel);
    axisWidgets_.append(QList<QWidget*>()<<ui->x2ADEdit<<ui->x2ALabel<<ui->x2MaxSpeedEdit<<ui->x2MLabel<<ui->x2SLabel);
    axisWidgets_.append(QList<QWidget*>()<<ui->y2ADEdit<<ui->y2ALabel<<ui->y2MaxSpeedEdit<<ui->y2MLabel<<ui->y2SLabel);
    axisWidgets_.append(QList<QWidget*>()<<ui->aADEdit<<ui->aALabel<<ui->aMaxSpeedEdit<<ui->aMLabel<<ui->aSLabel);
    axisWidgets_.append(QList<QWidget*>()<<ui->bADEdit<<ui->bALabel<<ui->bMaxSpeedEdit<<ui->bMLabel<<ui->bSLabel);
    axisWidgets_.append(QList<QWidget*>()<<ui->cADEdit<<ui->cALabel<<ui->cMaxSpeedEdit<<ui->cMLabel<<ui->cSLabel);
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    ui->xADEdit->SetDecimalPlaces(2);
    ui->xADEdit->setValidator(validator);
    ui->yADEdit->SetDecimalPlaces(2);
    ui->yADEdit->setValidator(validator);
    ui->zADEdit->SetDecimalPlaces(2);
    ui->zADEdit->setValidator(validator);
    ui->x2ADEdit->SetDecimalPlaces(2);
    ui->x2ADEdit->setValidator(validator);
    ui->y2ADEdit->SetDecimalPlaces(2);
    ui->y2ADEdit->setValidator(validator);
    ui->aADEdit->SetDecimalPlaces(2);
    ui->aADEdit->setValidator(validator);
    ui->bADEdit->SetDecimalPlaces(2);
    ui->bADEdit->setValidator(validator);
    ui->cADEdit->SetDecimalPlaces(2);
    ui->cADEdit->setValidator(validator);
    ui->toleranceLineEdit->SetDecimalPlaces(2);
    ui->toleranceLineEdit->setValidator(new QIntValidator(0, 9999, this));
    ui->pullPushDistance->SetDecimalPlaces(1);
    ui->pullPushDistance->setValidator(new QIntValidator(0, 1000, this));
    //    ui->xEarlyEndEdit->SetDecimalPlaces(1);
    //    ui->xEarlyEndEdit->setValidator(validator);
    //    ui->yEarlyEndEdit->SetDecimalPlaces(1);
    //    ui->yEarlyEndEdit->setValidator(validator);
    //    ui->zEarlyEndEdit->SetDecimalPlaces(1);
    //    ui->zEarlyEndEdit->setValidator(validator);

    validator = new QIntValidator(0, 200, this);
    ui->xMaxSpeedEdit->setValidator(validator);
    ui->yMaxSpeedEdit->setValidator(validator);
    ui->zMaxSpeedEdit->setValidator(validator);
    ui->x2MaxSpeedEdit->setValidator(validator);
    ui->y2MaxSpeedEdit->setValidator(validator);
    ui->aMaxSpeedEdit->setValidator(validator);
    ui->bMaxSpeedEdit->setValidator(validator);
    ui->cMaxSpeedEdit->setValidator(validator);

    ICLineEditWrapper* wrapper = new ICLineEditWrapper(ui->xADEdit,
                                                       ICVirtualHost::SM_ACCTIMEX,
                                                       ICLineEditWrapper::System,
                                                       ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->yADEdit,
                                    ICVirtualHost::SM_ACCTIMEY,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->yADEdit,
                                    ICVirtualHost::SM_ACCTIMEY,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->zADEdit,
                                    ICVirtualHost::SM_ACCTIMEZ,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->xMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVX,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->yMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVY,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->zMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVZ,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->toleranceLineEdit,
                                    ICVirtualHost::SM_ALLOWANCE,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->x2ADEdit,
                                    ICVirtualHost::SM_ACCTIMEP,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->y2ADEdit,
                                    ICVirtualHost::SM_ACCTIMEQ,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->aADEdit,
                                    ICVirtualHost::SM_ACCTIMEA,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->bADEdit,
                                    ICVirtualHost::SM_ACCTIMEB,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->cADEdit,
                                    ICVirtualHost::SM_ACCTIMEC,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);

    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->x2MaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVP,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->y2MaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVQ,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->aMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVA,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->bMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVB,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->cMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVC,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->pullPushDistance,
                                    ICVirtualHost::SM_PullPushDistance,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);
//    ui->toleranceLineEdit->hide();
    ui->pullPushDistance->hide();
    ui->label_18->hide();
    ui->label_4->hide();
}

ICMachineConfigPage::~ICMachineConfigPage()
{
    qDeleteAll(wrappers_);
    delete ui;
}

void ICMachineConfigPage::changeEvent(QEvent *e)
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

void ICMachineConfigPage::hideEvent(QHideEvent *e)
{
    qDebug("machine config hide");
    //    ICMold::CurrentMold()->SaveMoldParamsFile();
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    QWidget::hideEvent(e);
}

void ICMachineConfigPage::showEvent(QShowEvent *e)
{
    UpdateAxisDefine_();
    QWidget::showEvent(e);
}

void ICMachineConfigPage::UpdateAxisDefine_()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int currentAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    if(axisDefine_ != currentAxis)
    {
        axisDefine_ = currentAxis;
        for(int i = 0 ; i != axisWidgets_.size(); ++i)
        {
            HideWidgets_(axisWidgets_[i]);
        }


        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[0]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[0]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[1]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[1]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[2]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[2]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[3]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[3]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[4]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[4]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[5]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[5]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[6]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[6]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[7]);
        }
        else
        {
            ShowWidgets_(axisWidgets_[7]);
        }
    }
}

void ICMachineConfigPage::ShowWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->show();
    }
}

void ICMachineConfigPage::HideWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->hide();
    }
}
