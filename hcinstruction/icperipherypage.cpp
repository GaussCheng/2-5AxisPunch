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
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetGMVal(ICMold::GCondition);
//            item.SetSVal(rowToInfoMap_.value(i).pointNum);
            item.SetSubNum(rowToInfoMap_.value(i).hardwarePoint);
            item.SetIFVal(ui->onBox->isChecked());
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
