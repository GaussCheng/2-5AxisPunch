#include "virtualkeyboarddialog.h"
#include "ui_virtualkeyboarddialog.h"

#include <QMouseEvent>
#include <QFont>

#include <QDebug>
#include "mainframe.h"

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
    QList<QPushButton *> objList = ui->buttonBoxWidget->findChildren<QPushButton*>();
    foreach(QObject *obj,objList)
    {
        QPushButton *button = qobject_cast<QPushButton *>(obj);
        connect(button,
                SIGNAL(clicked()),
                this,
                SLOT(KeyboardClicked()));
    }
    ui->display->installEventFilter(this);
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
#ifdef HC_SK_8
    this->setFixedSize(800, 356);
#else
    this->setFixedSize(640, 356);
#endif
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

void VirtualKeyboardDialog::keyPressEvent(QKeyEvent *e)
{
    QKeyEvent* ke = new QKeyEvent(*e);
    qApp->postEvent(icMainFrame, ke);
    this->reject();
}

bool VirtualKeyboardDialog::eventFilter(QObject *o, QEvent *e)
{
#ifdef Q_WS_QWS
    if(o == ui->display && e->type() == QEvent::KeyPress)
    {
        return true;
    }
#endif
    return QDialog::eventFilter(o, e);

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
