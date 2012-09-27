#include "ictipswidget.h"
#include "ui_ictipswidget.h"

ICTipsWidget::ICTipsWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICTipsWidget)
{
    ui->setupUi(this);
}

ICTipsWidget::ICTipsWidget(const QString &msg, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ICTipsWidget)
{
    ui->setupUi(this);
    SetMessage(msg);
}

ICTipsWidget::~ICTipsWidget()
{
    delete ui;
}

void ICTipsWidget::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICTipsWidget::SetMessage(const QString &msg)
{
    ui->msgLabel->setText(msg);
}
