#include "icaxiskeyboard.h"
#include "ui_icaxiskeyboard.h"
#include "ickeyboard.h"


static QMap<QPushButton*, int> oneHitButtonToKey;
static QMap<QPushButton*, int> conHitButtonToKey;

ICAxisKeyboard::ICAxisKeyboardPTR ICAxisKeyboard::instance_ = NULL;
ICAxisKeyboard::ICAxisKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICAxisKeyboard)
{
    ui->setupUi(this);

    oneHitButtonToKey.insert(ui->upButton, ICKeyboard::FB_Up);
    oneHitButtonToKey.insert(ui->donwButton, ICKeyboard::FB_Down);

    conHitButtonToKey.insert(ui->xMInus, ICKeyboard::VFB_X1Sub);
    conHitButtonToKey.insert(ui->xPluse, ICKeyboard::VFB_X1Add);
    conHitButtonToKey.insert(ui->yMinus, ICKeyboard::VFB_Y1Sub);
    conHitButtonToKey.insert(ui->yPluse, ICKeyboard::VFB_Y1Add);
    conHitButtonToKey.insert(ui->zMinus, ICKeyboard::VFB_ZSub);
    conHitButtonToKey.insert(ui->zPluse, ICKeyboard::VFB_ZAdd);

    QList<QPushButton*> buttons = oneHitButtonToKey.keys();
    for(int i = 0; i != buttons.size(); ++i)
    {
        connect(buttons.at(i),
                SIGNAL(clicked()),
                SLOT(OnOneHitKeyPressed()));
    }
    buttons = conHitButtonToKey.keys();
    for(int i = 0; i != buttons.size(); ++i)
    {
        connect(buttons.at(i),
                SIGNAL(pressed()),
                SLOT(OnConHitKeyPressed()));
        connect(buttons.at(i),
                SIGNAL(released()),
                SLOT(OnConHitKeyRelease()));
    }
}

ICAxisKeyboard::~ICAxisKeyboard()
{
    delete ui;
}

void ICAxisKeyboard::changeEvent(QEvent *e)
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

void ICAxisKeyboard::SmartShow(const QWidget *caller)
{
    this->show();
}

void ICAxisKeyboard::OnOneHitKeyPressed()
{
    ICKeyboard::Instace()->SetKeyValue(oneHitButtonToKey.value(qobject_cast<QPushButton*>( sender())));
}

void ICAxisKeyboard::OnConHitKeyPressed()
{
    ICKeyboard::Instace()->SetKeyValue(conHitButtonToKey.value(qobject_cast<QPushButton*>( sender())));
    ICKeyboard::Instace()->SetPressed(true);

}

void ICAxisKeyboard::OnConHitKeyRelease()
{
    ICKeyboard::Instace()->SetPressed(false);
}

//void ICAxisKeyboard::on_manualMode_toggled(bool checked)
//{
//    if(checked)
//    {
//        ICKeyboard::Instace()->SetSwitchValue(ICKeyboard::KS_ManualStatu);
//    }
//}

//void ICAxisKeyboard::on_stopMode_toggled(bool checked)
//{
//    if(checked)
//    {
//        ICKeyboard::Instace()->SetSwitchValue(ICKeyboard::KS_StopStatu);
//    }
//}

//void ICAxisKeyboard::on_autoMode_toggled(bool checked)
//{
//    if(checked)
//    {
//        ICKeyboard::Instace()->SetSwitchValue(ICKeyboard::KS_AutoStatu);
//    }
//}

void ICAxisKeyboard::on_close_clicked()
{
    this->hide();
}
