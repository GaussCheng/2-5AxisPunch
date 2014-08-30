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
    ui->onBox->setEnabled(false);
    ui->offBox->setEnabled(false);

    ui->tableWidget->setColumnWidth(0, 50);
    ui->tableWidget->setColumnWidth(1, 120);

    QList<ICUserIOInfo> infos = ICUserDefineConfig::Instance()->AllEuXInfos();
    int infosSize = infos.size();
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->setRowCount(infosSize);
    QTableWidgetItem* item;
    //    QPushButton* button;
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem(infos.at(i).GetLocaleName("zh"));
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(i, 0, item);
        //        button = new QPushButton(infos.at(i).GetLocaleName("zh"));
        ////        button->setIcon(offPixmap_);
        //        button->setCheckable(true);
        //        ui->tableWidget->setCellWidget(i, 1, button);
        //        item = new QTableWidgetItem(infos.at(i).GetLocaleName("zh"));
        //        ui->tableWidget->setItem(i, 1, item);
        xrowToInfoMap_.insert(i, infos.at(i));
    }
    ui->tableWidget->blockSignals(false);

    infos = ICUserDefineConfig::Instance()->AllEuYInfos();
    infosSize = infos.size();
    ui->tableWidget_2->blockSignals(true);
    ui->tableWidget_2->setRowCount(infosSize);
    //    QPushButton* button;
    for(int i = 0; i != infosSize; ++i)
    {
        item = new QTableWidgetItem(infos.at(i).GetLocaleName("zh"));
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget_2->setItem(i, 0, item);
        //        button = new QPushButton(infos.at(i).GetLocaleName("zh"));
        ////        button->setIcon(offPixmap_);
        //        button->setCheckable(true);
        //        ui->tableWidget->setCellWidget(i, 1, button);
        //        item = new QTableWidgetItem(infos.at(i).GetLocaleName("zh"));
        //        ui->tableWidget->setItem(i, 1, item);
        yrowToInfoMap_.insert(i, infos.at(i));
    }
    ui->tableWidget_2->blockSignals(false);
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
    int gm;
    if(ui->euInbox->isChecked())
    {
        gm = ICMold::GWait;
    }
    else
    {
        gm = ICMold::GEuOut;
    }
    item.SetGMVal(gm);
    //            item.SetSVal(rowToInfoMap_.value(i).pointNum);
    if(gm == ICMold::GWait)
    {
        ICUserIOInfo actionInfo;
        for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
        {
            if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
            {
//                item.SetGMVal(ICMold::GOutY + ui->functionBox->currentIndex());
                //            item.SetSVal(rowToInfoMap_.value(i).pointNum);
                actionInfo = xrowToInfoMap_.value(i);
//                if(ui->functionBox->currentIndex() == 2)
//                {
//                    item.SetSubNum(actionInfo.hardwarePoint >> 1);
//                }
//                else
//                {
                    item.SetSubNum(actionInfo.hardwarePoint);
//                }
                //            button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 1));
                item.SetIFVal(ui->onBox->isChecked());
                item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
                ret.append(item);
            }
        }
    }
    else
    {
        ICUserIOInfo actionInfo;
        for(int i = 0; i != ui->tableWidget_2->rowCount(); ++i)
        {
            if(ui->tableWidget_2->item(i,0)->checkState() == Qt::Checked)
            {
//                item.SetGMVal(ICMold::GOutY + ui->functionBox->currentIndex());
                //            item.SetSVal(rowToInfoMap_.value(i).pointNum);
                actionInfo = yrowToInfoMap_.value(i);
//                if(ui->functionBox->currentIndex() == 2)
//                {
//                    item.SetSubNum(actionInfo.hardwarePoint >> 1);
//                }
//                else
//                {
                    item.SetSubNum(actionInfo.hardwarePoint);
//                }
                //            button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 1));
                item.SetIFVal(ui->onBox->isChecked());
                item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
                ret.append(item);
            }
        }
    }
//    item.SetSubNum(0);
    //            button = qobject_cast<QPushButton*>(tableWidget->cellWidget(i, 1));
//    item.SetIFVal(ui->onBox->isChecked());
//    item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
//    ret.append(item);
    return ret;
}

void ICWaitConditionEditor::on_euInbox_toggled(bool checked)
{
    if(checked)
    {
        ui->timeLabel->setText(tr("Limit"));
        ui->onBox->setChecked(true);
        ui->offBox->setChecked(false);
        ui->onBox->setEnabled(false);
        ui->offBox->setEnabled(false);
        ui->tabWidget->setCurrentIndex(0);
    }
    else
    {
        ui->timeLabel->setText(tr("Delay"));
        ui->onBox->setEnabled(true);
        ui->offBox->setEnabled(true);
        ui->tabWidget->setCurrentIndex(1);

    }
}
