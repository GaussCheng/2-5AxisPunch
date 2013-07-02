#include "ichcconditionpage.h"
#include "ui_ichcconditionpage.h"
#include "iccommandprocessor.h"
#include "icvirtualhost.h"

ICHCConditionPage::ICHCConditionPage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICHCConditionPage)
{
    ui->setupUi(this);
    ui->returnLineEdit->setValidator(new QIntValidator(1, 1000, this));
    buttonGroup = new QButtonGroup ;
    ui->subComboBox->setCurrentIndex(5);
    ui->subComboBox->setEnabled(false);
    InitCheckPointBox();
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
    item.SetIFVal(buttonGroup->checkedId());
    item.SetSVal(ui->subComboBox->currentIndex());
    item.SetDVal(ui->returnLineEdit->TransThisTextToThisInt());
    ret.append(item);
    return ret;
}

void ICHCConditionPage::InitCheckPointBox()
{
    buttonGroup->addButton(ui->defectiveProductsBox,0);
    buttonGroup->addButton(ui->x043checkBox,1);
    buttonGroup->addButton(ui->x044checkBox,2);
    buttonGroup->addButton(ui->tryProductBox,3);
    buttonGroup->addButton(ui->samplingBox,4);

    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    for(int i = 0; i != buttons.size(); ++i)
    {
        buttons[i]->setCheckable(true);
        connect(buttons[i],
                SIGNAL(clicked()),
                this,
                SLOT(BoxClicked()));
    }
    buttonGroup->setExclusive(true);
}

void ICHCConditionPage::BoxClicked()
{
    int temp = buttonGroup->checkedId();
    switch(temp)
    {
    case 0:
        ui->subComboBox->setCurrentIndex(5);
        ui->subComboBox->setEnabled(false);
        break;
    case 3:
        ui->subComboBox->setCurrentIndex(6);
        ui->subComboBox->setEnabled(false);
        break;
    case 4:
        ui->subComboBox->setCurrentIndex(7);
        ui->subComboBox->setEnabled(false);
        break;
    default:
        ui->subComboBox->setCurrentIndex(0);
        ui->subComboBox->setEnabled(true);
        break;
    }
}
