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

void ICProgramSelector::on_selectButton_clicked()
{
    emit ProgramChanged(ui->programList->currentRow(), ui->programList->item(ui->programList->currentRow())->text());
}
