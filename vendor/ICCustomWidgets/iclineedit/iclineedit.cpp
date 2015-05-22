#include "iclineedit.h"
#include <QKeyEvent>
#include <QDebug>

static bool IsDoubleValid(double v, int decimals, const QDoubleValidator* validator)
{
    const int p = qPow(10, decimals);
    int vv = v * p;
    int b = validator->bottom() * p;
    int t = validator->top() * p;
    if(vv < b || vv > t)
    {
        return false;
    }
    return true;
}

QList<int> ICLineEdit::numericKeys_(QList<int>()<<Qt::Key_0<<Qt::Key_1<<Qt::Key_2
                                    <<Qt::Key_3<<Qt::Key_4<<Qt::Key_5<<Qt::Key_6
                                    <<Qt::Key_7<<Qt::Key_8<<Qt::Key_9<<Qt::Key_0
                                    <<Qt::Key_Backspace);

//QList<void*> ICLineEdit::objectPool_;
ICLineEdit::ICLineEdit(QWidget *parent) :
    QLineEdit(parent),
    isOnlyNumeric_(true),
    decimals_(0),
    doubleValidator_(NULL),
    specialValue_(-1),
    tipsFormat_(tr("Max %1 Min %2")),
    isBlockValidator_(false)
{
    oldValue_ = -1;
    lastValue_ = -1;
    WrapText_("0");
    setAlignment(Qt::AlignRight);
//    setFrame(false);
//    QFont f = font();
//    f.setPointSize(13);
//    SetTextFont(f);
    setMinimumSize(sizeHint());
    setMaximumSize(sizeHint());
    connect(this,
            SIGNAL(editingFinished()),
            this,
            SLOT(OnEditFinished()));
    doubleValidator_ = new QDoubleValidator();
    doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    doubleValidator_->setBottom(0);
    doubleValidator_->setTop(100);
    SetValidator(doubleValidator_);

//    setFocusPolicy(Qt::NoFocus);
//    this->setAttribute(Qt::WA_OpaquePaintEvent);
//    this->setAttribute(Qt::WA_PaintOnScreen);
}

ICLineEdit::~ICLineEdit()
{
    if(doubleValidator_ != NULL)
    {
        delete doubleValidator_;
    }
}

void ICLineEdit::changeEvent(QEvent *e)
{
    QLineEdit::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        tipsFormat_ = tr("Max %1 Min %2");
        UpdateTips_();
    }
        break;
    default:
        break;
    }
}

void ICLineEdit::focusInEvent(QFocusEvent *e)
{
    //    selectAll();
    setSelection(Prefix().size(), text().length() - Suffix().size());
    lastValue_ = oldValue_;
    wrapText_.clear();

    setFocus();
    QLineEdit::focusInEvent(e);
}

void ICLineEdit::focusOutEvent(QFocusEvent *e)
{
    QString tmpText = text();
    int tmpInt = 0;
    if(validator() != NULL)
    {
        if(validator()->validate(tmpText,tmpInt) != QValidator::Acceptable)
        {
            OnEditFinished();
        }
    }
    deselect(); //Qt::ActiveWindowFocusReasonå’ŒQt::PopupFocusReason
    QLineEdit::focusOutEvent(e);
}

void ICLineEdit::keyPressEvent(QKeyEvent *e)
{
    if(!IsOnlyNumeric())
    {
        QLineEdit::keyPressEvent(e);
        return;
    }
    int key = e->key();
    if(key == Qt::Key_Return)
    {
        //        selectAll();
        setSelection(Prefix().size(), text().length() - Suffix().size());
        emit editingFinished();
        e->accept();
        return;
    }
    if(!numericKeys_.contains(key))
    {
        setSelection(Prefix().size(), text().length() - Suffix().size());
        emit editingFinished();
        e->ignore();
        return;
    }
    if(key == Qt::Key_Backspace)
    {
        wrapText_.chop(1);
        if(wrapText_.isEmpty())
        {
            if(!SpecialValueText().isEmpty())
            {
                WrapText_(SpecialValueText());
            }
            else
            {
                WrapText_(QString("%1").arg((wrapText_.toUInt() / qPow(10, Decimals())), 0, 'f', Decimals()));
                //WrapSuffix_(QString("%1").arg((double)0, 0, 'f', Decimals()));//wrapText_åœ¨è¿™é‡Œå·²ç»æ˜¯Emptyäº†
            }
            selectAll();
            return;
        }
    }
    else
    {
        QString text = wrapText_;
        text += key;
        double tv = text.toDouble() / qPow(10, Decimals());
        const QIntValidator* iv = qobject_cast<const QIntValidator*>(this->validator());
        const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(this->validator());
//        qDebug()<<text;
        if(doubleValidator_ != NULL)
        {
//            qDebug()<<"double"<<doubleValidator_->top();
            if(tv > doubleValidator_->top())
            {
                return;
            }
        }
        else if(iv != NULL)
        {
//            qDebug()<<"iv"<<iv->top();
            if(tv > iv->top())
            {
                return;
            }
        }
        else if(dv != NULL)
        {
//            qDebug()<<"dv"<<dv->top();
            if(tv > dv->top())
            {
                return;
            }
        }
        wrapText_ += (char)key;
    }
    if(wrapText_.length() > 9)
    {
        wrapText_ = "0";
        WrapText_(QString("%1").arg((wrapText_.toUInt() / qPow(10, Decimals())), 0, 'f', Decimals()));
        selectAll();
        return;
    }
    WrapText_(QString("%1").arg((wrapText_.toUInt() / qPow(10, Decimals())), 0, 'f', Decimals()));
    setCursorPosition(text().length() - Suffix().size() - Prefix().size());
}

