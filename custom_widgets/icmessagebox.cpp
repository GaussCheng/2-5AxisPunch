#include "icmessagebox.h"
#include <QKeyEvent>
#include <QAbstractButton>
#include <QList>
#include <QApplication>
#include <QDebug>

ICMessageBox::ICMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
}

void ICMessageBox::keyPressEvent(QKeyEvent *e)
{
//    e->ignore();
    QKeyEvent* ke = new QKeyEvent(*e);
    qApp->postEvent(this->parentWidget(), ke);
    this->close();
}

int ICMessageBox::ICWarning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    ICMessageBox *box = new ICMessageBox(parent);
    box->setStandardButtons(buttons);
    box->setDefaultButton(defaultButton);
    box->setWindowTitle(title);
    box->setText(text);
    QList<QAbstractButton *> bs = box->buttons();
    for(int i = 0; i !=bs.size(); ++i)
    {
        bs[i]->setFocusPolicy(Qt::NoFocus);
    }
    box->setWindowModality(Qt::NonModal);
    box->setAttribute(Qt::WA_DeleteOnClose);

    box->show();
//    if(parent != NULL)
//        parent->activateWindow();
    return box->standardButton(box->clickedButton());
}
