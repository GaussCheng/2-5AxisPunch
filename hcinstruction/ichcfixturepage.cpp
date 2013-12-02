#include <QLabel>
#include <QPushButton>

#include "ichcfixturepage.h"
#include "ui_ichcfixturepage.h"

#include "icvirtualhost.h"
#include "icvirtualkey.h"
#include "iccommandprocessor.h"


ICHCFixturePage::ICHCFixturePage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCFixturePage),
    onPixmap_(":/resource/ledgreen(16).png"),
    offPixmap_(":/resource/ledgray(16).png")
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->tableWidget->setColumnWidth(0, 50);
    ui->tableWidget->setColumnWidth(1, 120);

    QList<ICUserActionInfo> infos = ICUserDefineConfig::Instance()->GetActionInfosByType(0);
    const int infosSize = infos.size();
    ui->tableWidget->setRowCount(infosSize);
    QTableWidgetItem* item;
    QPushButton* button;
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(i, 0, item);
        button = new QPushButton(infos.at(i).GetLocaleName("zh"));
//        button->setIcon(offPixmap_);
        button->setCheckable(true);
        ui->tableWidget->setCellWidget(i, 1, button);
        rowToInfoMap_.insert(i, infos.at(i));
    }
}

ICHCFixturePage::~ICHCFixturePage()
{
    delete ui;
}

void ICHCFixturePage::changeEvent(QEvent *e)
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

void ICHCFixturePage::showEvent(QShowEvent *e)
{
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    QWidget::showEvent(e);
}

void ICHCFixturePage::hideEvent(QHideEvent *e)
{
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    QWidget::hideEvent(e);
}

void ICHCFixturePage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}


void ICHCFixturePage::StatusRefreshed()
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

void ICHCFixturePage::CommandButtonClicked(QWidget *widget)
{
}


QList<ICMoldItem> ICHCFixturePage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    QPushButton* button;
    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetGMVal(ICMold::GOutY);
//            item.SetSVal(rowToInfoMap_.value(i).pointNum);
            item.SetSubNum(rowToInfoMap_.value(i).pointNum);
            button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 1));
            item.SetIFVal(button->isChecked());
            item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;
}
