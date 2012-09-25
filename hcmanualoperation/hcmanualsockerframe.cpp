#include "hcmanualsockerframe.h"
#include "ui_hcmanualsockerframe.h"

#include "iccommandkeywrapper.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"

HCManualSockerFrame::HCManualSockerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCManualSockerFrame)
{
    ui->setupUi(this);
    ICCommandKeyWrapper *wrapper;
    wrapper = new ICCommandKeyWrapper(ui->connectSucker1ToolButton, IC::VKEY_CLIP5ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectSucker2ToolButton, IC::VKEY_CLIP6ON);
    wrappers_.append(wrapper);

    wrapper = new ICCommandKeyWrapper(ui->disconnectSucker1ToolButton, IC::VKEY_CLIP5OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectSucker2ToolButton, IC::VKEY_CLIP6OFF);
    wrappers_.append(wrapper);

}

HCManualSockerFrame::~HCManualSockerFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void HCManualSockerFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            SLOT(StatusRefreshed()));
}

void HCManualSockerFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
}

void HCManualSockerFrame::changeEvent(QEvent *e)
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

void HCManualSockerFrame::StatusRefreshed()
{
    static QPixmap off(":/resource/ledgray(16).png");
    static QPixmap on(":/resource/ledgreen(16).png");
    static QPixmap inOn(":/resource/ledred(16).png");
    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsClipOn(4))
    {
        ui->sucker1StatusLabel->setPixmap(on);
    }
    else
    {
        ui->sucker1StatusLabel->setPixmap(off);
    }

    if(host->IsClipOn(5))
    {
        ui->sucker2StatusLabel->setPixmap(on);
    }
    else
    {
        ui->sucker2StatusLabel->setPixmap(off);
    }

    if(host->IsInputOn(5))
    {
        ui->sucker1Label->setPixmap(inOn);
    }
    else
    {
        ui->sucker1Label->setPixmap(off);
    }

    if(host->IsInputOn(4))
    {
        ui->sucker2Label->setPixmap(inOn);
    }
    else
    {
        ui->sucker2Label->setPixmap(off);
    }
}
