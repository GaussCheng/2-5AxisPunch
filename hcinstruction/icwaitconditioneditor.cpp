#include "icwaitconditioneditor.h"
#include "ui_icwaitconditioneditor.h"

ICWaitConditionEditor::ICWaitConditionEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICWaitConditionEditor)
{
    ui->setupUi(this);
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
    item.SetAction(ICMold::ACT_WaitMoldOpened);
    item.SetDVal(0);
    if(ui->moldOpenedCheckBox->isChecked())
    {
        item.SetSVal(1);
    }
    else if(ui->securityDoorCheckBox->isChecked())
    {
        item.SetSVal(2);
    }
    else if(ui->x043CheckBox->isChecked())
    {
        item.SetSVal(3);
    }
    else if(ui->x044CheckBox->isChecked())
    {
        item.SetSVal(4);
    }
    ret.append(item);
    return ret;
}
