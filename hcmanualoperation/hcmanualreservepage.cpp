#include "hcmanualreservepage.h"
#include "ui_hcmanualreservepage.h"
#include "icvirtualkey.h"
#include "iccommandkeywrapper.h"
#include "icvirtualhost.h"

HCManualReservePage::HCManualReservePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HCManualReservePage),
    clips_(6, false)
{
    ui->setupUi(this);
    ui->reserve1InLabel->hide();
    ui->reserve2InLabel->hide();
    ui->reserve3InLabel->hide();
    ui->reserve4InLabel->hide();
    ui->reserve5InLabel->hide();
    ui->reserve6InLabel->hide();
    ICCommandKeyWrapper *wrapper;
    wrapper = new ICCommandKeyWrapper(ui->connectReserve1ToolButton, IC::VKEY_RESERVE1_ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectReserve2ToolButton, IC::VKEY_RESERVE2_ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectReserve3ToolButton, IC::VKEY_RESERVE3_ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectReserve4ToolButton, IC::VKEY_RESERVE4_ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectReserve5ToolButton, IC::VKEY_RESERVE5_ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectReserve6ToolButton, IC::VKEY_RESERVE6_ON);
    wrappers_.append(wrapper);

    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve1ToolButton, IC::VKEY_RESERVE1_OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve2ToolButton, IC::VKEY_RESERVE2_OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve3ToolButton, IC::VKEY_RESERVE3_OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve4ToolButton, IC::VKEY_RESERVE4_OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve5ToolButton, IC::VKEY_RESERVE5_OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectReserve6ToolButton, IC::VKEY_RESERVE6_OFF);
    wrappers_.append(wrapper);

}

HCManualReservePage::~HCManualReservePage()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void HCManualReservePage::changeEvent(QEvent *e)
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

void HCManualReservePage::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
}

void HCManualReservePage::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
}

void HCManualReservePage::StatusRefreshed()
{
    static QPixmap off(":/resource/ledgray(16).png");
    static QPixmap on(":/resource/ledgreen(16).png");
    static QPixmap inOn(":/resource/ledred(16).png");
    static ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    if(host->PeripheryOutput(0) == 1)
    {
        if(host->IsOutputOn(10))
        {
            if(!clips_.at(0))
            {
                clips_.setBit(0);
                ui->reserve1StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(0))
            {
                clips_.clearBit(0);
                ui->reserve1StatusLabel->setPixmap(off);
            }
        }
    }

    if(host->PeripheryOutput(1) == 1)
    {
        if(host->IsOutputOn(11))
        {
            if(!clips_.at(1))
            {
                clips_.setBit(1);
                ui->reserve2StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(1))
            {
                clips_.clearBit(1);
                ui->reserve2StatusLabel->setPixmap(off);
            }
        }
    }

    if(host->PeripheryOutput(2) == 1)
    {
        if(host->IsOutputOn(18))
        {
            if(!clips_.at(2))
            {
                clips_.setBit(2);
                ui->reserve3StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(2))
            {
                clips_.clearBit(2);
                ui->reserve3StatusLabel->setPixmap(off);
            }
        }
    }

    if(host->PeripheryOutput(3) == 1)
    {
        if(host->IsOutputOn(19))
        {
            if(!clips_.at(3))
            {
                clips_.setBit(3);
                ui->reserve4StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(3))
            {
                clips_.clearBit(3);
                ui->reserve4StatusLabel->setPixmap(off);
            }
        }
    }

    if(host->PeripheryOutput(4) == 1)
    {
        if(host->IsOutputOn(21))
        {
            if(!clips_.at(4))
            {
                clips_.setBit(4);
                ui->reserve5StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(4))
            {
                clips_.clearBit(4);
                ui->reserve5StatusLabel->setPixmap(off);
            }
        }
    }

    if(host->PeripheryOutput(5) == 1)
    {
        if(host->IsOutputOn(22))
        {
            if(!clips_.at(5))
            {
                clips_.setBit(5);
                ui->reserve6StatusLabel->setPixmap(on);
            }
        }
        else
        {
            if(clips_.at(5))
            {
                clips_.clearBit(5);
                ui->reserve6StatusLabel->setPixmap(off);
            }
        }
    }

}
