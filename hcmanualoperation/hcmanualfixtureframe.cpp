#include "hcmanualfixtureframe.h"
#include "ui_hcmanualfixtureframe.h"
#include "iccommandkeywrapper.h"
#include "icvirtualkey.h"
#include "icvirtualhost.h"
#include "ictimerpool.h"

HCManualFixtureFrame::HCManualFixtureFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HCManualFixtureFrame),
    clips_(8, false)
{
    ui->setupUi(this);
    ICCommandKeyWrapper *wrapper;
    wrapper = new ICCommandKeyWrapper(ui->connectFixture1ToolButton, IC::VKEY_CLIP1ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectFixture2ToolButton, IC::VKEY_CLIP2ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectFixture3ToolButton, IC::VKEY_CLIP3ON);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->connectFixture4ToolButton, IC::VKEY_CLIP4ON);
    wrappers_.append(wrapper);

    wrapper = new ICCommandKeyWrapper(ui->disconnectFixture1ToolButton, IC::VKEY_CLIP1OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectFixture2ToolButton, IC::VKEY_CLIP2OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectFixture3ToolButton, IC::VKEY_CLIP3OFF);
    wrappers_.append(wrapper);
    wrapper = new ICCommandKeyWrapper(ui->disconnectFixture4ToolButton, IC::VKEY_CLIP4OFF);
    wrappers_.append(wrapper);
}

HCManualFixtureFrame::~HCManualFixtureFrame()
{
    delete ui;
    qDeleteAll(wrappers_);
}

void HCManualFixtureFrame::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);
    //    connect(ICVirtualHost::GlobalVirtualHost(),
    //            SIGNAL(StatusRefreshed()),
    //            this,
    //            SLOT(StatusRefreshed()));
    timerID_ = ICTimerPool::Instance()->Start(ICTimerPool::RefreshTime, this, SLOT(StatusRefreshed()));
}

void HCManualFixtureFrame::hideEvent(QHideEvent *e)
{
    QFrame::hideEvent(e);
    //    disconnect(ICVirtualHost::GlobalVirtualHost(),
    //               SIGNAL(StatusRefreshed()),
    //               this,
    //               SLOT(StatusRefreshed()));
    ICTimerPool::Instance()->Stop(timerID_, this, SLOT(StatusRefreshed()));
}

void HCManualFixtureFrame::changeEvent(QEvent *e)
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

void HCManualFixtureFrame::StatusRefreshed()
{
    static QPixmap off(":/resource/ledgray(16).png");
    static QPixmap on(":/resource/ledgreen(16).png");
    static QPixmap inOn(":/resource/ledred(16).png");
    static ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    if(host->IsClipOn(0))
    {
        if(!clips_.at(0))
        {
            clips_.setBit(0);
            ui->fixture1StatusLabel->setPixmap(on);
        }
    }
    else
    {
        if(clips_.at(0))
        {
            clips_.clearBit(0);
            ui->fixture1StatusLabel->setPixmap(off);
        }
    }
    if(host->IsClipOn(1))
    {
        if(!clips_.at(1))
        {
            clips_.setBit(1);
            ui->fixture2StatusLabel->setPixmap(on);
        }
    }
    else
    {
        if(clips_.at(1))
        {
            clips_.clearBit(1);
            ui->fixture2StatusLabel->setPixmap(off);
        }
    }
    if(host->IsOutputOn(20))
    {
        if(!clips_.at(2))
        {
            clips_.setBit(2);
            ui->fixture3StatusLabel->setPixmap(on);
        }
    }
    else
    {
        if(clips_.at(2))
        {
            clips_.clearBit(2);
            ui->fixture3StatusLabel->setPixmap(off);
        }
    }
    if(host->IsClipOn(3))
    {
        if(!clips_.at(3))
        {
            clips_.setBit(3);
            ui->fixture4StatusLabel->setPixmap(on);
        }
    }
    else
    {
        if(clips_.at(3))
        {
            clips_.clearBit(3);
            ui->fixture4StatusLabel->setPixmap(off);
        }
    }
    if(host->IsInputOn(2))
    {
        if(!clips_.at(4))
        {
            clips_.setBit(4);
            ui->fixture1InLabel->setPixmap(inOn);
        }
    }
    else
    {
        if(clips_.at(4))
        {
            clips_.clearBit(4);
            ui->fixture1InLabel->setPixmap(off);
        }
    }
    if(host->IsInputOn(3))
    {
        if(!clips_.at(5))
        {
            clips_.setBit(5);
            ui->fixture2InLabel->setPixmap(inOn);
        }
    }
    else
    {
        if(clips_.at(5))
        {
            clips_.clearBit(5);
            ui->fixture2InLabel->setPixmap(off);
        }
    }
    if(host->IsInputOn(20))
    {
        if(!clips_.at(6))
        {
            clips_.setBit(6);
            ui->fixture3InLabel->setPixmap(inOn);
        }
    }
    else
    {
        if(clips_.at(6))
        {
            clips_.clearBit(6);
            ui->fixture3InLabel->setPixmap(off);
        }
    }
    if(host->IsInputOn(9))
    {
        if(!clips_.at(7))
        {
            clips_.setBit(7);
            ui->fixture4InLabel->setPixmap(inOn);
        }
    }
    else
    {
        if(clips_.at(7))
        {
            clips_.clearBit(7);
            ui->fixture4InLabel->setPixmap(off);
        }
    }
}
