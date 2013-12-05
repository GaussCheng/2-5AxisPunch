#include <QPushButton>
#include <QTableWidgetItem>
#include "iccutpage.h"
#include "ui_iccutpage.h"

#include "icvirtualhost.h"
#include "icactioncommand.h"
#include "icvirtualkey.h"

ICCutPage::ICCutPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICCutPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(new QIntValidator(0, 65530, this));
    QList<ICUserIOInfo> infos = ICUserDefineConfig::Instance()->AllXInfos();
    const int infosSize = infos.size();
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->setRowCount(infosSize);
    QTableWidgetItem* item;
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem(infos.at(i).GetLocaleName("zh"));
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(i, 0, item);
        rowToInfoMap_.insert(i, infos.at(i));
    }
    ui->tableWidget->blockSignals(false);
}

ICCutPage::~ICCutPage()
{
    delete ui;
}
void ICCutPage::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICCutPage::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICCutPage::changeEvent(QEvent *e)
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

void ICCutPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}


void ICCutPage::StatusRefreshed()
{
//    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    QMap<QLabel*, int>::iterator p = currentPosMap_.begin();
//    while(p != currentPosMap_.end())
//    {
//        if(host->IsClipOn(p.value()))
//        {
//            if(!ledIsOnMap_.value(p.key()))
//            {
//                p.key()->setPixmap(onPixmap_);
//                ledIsOnMap_.insert(p.key(), true);
//            }
//        }
//        else
//        {
//            if(ledIsOnMap_.value(p.key()))
//            {
//                p.key()->setPixmap(offPixmap_);
//                ledIsOnMap_.insert(p.key(), false);
//            }
//        }
//        ++p;
//    }
}

void ICCutPage::CommandButtonClicked(QWidget *widget)
{
}


QList<ICMoldItem> ICCutPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetGMVal(ICMold::GCheckX);
//            item.SetSVal(rowToInfoMap_.value(i).pointNum);
            item.SetSubNum(rowToInfoMap_.value(i).hardwarePoint);
            item.SetIFVal(ui->onBox->isChecked());
            item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;
}

void ICCutPage::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    Qt::CheckState state = item->checkState();
    const int rCount = ui->tableWidget->rowCount();
    ui->tableWidget->blockSignals(true);
    for(int i = 0; i != rCount; ++i)
    {
        ui->tableWidget->item(i, 0)->setCheckState(Qt::Unchecked);
    }
    item->setCheckState(state);
    ui->tableWidget->blockSignals(false);

}
