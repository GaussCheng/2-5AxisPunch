#include <QPushButton>
#include "icperipherypage.h"
#include "ui_icperipherypage.h"

#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"
#include "icperipheryparametereditor.h"

ICPeripheryPage::ICPeripheryPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICPeripheryPage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(new QIntValidator(0, 65530, this));
    ui->returnEdit->setValidator(new QIntValidator(-100, 100, this));
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
    QStringList items;
    items<<"M10"<<"M11"<<"M12"<<"M13"<<"M14"
        <<"M15"<<"M16"<<"M17"<<"M20"<<"M21"
       <<"M22"<<"M23"<<"M24"<<"M25"<<"M26"
      <<"M27"<<"M30"<<"M31"<<"M32"<<"M33"
     <<"M34"<<"M35"<<"M36"<<"M37"<<"M40"
    <<"M41"<<"M42"<<"M43"<<"M44"<<"M45"
    <<"M46"<<"M47";
    ui->mList->blockSignals(true);
    ui->mList->setRowCount(items.size());
    for(int i = 0; i != items.size(); ++i)
    {
        item = new QTableWidgetItem(items.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->mList->setItem(i, 0, item);
    }
    ui->mList->blockSignals(false);
}

ICPeripheryPage::~ICPeripheryPage()
{
    delete ui;
}

void ICPeripheryPage::changeEvent(QEvent *e)
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

void ICPeripheryPage::showEvent(QShowEvent *e)
{
//    connect(ICVirtualHost::GlobalVirtualHost(),
//            SIGNAL(StatusRefreshed()),
//            this,
//            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICPeripheryPage::hideEvent(QHideEvent *e)
{
//    disconnect(ICVirtualHost::GlobalVirtualHost(),
//               SIGNAL(StatusRefreshed()),
//               this,
//               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICPeripheryPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}

void ICPeripheryPage::StatusRefreshed(){}

void ICPeripheryPage::CommandButtonClicked(QWidget *widget)
{
}


QList<ICMoldItem> ICPeripheryPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    QTableWidget* tableWidget = ui->tabWidget->currentIndex() == 0 ? ui->tableWidget : ui->mList;
    for(int i = 0; i != tableWidget->rowCount(); ++i)
    {
        if(tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetGMVal(ICMold::GCondition);
//            item.SetSVal(rowToInfoMap_.value(i).pointNum);
            if(ui->tabWidget->currentIndex() == 0)
            {
                item.SetSubNum(rowToInfoMap_.value(i).hardwarePoint);
                item.SetIFVal(ui->onBox->isChecked());
            }
            else
            {
                item.SetSubNum(i);
                item.SetIFVal(ui->onBox->isChecked() | 0x80);
            }
            item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
            item.SetSVal(ui->returnEdit->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;
}

void ICPeripheryPage::on_tableWidget_itemChanged(QTableWidgetItem *item)
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


void ICPeripheryPage::on_mList_itemChanged(QTableWidgetItem *item)
{
    Qt::CheckState state = item->checkState();
    const int rCount = ui->mList->rowCount();
    ui->mList->blockSignals(true);
    for(int i = 0; i != rCount; ++i)
    {
        ui->mList->item(i, 0)->setCheckState(Qt::Unchecked);
    }
    item->setCheckState(state);
    ui->mList->blockSignals(false);
}
