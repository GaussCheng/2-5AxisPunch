#include "virtualnumerickeypaddialog.h"
#include "ui_virtualnumerickeypaddialog.h"

#include <QToolButton>
#include <QSignalMapper>

#include <QDebug>

QScopedPointer<VirtualNumericKeypadDialog> VirtualNumericKeypadDialog::instance_;
VirtualNumericKeypadDialog::VirtualNumericKeypadDialog(QWidget *parent) :
    QDialog(parent),
    numerickSignalMapper_(new QSignalMapper(this)),
    ui(new Ui::VirtualNumericKeypadDialog)
{
    ui->setupUi(this);

    QFont font = ui->displayLineEdit->font();
    font.setPointSize(font.pointSize() + 8);
    ui->displayLineEdit->setFont(font);

    ui->displayLineEdit->setFocus();
    ui->okToolButton->setDefault(true);

    QList<QAbstractButton *> buttonList = ui->numericButtonGroup->buttons();
    foreach(QAbstractButton * absButton, buttonList)
    {
        connect(absButton,
                SIGNAL(clicked()),
                numerickSignalMapper_,
                SLOT(map()));
        numerickSignalMapper_->setMapping(absButton, absButton->text());
    }

    connect(numerickSignalMapper_,
            SIGNAL(mapped(QString)),
            this,
            SLOT(KeyboardClicked(QString)));

    ui->displayLineEdit->setValidator(new QDoubleValidator(ui->displayLineEdit));
}

VirtualNumericKeypadDialog::~VirtualNumericKeypadDialog()
{
    delete ui;
}

void VirtualNumericKeypadDialog::changeEvent(QEvent *e)
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

void VirtualNumericKeypadDialog::ResetDisplay()
{
//    ui->displayLineEdit->setText(displayValue);
    ui->displayLineEdit->clear();
    ui->displayLineEdit->setFocus();
}

void VirtualNumericKeypadDialog::KeyboardClicked(QString buttonNum)
{
    if(ui->displayLineEdit->text().contains( QChar('.') )
        && buttonNum == tr("."))
    {
        return;
    }

    int cursorPosition = ui->displayLineEdit->cursorPosition();
    QString displayValue = ui->displayLineEdit->text();
    displayValue.insert(cursorPosition, buttonNum);

    ui->displayLineEdit->setText(displayValue);
    ui->displayLineEdit->setCursorPosition(++cursorPosition);
    ui->displayLineEdit->setFocus();
}

void VirtualNumericKeypadDialog::on_clearToolButton_clicked()
{
    ui->displayLineEdit->setText(QString());
}

void VirtualNumericKeypadDialog::on_okToolButton_clicked()
{
    if(!ui->displayLineEdit->text().isEmpty())
    {
        emit EnterComplete(ui->displayLineEdit->text());
    }
    else
    {
        emit EnterComplete("0");
    }
    this->accept();
}

void VirtualNumericKeypadDialog::on_backspaceToolButton_clicked()
{
    QString displayString = ui->displayLineEdit->text();
    int cursorPosition = ui->displayLineEdit->cursorPosition() - 1;
    displayString.remove(cursorPosition, 1);
    ui->displayLineEdit->setText(displayString);
    ui->displayLineEdit->setCursorPosition(cursorPosition);
    ui->displayLineEdit->setFocus();
}

void VirtualNumericKeypadDialog::on_delToolButton_clicked()
{
    QString displayString = ui->displayLineEdit->text();
    int cursorPosition = ui->displayLineEdit->cursorPosition();
    displayString.remove(cursorPosition, 1);
    ui->displayLineEdit->setText(displayString);
    ui->displayLineEdit->setCursorPosition(cursorPosition);
    ui->displayLineEdit->setFocus();
}

void VirtualNumericKeypadDialog::on_cancelToolButton_clicked()
{
    this->accept();
}

void VirtualNumericKeypadDialog::on_minusToolButton_clicked()
{
    QString displayString = ui->displayLineEdit->text();
    if(displayString.left(1) != "-")
    {
        displayString.prepend("-");
        ui->displayLineEdit->setText(displayString);
    }
}

void VirtualNumericKeypadDialog::on_addButton_clicked()
{
    QString displayString = ui->displayLineEdit->text();
    if(displayString.left(1) != "+")
    {
        displayString.prepend("+");
        ui->displayLineEdit->setText(displayString);
    }
}
