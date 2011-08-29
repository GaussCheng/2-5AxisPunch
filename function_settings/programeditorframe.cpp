#include "programeditorframe.h"
#include "ui_programeditorframe.h"

ProgramEditorFrame::ProgramEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ProgramEditorFrame)
{
    ui->setupUi(this);
}

ProgramEditorFrame::~ProgramEditorFrame()
{
    delete ui;
}
