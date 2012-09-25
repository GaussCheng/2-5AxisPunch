#include "hcmanualotherioframe.h"
#include "ui_hcmanualotherioframe.h"

#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandkeywrapper.h"

HCManualOtherIOFrame::HCManualOtherIOFrame(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::HCManualOtherIOFrame)
{
    ui->setupUi(this);

    ICCommandKeyWrapper *wrapper;
    wrapper = new ICCommandKeyWrapper(ui->conveyorOnButton, IC::VKEY_CLIP8ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->onInjectionButton, IC::VKEY_CLIP7ON);
    wrappers_.append(wrapper);

    wrapper = new ICCommandKeyWrapper(ui->conveyorOFFButton, IC::VKEY_CLIP8OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->offInjectionButton, IC::VKEY_CLIP7OFF);
    wrappers_.append(wrapper);
}

HCManualOtherIOFrame::~HCManualOtherIOFrame()
{
    delete ui;
}

void HCManualOtherIOFrame::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            SLOT(StatusRefreshed()));
}

void HCManualOtherIOFrame::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
}

void HCManualOtherIOFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
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

void HCManualOtherIOFrame::StatusRefreshed()
{
    static QPixmap off(":/resource/ledgray(16).png");
    static QPixmap on(":/resource/ledgreen(16).png");
    static ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsClipOn(6))
    {
        ui->injectionStatusLabel->setPixmap(on);
    }
    else
    {
        ui->injectionStatusLabel->setPixmap(off);
    }

    if(host->IsClipOn(7))
    {
        ui->conveyorStatusLabel->setPixmap(on);
    }
    else
    {
        ui->conveyorStatusLabel->setPixmap(off);
    }
}

