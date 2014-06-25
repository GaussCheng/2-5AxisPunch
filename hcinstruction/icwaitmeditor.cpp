#include "icwaitmeditor.h"
#include "ui_icwaitmeditor.h"

ICWaitMEditor::ICWaitMEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICWaitMEditor)
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(new QIntValidator(0, 65530, this));
    QStringList items;

    items<<tr("M10")<<tr("M11")<<tr("M12")<<tr("M13")
        <<tr("N14")
        <<tr("M15")<<"M16"<<"M17"<<"M20"<<"M21"
       <<"M22"<<"M23"<<"M24"<<"M25"<<"M26"
      <<"M27"<<"M30"<<"M31"<<"M32"<<"M33"
     <<"M34"<<"M35"<<"M36"<<"M37"<<"M40"
    <<"M41"<<"M42"<<"M43"<<"M44"<<"M45"
    <<"M46"<<"M47";
    ui->mList->blockSignals(true);
    ui->mList->setRowCount(items.size());
    QTableWidgetItem* item;
    for(int i = 0; i != items.size(); ++i)
    {
        item = new QTableWidgetItem(items.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->mList->setItem(i, 0, item);
    }
    ui->mList->blockSignals(false);
}

ICWaitMEditor::~ICWaitMEditor()
{
    delete ui;
}

void ICWaitMEditor::changeEvent(QEvent *e)
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

QList<ICMoldItem> ICWaitMEditor::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
//    QPushButton* button;
    QTableWidget * tableWidget = ui->mList;
//    QMap<int, ICUserIOInfo> rowToInfoMap_;
    int gm = ui->waitBox->isChecked() ? ICMold::GMWait : ICMold::GMOut;
    for(int i = 0; i != tableWidget->rowCount(); ++i)
    {
        if(tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            item.SetGMVal(gm);
//            item.SetSVal(rowToInfoMap_.value(i).pointNum);
            item.SetSubNum(i);
//            button = qobject_cast<QPushButton*>(tableWidget->cellWidget(i, 1));
            item.SetIFVal(ui->onBox->isChecked());
            item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
            ret.append(item);
        }
    }
    return ret;

}

void ICWaitMEditor::on_mList_itemChanged(QTableWidgetItem *item)
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
