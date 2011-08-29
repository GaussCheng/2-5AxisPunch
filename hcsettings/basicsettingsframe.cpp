#include "basicsettingsframe.h"
#include "ui_basicsettingsframe.h"

#include "icparameterssave.h"
#include "iclineeditwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"

BasicSettingsFrame::BasicSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BasicSettingsFrame)
{
    ui->setupUi(this);

    InitInterface();
}

BasicSettingsFrame::~BasicSettingsFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void BasicSettingsFrame::InitInterface()
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    ui->xADEdit->SetDecimalPlaces(2);
    ui->xADEdit->setValidator(validator);
    ui->yADEdit->SetDecimalPlaces(2);
    ui->yADEdit->setValidator(validator);
    ui->zADEdit->SetDecimalPlaces(2);
    ui->zADEdit->setValidator(validator);
    ui->toleranceLineEdit->SetDecimalPlaces(2);
    ui->toleranceLineEdit->setValidator(validator);

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
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->yMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVY,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->zMaxSpeedEdit,
                                    ICVirtualHost::SM_LMTVZ,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->toleranceLineEdit,
                                    ICVirtualHost::SM_ALLOWANCE,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::TwoFraction);
    wrappers_.append(wrapper);
}

void BasicSettingsFrame::hideEvent(QHideEvent *e)
{
    qDebug("base setting hide");
//    ICMold::CurrentMold()->SaveMoldParamsFile();
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
    ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    QFrame::hideEvent(e);
}

void BasicSettingsFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

//void BasicSettingsFrame::on_languageComboBox_activated(int index)
//{
//    ICParametersSave* paraSave = ICParametersSave::Instance();
//    switch(index)
//    {
//    case 0:
//        paraSave->SetLanguage(QLocale::Chinese);
//        break;
//    case 1:
//        paraSave->SetLanguage(QLocale::English);
//        break;
//    default:
//        break;
//    }
//}
