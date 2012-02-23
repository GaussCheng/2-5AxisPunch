#include "ichcconditionpage.h"
#include "ui_ichcconditionpage.h"
#include "iccommandprocessor.h"
#include "icvirtualhost.h"

ICHCConditionPage::ICHCConditionPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCConditionPage)
{
    ui->setupUi(this);
    ui->returnLineEdit->setValidator(new QIntValidator(0, 1000, this));
}

ICHCConditionPage::~ICHCConditionPage()
{
    delete ui;
}

void ICHCConditionPage::changeEvent(QEvent *e)
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

void ICHCConditionPage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)
}


QList<ICMoldItem> ICHCConditionPage::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    item.SetAction(ICMold::ACTCHECKINPUT);
    item.SetIFVal(ui->checkPointComboBox->currentIndex());
    item.SetSVal(ui->subComboBox->currentIndex());
    item.SetDVal(ui->returnLineEdit->TransThisTextToThisInt());
    ret.append(item);
    return ret;
}
