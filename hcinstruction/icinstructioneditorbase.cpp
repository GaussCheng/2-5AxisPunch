#include "icinstructioneditorbase.h"
#include "ui_icinstructioneditorbase.h"

ICInstructionEditorBase::ICInstructionEditorBase(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICInstructionEditorBase)
{
    ui->setupUi(this);
}

ICInstructionEditorBase::~ICInstructionEditorBase()
{
    delete ui;
}
