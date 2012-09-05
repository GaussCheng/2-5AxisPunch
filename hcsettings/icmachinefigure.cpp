#include "icmachinefigure.h"
#include "ui_icmachinefigure.h"
#include "icvirtualhost.h"
#include "iclineeditwrapper.h"

ICMachineFigure::ICMachineFigure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMachineFigure)
{
    ui->setupUi(this);
    ui->figure->resize(this->size());

    QIntValidator * validator = new QIntValidator(0, 65530, this);
    ui->xsec1->setValidator(validator);
    ui->xsec2->setValidator(validator);
    ui->xsec3->setValidator(validator);
    ui->xsec4->setValidator(validator);

    ui->xsec1->SetDecimalPlaces(1);
    ui->xsec2->SetDecimalPlaces(1);
    ui->xsec3->SetDecimalPlaces(1);
    ui->xsec4->SetDecimalPlaces(1);

    ICLineEditWrapper* wrapper = new ICLineEditWrapper(ui->xsec1,
                                                       ICVirtualHost::SM_X_SEC1,
                                                       ICLineEditWrapper::System,
                                                       ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->xsec2,
                                    ICVirtualHost::SM_X_SEC2,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->xsec3,
                                    ICVirtualHost::SM_X_SEC3,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);
    wrapper = new ICLineEditWrapper(ui->xsec4,
                                    ICVirtualHost::SM_X_SEC4,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);
}

ICMachineFigure::~ICMachineFigure()
{
    delete ui;
}

void ICMachineFigure::changeEvent(QEvent *e)
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

void ICMachineFigure::showEvent(QShowEvent *e)
{
    ui->xmin->setText(QString("%1").arg(ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_X_InSafe).toUInt() / 10.0));
    ui->xmax->setText(QString("%1").arg(ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::SYS_X_OutSafe).toUInt() / 10.0));
}

void ICMachineFigure::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
}

void ICMachineFigure::on_saveButton_clicked()
{
    ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
}
