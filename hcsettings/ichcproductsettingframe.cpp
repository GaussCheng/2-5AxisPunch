#include "ichcproductsettingframe.h"
#include "ui_ichcproductsettingframe.h"

#include "iclineeditwrapper.h"
#include "icmold.h"
#include "icvirtualhost.h"
#include "icvirtualkey.h"

ICHCProductSettingFrame::ICHCProductSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCProductSettingFrame)
{
    ui->setupUi(this);

    ui->productLineEdit->setValidator(new QIntValidator(0, 65535, ui->productLineEdit));
    ui->alarmTimesEdit->setValidator(new QIntValidator(0, 65535, ui->alarmTimesEdit));
    ui->waitTimeEdit->SetDecimalPlaces(1);
    ui->waitTimeEdit->setValidator(new QIntValidator(0, 60000, ui->waitTimeEdit));
    ICLineEditWrapper *wrapper = new ICLineEditWrapper(ui->productLineEdit,
                                                       ICMold::Product,
                                                       ICLineEditWrapper::Mold,
                                                       ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->waitTimeEdit,
                                    ICVirtualHost::SM_WaitMoldOpenLimit,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::OneFraction);
    wrappers_.append(wrapper);

    wrapper = new ICLineEditWrapper(ui->alarmTimesEdit,
                                    ICVirtualHost::SM_ACCTIME,
                                    ICLineEditWrapper::System,
                                    ICLineEditWrapper::Integer);
    wrappers_.append(wrapper);

    int currentPos = ICMold::CurrentMold()->MoldParam(ICMold::PosMainDown);
    if(currentPos > 1)
    {
        currentPos = 1;
    }
    connect(ICMold::CurrentMold(),
            SIGNAL(MoldNumberParamChanged()),
            this,
            SLOT(OnMoldNumberParamChanged()));
}

ICHCProductSettingFrame::~ICHCProductSettingFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void ICHCProductSettingFrame::hideEvent(QHideEvent *e)
{
    qDebug("Product hide");
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())
    {
        ICMold::CurrentMold()->SaveMoldParamsFile();
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
    }
    QFrame::hideEvent(e);
}

void ICHCProductSettingFrame::changeEvent(QEvent *e)
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

void ICHCProductSettingFrame::OnMoldNumberParamChanged()
{
    for(int i = 0; i != wrappers_.size(); ++i)
    {
        wrappers_[i]->UpdateParam();
    }
}

void ICHCProductSettingFrame::on_productClearButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_PRODUCT_CLEAR);
    ICVirtualHost::GlobalVirtualHost()->SetFinishProductCount(0);
}
