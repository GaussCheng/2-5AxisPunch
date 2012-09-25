#include "icprogramselector.h"
#include "ui_icprogramselector.h"

ICProgramSelector::ICProgramSelector(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICProgramSelector)
{
    ui->setupUi(this);
}

ICProgramSelector::~ICProgramSelector()
{
    delete ui;
}

void ICProgramSelector::changeEvent(QEvent *e)
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

void ICProgramSelector::on_selectButton_clicked()
{
    if(ui->programList->currentRow() < 0)
    {
        return;
    }
    emit ProgramChanged(ui->programList->currentRow(), ui->programList->item(ui->programList->currentRow())->text());
}
