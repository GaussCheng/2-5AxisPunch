#include "icmachineconfigpage.h"
#include "ui_icmachineconfigpage.h"
#include "iclineeditwrapper.h"
#include "icvirtualhost.h"

ICMachineConfigPage::ICMachineConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineConfigPage)
{
    ui->setupUi(this);
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
    ui->toleranceLineEdit->setValidator(validator);
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
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    QWidget::hideEvent(e);
}
