#include "iccombobox.h"
#include "iclineedit.h"

#include <QKeyEvent>
#include <QCompleter>
#include <QDebug>
#include <QAbstractItemView>
#include <QApplication>

ICComboBox::ICComboBox(QWidget *parent) :
    QComboBox(parent)
{
    //    setStyleSheet(style_);
    //    setMinimumSize(sizeHint());
    //    setMaximumSize(sizeHint());
    //    QFont f = this->font();
    //    f.setPointSize(13);
    //    SetTextFont(f);
    setFocusPolicy(Qt::NoFocus);
    this->view()->installEventFilter(this);
    //    lineEdit()->setAlignment(Qt::AlignRight);
    //    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    //    setAttribute(Qt::WA_OpaquePaintEvent);
}

void ICComboBox::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return && completer() == NULL)
    {
        showPopup();
        this->view()->setCurrentIndex(QModelIndex());
        e->accept();
        return;
    }
    //    if(completer() != NULL && e->key() == Qt::Key_Return)
    //    {
    //        if(completer()->popup() == NULL)
    //        {
    //            showPopup();
    //            this->view()->setCurrentIndex(QModelIndex());
    //            e->accept();
    //            return;
    //        }
    //        else if(!completer()->popup()->isVisible())
    //        {
    //            showPopup();
    //            this->view()->setCurrentIndex(QModelIndex());
    //            e->accept();
    //            return;
    //        }
    //    }
    //    if(e->key() >= Qt::Key_A && e->key() <= Qt::Key_Z)
    e->ignore();
    //    QComboBox::keyPressEvent(e);
}

void ICComboBox::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
    //    QComboBox::keyReleaseEvent(e);
}


void ICComboBox::focusInEvent(QFocusEvent *e)
{
    oldValue_ = currentIndex();
    QComboBox::focusInEvent(e);
}

bool ICComboBox::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonRelease)
    {
        setFocus();
    }
    if((e->type() == QEvent::KeyPress) &&  (o == this->view()))
    {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        if(ke->key() != Qt::Key_Up &&
                ke->key() != Qt::Key_Down &&
                ke->key() != Qt::Key_Return &&
                ke->key() != Qt::Key_Home &&
                ke->key() != Qt::Key_End)
        {
            hidePopup();
            e->accept();
            return true;
        }
        else if(ke->key() == Qt::Key_Up)
        {
            QAbstractItemView* view = this->view();
            if(view->currentIndex().row() == 0)
            {
                QKeyEvent *wrapEvent = new QKeyEvent(ke->type(),
                                                     Qt::Key_End,
                                                     ke->modifiers(),
                                                     ke->text(),
                                                     ke->isAutoRepeat(),
                                                     ke->count());
                e->accept();
                qApp->sendEvent(view, wrapEvent);
                return true;
            }
        }
        else if(ke->key() == Qt::Key_Down)
        {
            QAbstractItemView* view = this->view();
            if(view->currentIndex().row() == this->count() - 1)
            {
                QKeyEvent *wrapEvent = new QKeyEvent(ke->type(),
                                                     Qt::Key_Home,
                                                     ke->modifiers(),
                                                     ke->text(),
                                                     ke->isAutoRepeat(),
                                                     ke->count());
                e->accept();
                qApp->sendEvent(view, wrapEvent);
                return true;
            }
        }

    }
    return QComboBox::eventFilter(o, e);
}

void ICComboBox::SetComboBoxSize(QSize &size)
{
    setMinimumSize(size);
    setMaximumSize(size);
}
