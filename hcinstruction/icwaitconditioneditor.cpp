#include "icwaitconditioneditor.h"
#include "ui_icwaitconditioneditor.h"
#include <QPushButton>

ICWaitConditionEditor::ICWaitConditionEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICWaitConditionEditor)
{
    ui->setupUi(this);
    ui->delayEdit->SetDecimalPlaces(1);
    ui->delayEdit->setValidator(new QIntValidator(0, 65530, this));
    ui->onBox->setEnabled(false);
    ui->offBox->setEnabled(false);
    ui->delayEdit->SetThisIntToThisText(0);
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
    item.SetSubNum(0);
    //            button = qobject_cast<QPushButton*>(tableWidget->cellWidget(i, 1));
    item.SetIFVal(ui->onBox->isChecked());
    item.SetDVal(ui->delayEdit->TransThisTextToThisInt());
    ret.append(item);
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
    }
    else
    {
        ui->timeLabel->setText(tr("Delay"));
        ui->onBox->setEnabled(true);
        ui->offBox->setEnabled(true);
    }
}

void ICWaitConditionEditor::on_euOutBox_toggled(bool checked)
{
    ui->delayEdit->SetDecimalPlaces(checked ? 2 : 1);
    ui->delayEdit->SetThisIntToThisText(ui->delayEdit->TransThisTextToThisInt());
}
