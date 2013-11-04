#include "ichcotherpage.h"
#include "ui_ichcotherpage.h"

ICHCOtherPage::ICHCOtherPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCOtherPage)
{
    ui->setupUi(this);
}

ICHCOtherPage::~ICHCOtherPage()
{
    delete ui;
}

QList<ICMoldItem> ICHCOtherPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    item.SetAction(ICMold::ACT_OTHER);
    if(ui->clearButton->isChecked())
    {
        item.SetIFVal(1);
    }
    ret.append(item);
    return ret;
}
