#include "icmessagebox.h"
#include <QKeyEvent>
#include <QAbstractButton>
#include <QList>
#include <QApplication>
#include <QDebug>
#include "mainframe.h"

ICMessageBox::ICMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
}

void ICMessageBox::keyPressEvent(QKeyEvent *e)
{
//    e->ignore();
    QKeyEvent* ke = new QKeyEvent(*e);
    qApp->postEvent(icMainFrame, ke);
    qDebug("messabox key");

    this->close();
}

int ICMessageBox::ICWarning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
////    if(parent == NULL)
//        parent = icMainFrame;
    ICMessageBox *box = new ICMessageBox(parent);
    box->setWindowFlags(box->windowFlags()  | Qt::WindowStaysOnTopHint);
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

//    box->show();
    return box->exec();
//    if(parent != NULL)
//        parent->activateWindow();
    qDebug("messabox");
    return box->standardButton(box->clickedButton());
}