void ICLineEdit::mousePressEvent(QMouseEvent *e)
{
    setFocus();
    e->accept();
//    QWidget::mousePressEvent(e);
}

void ICLineEdit::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
}

void ICLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    e->accept();
}

#ifdef Q_WS_QWS
void ICLineEdit::inputMethodEvent(QInputMethodEvent *event)
{
    QLineEdit::inputMethodEvent(event);
    clearFocus();
}
#endif



//void ICLineEdit::SetAddValue()
//{
//    double value = Value();
//    value++ ;
//    const QIntValidator* iv = qobject_cast<const QIntValidator*>(this->validator());
//    const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(this->validator());
//    if(doubleValidator_ != NULL)
//    {
//        if(value > doubleValidator_->top())
//        {
//            return;
//        }
//    }
//    else if(iv != NULL)
//    {
//        if(value > iv->top())
//        {
//            return;
//        }
//    }
//    else if(dv != NULL)
//    {
//        if(value > dv->top())
//        {
//            return;
//        }
//    }
//    wrapText_ = QString("%1").arg(value);
//}
//void ICLineEdit::SetSubValue()
//{
//    double value = Value();
//    value-- ;
//    const QIntValidator* iv = qobject_cast<const QIntValidator*>(this->validator());
//    const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(this->validator());
//    if(doubleValidator_ != NULL)
//    {
//        if(value < doubleValidator_->bottom())
//        {
//            return;
//        }
//    }
//    else if(iv != NULL)
//    {
//        if(value < iv->bottom())
//        {
//            return;
//        }
//    }
//    else if(dv != NULL)
//    {
//        if(value < dv->bottom())
//        {
//            return;
//        }
//    }
//
//    wrapText_ = QString("%1").arg(value);
//
//}


void ICLineEdit::OnEditFinished()
{
    if(!IsOnlyNumeric())
    {
        emit ValueTextChanged(text().mid(Prefix().size(), text().length() - Suffix().size() - Prefix().size()));
        return;
    }
    wrapText_.clear();
    if(text() == Prefix() + SpecialValueText() + Suffix())
    {
        if(oldValue_ != SpecialValue())
        {
            oldValue_ = SpecialValue();
            emit ValueTextChanged(SpecialValueText());
            return;
        }
        return;
    }
    QString valueText = text().mid(Prefix().size(), text().length() - Suffix().size() - Prefix().size());
    double now = valueText.toDouble();
    if(fabs(oldValue_ - now) >= 1 / qPow(10, Decimals() + 1))    //åˆ¤æ–­ç”¨æˆ·å‘thiså¯¹è±¡è¾“å…¥çš„å€¼ä¸Žè¾“å…¥å‰çš„å€¼æ˜¯å¦ç›¸åŒ
    {
//        qDebug()<<"TestLine:"<<oldValue_<<now;
        const QValidator* validator = (doubleValidator_ != NULL) ? doubleValidator_ : this->validator();
        const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(validator);
        if(dv != NULL)
        {
            if(!IsDoubleValid(now, Decimals(), dv))
            {
//                qDebug("return");
                SetValue(oldValue_);
                return;
            }
        }
        else if(validator != NULL)
        {
            QString text = this->text().mid(Prefix().size(), this->text().length() - Suffix().size() - Prefix().size());
            int tmp = 0;
            if(validator->validate(text, tmp) != QValidator::Acceptable)
            {
                SetValue(oldValue_);
                return;
            }
        }
        lastValue_ = oldValue_;
        oldValue_ = now;
        emit ValueTextChanged(valueText);
        emit ValueChanged(now);
    }
}

void ICLineEdit::WrapSuffix_(const QString &text)
{
    QLineEdit::setText(text + Suffix());
}

void ICLineEdit::WrapPrefix_(const QString &text)
{
    QLineEdit::setText(Prefix() + text);
}

void ICLineEdit::WrapText_(const QString &text)
{
    QLineEdit::setText(Prefix() + text + Suffix());
}

double ICLineEdit::Value()
{
    if(!IsOnlyNumeric())
    {
        return -1;
    }
    return text().mid(Prefix().size(), text().size() - Suffix().size() - Prefix().size()).toDouble();
}

void ICLineEdit::SetValue(double value)
{
    if(doubleValidator_ != NULL && !isBlockValidator_)
    {
        if(value < doubleValidator_->bottom() || value > doubleValidator_->top())
        {
            return;
        }
    }
    if(value == SpecialValue() / qPow(10, Decimals()))
    {
        WrapText_(SpecialValueText());
    }
    else
    {
        WrapText_(QString("%1").arg(value, 0, 'f', Decimals()));
    }
    if(oldValue_ != value)
    {
        lastValue_ = oldValue_;
        oldValue_ = value;
        emit ValueChanged(value);
//        emit ValueTextChanged(QString::number(value));
    }
}

void ICLineEdit::SetValue(int value)
{
    if(doubleValidator_ != NULL && !isBlockValidator_)
    {
        if(value < doubleValidator_->bottom() || value > doubleValidator_->top())
        {
            return;
        }
    }
    if(value == SpecialValue())
    {
        WrapText_(SpecialValueText());
    }
    else
    {
        WrapText_(QString("%1").arg(value / qPow(10, Decimals()), 0, 'f', Decimals()));
    }
    if(oldValue_ != value)
    {
        lastValue_ = oldValue_;
        oldValue_ = value;
        emit ValueChanged(oldValue_);
//        emit ValueTextChanged(QString::number(oldValue_));
    }
}
