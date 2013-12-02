#include "icaxiskeyboard.h"
#include "ui_icaxiskeyboard.h"

ICAxisKeyboard::ICAxisKeyboardPTR ICAxisKeyboard::instance_ = NULL;
ICAxisKeyboard::ICAxisKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICAxisKeyboard)
{
    ui->setupUi(this);
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
