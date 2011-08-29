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

//    wrapper = new ICLineEditWrapper(ui->xEarlyEndEdit,
//                                    ICVirtualHost::SM_XEarlyEnd,
//                                    ICLineEditWrapper::System,
//                                    ICLineEditWrapper::OneFraction);
//    wrappers_.append(wrapper);

//    wrapper = new ICLineEditWrapper(ui->yEarlyEndEdit,
//                                    ICVirtualHost::SM_YEarlyEnd,
//                                    ICLineEditWrapper::System,
//                                    ICLineEditWrapper::OneFraction);
//    wrappers_.append(wrapper);

//    wrapper = new ICLineEditWrapper(ui->zEarlyEndEdit,
//                                    ICVirtualHost::SM_ZEarlyEnd,
//                                    ICLineEditWrapper::System,
//                                    ICLineEditWrapper::OneFraction);
//    wrappers_.append(wrapper);
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
