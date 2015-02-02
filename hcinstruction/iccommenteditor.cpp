#include "iccommenteditor.h"
#include "ui_iccommenteditor.h"

ICCommentEditor::ICCommentEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICCommentEditor)
{
    ui->setupUi(this);
}

ICCommentEditor::~ICCommentEditor()
{
    delete ui;
}

void ICCommentEditor::changeEvent(QEvent *e)
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

QList<ICMoldItem> ICCommentEditor::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    item.SetAction(ICMold::ACTCOMMENT);
    item.SetComment(ui->plainTextEdit->toPlainText());
    ret.append(item);
    return ret;
}

QString ICCommentEditor::Comment()
{
    return ui->plainTextEdit->toPlainText();
}
