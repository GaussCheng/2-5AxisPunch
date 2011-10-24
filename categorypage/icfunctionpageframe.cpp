#include "icfunctionpageframe.h"
#include "ui_icfunctionpageframe.h"

#include "icsignalsettingframe.h"
#include "icproductsettingframe.h"
#include "icinstructionpageframe.h"
//#include "icsecuritypointsettingframe.h"
//#include "icstacksettingpageframe.h"
#include "ichcsystemsettingsframe.h"
#include "icsystemsettingframe.h"
#include "icmachinestructpage.h"

#include "ichcsettingsframe.h"
#include "ichcdetectionframe.h"
#include "ichcproductsettingframe.h"
#include "ichcstackedsettingsframe.h"
#include "icupdatesystem.h"
#include "icmachineconfigpage.h"

#include "icprogramheadframe.h"
#include "icparameterssave.h"

#include <QDebug>

class ICSettingFunctionFactory
{
public:
    static QWidget * CreatePage(
            const QWidget * clickedButton = NULL, ICFunctionPageFrame * functionPage = NULL, QWidget *parent = NULL)
    {
        if(clickedButton == functionPage->ui->signalSettingWidget)
        {
//            return new ICSignalSettingFrame(parent);
            return new ICHCDetectionFrame(parent);
        }
        else if(clickedButton == functionPage->ui->productSettingWidget)
        {
//            return new ICProductSettingFrame(parent);
            return new ICHCProductSettingFrame(parent);
        }
        else if(clickedButton == functionPage->ui->maintainWidget)
        {
            return new ICUpdateSystem(parent);
        }
        else if(clickedButton == functionPage->ui->securityPointSettingWidget)
        {
            return new ICHCSettingsFrame(parent);
        }
        else if(clickedButton == functionPage->ui->stackSettingWidget)
        {
//            return new ICStackSettingPageFrame(parent);
            return new ICHCStackedSettingsFrame(parent);
        }
        else if(clickedButton == functionPage->ui->machineStructConfig)
        {
            return new ICMachineStructPage(parent);
        }
        else if(clickedButton == functionPage->ui->machineConfigSettingWidget)
        {
            return new ICMachineConfigPage(parent);
        }
        else
        {
            return NULL;
        }
    }
};

ICFunctionPageFrame::ICFunctionPageFrame(QWidget *parent, QStackedLayout * backFrameCenterStackedLayout) :
    ICCategoryPageBaseFrame(parent),
    ui(new Ui::ICFunctionPageFrame),
    backFrameCenterStackedLayout_(NULL)
{
    ui->setupUi(this);

    backFrameCenterStackedLayout_ = backFrameCenterStackedLayout;

    InitSettingPage();
    UpdateTranslate();
    InitSignal();

//    backFrameCenterStackedLayout_->setCurrentWidget(this);
}

ICFunctionPageFrame::~ICFunctionPageFrame()
{
    delete ui;
}

void ICFunctionPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            UpdateTranslate();
        }
        break;
    default:
        break;
    }
}

void ICFunctionPageFrame::ShowFunctionSelectPage()
{
    backFrameCenterStackedLayout_->setCurrentWidget(this);
}

void ICFunctionPageFrame::InitSettingPage()
{
    systemSettings_ = new ICHCSystemSettingsFrame;
    settingButtonToPage_.insert(ui->systemSettingWidget, systemSettings_);
    backFrameCenterStackedLayout_->addWidget(systemSettings_);


    ui->signalSettingWidget->setIcon(QIcon(":/resources/signal.png"));
    ui->productSettingWidget->setIcon(QIcon(":/resources/product.png"));
    ui->maintainWidget->setIcon(QIcon(":/resources/meter.png"));
    ui->securityPointSettingWidget->setIcon(QIcon(":/resources/security-point.png"));
    ui->stackSettingWidget->setIcon(QIcon(":/resources/stacked.png"));
    ui->systemSettingWidget->setIcon(QIcon(":/resources/system.png"));
    ui->machineStructConfig->setIcon(QIcon(":/resources/machineStruct.png"));
    ui->machineConfigSettingWidget->setIcon(QIcon(":/resources/machine.png"));

//    ui->securityPointSettingWidget->SetDirection(ICPageTag::IconOnRight);
    ui->stackSettingWidget->SetDirection(ICPageTag::IconOnRight);
    ui->systemSettingWidget->SetDirection(ICPageTag::IconOnRight);
    ui->maintainWidget->SetDirection(ICPageTag::IconOnRight);
    ui->machineStructConfig->SetDirection(ICPageTag::IconOnRight);
//    ui->machineConfigSettingWidget->SetDirection(ICPageTag::IconOnRight);
}

void ICFunctionPageFrame::InitSignal()
{
    connect(ui->signalSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->productSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->maintainWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->securityPointSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->stackSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->systemSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->machineStructConfig,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ui->machineConfigSettingWidget,
            SIGNAL(clicked()),
            this,
            SLOT(SettingButtonClicked()));
    connect(ICProgramHeadFrame::Instance(),
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(LevelChanged(int)));
}

void ICFunctionPageFrame::SettingButtonClicked()
{
    QAbstractButton *clickedButton = qobject_cast<QAbstractButton *>(sender());

    QWidget * settingPage = settingButtonToPage_.value(clickedButton, NULL);

    if(settingPage != NULL)
    {
        backFrameCenterStackedLayout_->setCurrentWidget(settingPage);
    }
    else
    {
        settingPage = ICSettingFunctionFactory::CreatePage(clickedButton, this);

        if(settingPage != NULL)
        {
            settingButtonToPage_.insert(clickedButton, settingPage);
            backFrameCenterStackedLayout_->addWidget(settingPage);
            backFrameCenterStackedLayout_->setCurrentWidget(settingPage);
        }
    }

    emit SettingPageShown(clickedButton->text());
//    ICCurrentChildPageNameFrame::Instance()->SetCurrentChildPageName(clickedButton->text());
}

void ICFunctionPageFrame::UpdateTranslate()
{
    ui->securityPointSettingWidget->setText(tr("Security Point Settings"));
    ui->systemSettingWidget->setText(tr("System Settings"));
    ui->machineStructConfig->setText(tr("Struct Settings"));
    ui->stackSettingWidget->setText(tr("Stack Settings"));
    ui->maintainWidget->setText(tr("Maintains"));
    ui->signalSettingWidget->setText(tr("Signal Settings"));
    ui->productSettingWidget->setText(tr("Product Settings"));
    ui->machineConfigSettingWidget->setText(tr("Machine Configure"));
}

void ICFunctionPageFrame::LevelChanged(int level)
{
    if(level >=  ICParametersSave::AdvanceAdmin)
    {
        ui->machineStructConfig->setEnabled(true);
    }
    else
    {
        ui->machineStructConfig->setEnabled(false);
    }
}
