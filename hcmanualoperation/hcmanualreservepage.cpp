#include "hcmanualreservepage.h"
#include "ui_hcmanualreservepage.h"
#include "icvirtualkey.h"
#include "iccommandkeywrapper.h"

HCManualReservePage::HCManualReservePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HCManualReservePage)
{
    ui->setupUi(this);
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
