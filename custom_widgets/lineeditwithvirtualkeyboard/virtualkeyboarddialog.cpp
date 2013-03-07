#include "virtualkeyboarddialog.h"
#include "ui_virtualkeyboarddialog.h"

#include <QMouseEvent>
#include <QFont>

#include <QDebug>

QScopedPointer<VirtualKeyboardDialog> VirtualKeyboardDialog::instance_;

VirtualKeyboardDialog::VirtualKeyboardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VirtualKeyboardDialog)
{
    ui->setupUi(this);

//    this->setAttribute(Qt::WA_OpaquePaintEvent);
    this->setWindowTitle(tr("VirtualKeyboard"));

    QFont font = ui->display->font();
    font.setPointSize(font.pointSize() + 8);
    ui->display->setFont(font);

    ui->display->setFocus();

    ui->ok->setDefaultAction();
    QList<QPushButton *> objList = ui->buttonBoxWidget->findChildren<QPushButton*>();
    foreach(QObject *obj,objList)
    {
        QPushButton *button = qobject_cast<QPushButton *>(obj);
        connect(button,
                SIGNAL(clicked()),
                this,
                SLOT(KeyboardClicked()));
    }
}

VirtualKeyboardDialog::~VirtualKeyboardDialog()
{
    delete ui;
}

void VirtualKeyboardDialog::changeEvent(QEvent *e)
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

//public slots:
void VirtualKeyboardDialog::ResetDisplay()
{
    //qDebug()<<(ui->display == NULL);
    ui->display->clear();
//    ui->display->setFocus();
}

//private slots
void VirtualKeyboardDialog::on_cancel_clicked()
{
    ui->display->clear();
    this->reject();
}

void VirtualKeyboardDialog::on_backspace_clicked()
{
//    QString displayString = ui->display->text();
//    int cursorPosition = ui->display->cursorPosition() - 1;
//    displayString.remove(cursorPosition, 1);
//    ui->display->setText(displayString);
//    ui->display->setCursorPosition(cursorPosition);
//    ui->display->setFocus();
    ui->display->backspace();
}

void VirtualKeyboardDialog::on_ok_clicked()
{
//    emit EnterComplete(ui->display->text());
    this->accept();
}

void VirtualKeyboardDialog::KeyboardClicked()
{
    QPushButton * clickedButton = qobject_cast<QPushButton *>(sender());
    QChar addedValue = clickedButton->text()[0];
    if(addedValue.isLetter())
    {
        addedValue = addedValue.toAscii() + 0x20;
    }
    ui->display->insert(addedValue);
//    int cursorPosition = ui->display->cursorPosition();
//    QString disPlayValue = ui->display->text();
//    disPlayValue.insert(cursorPosition, addedValue);

//    ui->display->setText(disPlayValue);
//    ui->display->setCursorPosition(++cursorPosition);
//    ui->display->setFocus();
}

QString VirtualKeyboardDialog::GetCurrentText() const
{
    return ui->display->text();
}
