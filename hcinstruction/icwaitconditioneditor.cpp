#include "icwaitconditioneditor.h"
#include "ui_icwaitconditioneditor.h"
#include <QPushButton>

ICWaitConditionEditor::ICWaitConditionEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICWaitConditionEditor)
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(new QIntValidator(0, 65530, this));
    ui->euInList->setColumnWidth(0, 50);
    ui->euInList->setColumnWidth(1, 120);
    ui->euOutList->setColumnWidth(0, 50);
    ui->euOutList->setColumnWidth(1, 120);

    QStringList infos = ICUserDefineConfig::Instance()->AllEuXStrings();
    int infosSize = infos.size();
    ui->euInList->blockSignals(true);
    ui->euOutList->blockSignals(true);
    ui->euInList->setRowCount(infosSize);
    QTableWidgetItem* item;
//    QPushButton* button;
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem(infos.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->euInList->setItem(i, 0, item);
//        button = new QPushButton(infos.at(i).GetLocaleName("zh"));
//        button->setIcon(offPixmap_);
//        button->setCheckable(true);
//        ui->euInList->setCellWidget(i, 1, button);
//        xrowToInfoMap_.insert(i, infos.at(i));
    }

    infos = ICUserDefineConfig::Instance()->AllEuYStrings();
    infosSize = infos.size();
    ui->euOutList->setRowCount(infosSize);
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem(infos.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->euOutList->setItem(i, 0, item);
//        button = new QPushButton(infos.at(i).GetLocaleName("zh"));
//        button->setIcon(offPixmap_);
//        button->setCheckable(true);
//        ui->euOutList->setCellWidget(i, 1, button);
//        yrowToInfoMap_.insert(i, infos.at(i));
    }
    ui->euInList->blockSignals(false);
    ui->euOutList->blockSignals(false);
    ui->onBox->setEnabled(false);
    ui->offBox->setEnabled(false);
}

ICWaitConditionEditor::~ICWaitConditionEditor()
{
    delete ui;
}

void ICWaitConditionEditor::changeEvent(QEvent *e)
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

QList<ICMoldItem> ICWaitConditionEditor::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
//    QPushButton* button;
    QTableWidget * tableWidget;
//    QMap<int, ICUserIOInfo> rowToInfoMap_;
    int gm;
    if(ui->tabWidget->currentIndex() == 0)
    {
        tableWidget = ui->euInList;
//        rowToInfoMap_ = xrowToInfoMap_;
        gm = ICMold::GWait;
    }
    else
    {
        tableWidget = ui->euOutList;
//        rowToInfoMap_ = yrowToInfoMap_;
        gm = ICMold::GEuOut;
    }
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

void ICWaitConditionEditor::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        ui->timeLabel->setText(tr("Limit"));
        ui->onBox->setChecked(true);
        ui->offBox->setChecked(false);
        ui->onBox->setEnabled(false);
        ui->offBox->setEnabled(false);
    }
    else
    {
        ui->timeLabel->setText(tr("Delay"));
        ui->onBox->setEnabled(true);
        ui->offBox->setEnabled(true);
    }
}

void ICWaitConditionEditor::on_euInList_itemChanged(QTableWidgetItem *item)
{
    Qt::CheckState state = item->checkState();
    const int rCount = ui->euInList->rowCount();
    ui->euInList->blockSignals(true);
    for(int i = 0; i != rCount; ++i)
    {
        ui->euInList->item(i, 0)->setCheckState(Qt::Unchecked);
    }
    item->setCheckState(state);
    ui->euInList->blockSignals(false);
}

void ICWaitConditionEditor::on_euOutList_itemChanged(QTableWidgetItem *item)
{
    Qt::CheckState state = item->checkState();
    const int rCount = ui->euOutList->rowCount();
    ui->euOutList->blockSignals(true);
    for(int i = 0; i != rCount; ++i)
    {
        ui->euOutList->item(i, 0)->setCheckState(Qt::Unchecked);
    }
    item->setCheckState(state);
    ui->euOutList->blockSignals(false);
}
