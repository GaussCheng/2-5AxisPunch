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

    items<<tr("M10")<<tr("M11")<<tr("M12")<<tr("M13")<<tr("M14")
        <<tr("M15")<<tr("M16")<<tr("M17")<<tr("M20")<<tr("M21")
        <<tr("M22")<<tr("M23")<<tr("M24")<<tr("M25")<<tr("M26")
        <<tr("M27")<<tr("M30")<<tr("M31")<<tr("M32")<<tr("M33")
        <<tr("M34")<<tr("M35")<<tr("M36")<<tr("M37")<<tr("M40")
        <<tr("M41")<<tr("M42")<<tr("M43")<<tr("M44")<<tr("M45")
        <<tr("M46")<<tr("M47");
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
