#include <QKeyEvent>
#include <QPainter>
#include "icinputmethodedit.h"
#include "containerinit.h"

#ifdef Q_WS_QWS

#include <QInputMethodEvent>

#endif

QMultiMap<int, QChar> ICInputMethodEdit::keyToCharMap_ = CreateMultiMap<int, QChar>(Qt::Key_7, 's')
(Qt::Key_7, 'r')(Qt::Key_7, 'q')(Qt::Key_7, 'p')(Qt::Key_7, '7')
(Qt::Key_8, 'v')(Qt::Key_8, 'u')(Qt::Key_8, 't')(Qt::Key_8, '8')
(Qt::Key_9, 'z')(Qt::Key_9, 'y')(Qt::Key_9, 'x')(Qt::Key_9, 'w')(Qt::Key_9, '9')
(Qt::Key_4, 'i')(Qt::Key_4, 'h')(Qt::Key_4, 'g')(Qt::Key_4, '4')
(Qt::Key_5, 'l')(Qt::Key_5, 'k')(Qt::Key_5, 'j')(Qt::Key_5, '5')
(Qt::Key_6, 'o')(Qt::Key_6, 'n')(Qt::Key_6, 'm')(Qt::Key_6, '6')
(Qt::Key_1, ')')(Qt::Key_1, '(')(Qt::Key_1, '-')(Qt::Key_1, '1')
(Qt::Key_2, 'c')(Qt::Key_2, 'b')(Qt::Key_2, 'a')(Qt::Key_2, '2')
(Qt::Key_3, 'f')(Qt::Key_3, 'e')(Qt::Key_3, 'd')(Qt::Key_3, '3')
(Qt::Key_0, '&')(Qt::Key_0, '#')(Qt::Key_0, '+')(Qt::Key_0, '0');

QMultiMap<int, QChar> ICInputMethodEdit::digitalKeyToCharMap_ = CreateMultiMap<int, QChar>(Qt::Key_7, '7')
(Qt::Key_8, '8')
(Qt::Key_9, '9')
(Qt::Key_4, '4')
(Qt::Key_5, '5')
(Qt::Key_6, '6')
(Qt::Key_1, '1')
(Qt::Key_2, '2')
(Qt::Key_3, '3')
(Qt::Key_0, '0');
ICInputMethodEdit::ICInputMethodEdit(QWidget *parent) :
    QLineEdit(parent),
    oldKey_(-1),
    currentKeyChar_(-1),
    hasSelected_(false),
    echoMode_(EchoMode_Normal),
    isDigital_(false),
    isUpper_(false)
{
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(OnEditTimeOut()));
//    setFrame(false);
    QFont f = font();
    f.setPointSize(12);
    SetTextFont(f);
    setMinimumSize(sizeHint());
    setMaximumSize(sizeHint());
    setFocusPolicy(Qt::NoFocus);
}

void ICInputMethodEdit::mousePressEvent(QMouseEvent *e)
{
    setFocus();
    e->accept();
//    QWidget::mousePressEvent(e);
}

void ICInputMethodEdit::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
}
void ICInputMethodEdit::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_Return)
    {
        if(completer() == NULL)
        {
            e->accept();
            return;
        }
        QLineEdit::keyPressEvent(e);
        return;
    }
    if(key == Qt::Key_Backspace)
    {
        actualText_.chop(1);
        hasSelected_ = false;
        currentKeyChar_ = -1;
        timer_.stop();
        QLineEdit::keyPressEvent(e);
        return;
    }
    const QList<QChar> keyChars = (IsDigitalOnly() == true) ? digitalKeyToCharMap_.values(key) : keyToCharMap_.values(key);
    if(keyChars.isEmpty())
    {
        e->ignore();
        return;
    }
    e->accept();
    if(key != oldKey_ || IsDigitalOnly())
    {
        oldKey_ = key;
        currentKeyChar_ = -1;
        hasSelected_ = false;
        if(GetEchoMode() == EchoMode_PasswordAfter)
        {
            setText(QString(actualText_.size(), '*'));
        }
    }
    currentKeyChar_ = (currentKeyChar_ + 1) % keyChars.size();
    if(hasSelected_)
    {
        cursorBackward(true);
        if(isUpper_)
            insert(keyChars.at(currentKeyChar_).toUpper());
        else
            insert(keyChars.at(currentKeyChar_));

        actualText_[actualText_.size() - 1] = keyChars.at(currentKeyChar_);
        cursorForward(false);
    }
    else
    {
        if(isUpper_)
            insert(keyChars.at(currentKeyChar_).toUpper());
        else
            insert(keyChars.at(currentKeyChar_));
        actualText_ += keyChars.at(currentKeyChar_);
    }
    hasSelected_ = true;

    timer_.stop();
    //    cursorBackward(true);
    timer_.start(1000);
}

void ICInputMethodEdit::OnEditTimeOut()
{
    deselect();
    hasSelected_ = false;
    //    cursorForward(false);
    currentKeyChar_ = -1;
    if(GetEchoMode() == EchoMode_PasswordAfter)
    {
        setText(QString(actualText_.size(), '*'));
    }
    timer_.stop();
}

void ICInputMethodEdit::Clear()
{
    actualText_.clear();
    this->clear();
}

#ifdef Q_WS_QWS
void ICInputMethodEdit::inputMethodEvent(QInputMethodEvent *e)
{
    actualText_ += e->commitString();
    hasSelected_ = false;
    timer_.stop();
    //    cursorBackward(true);
    timer_.start(1000);
}
#endif
