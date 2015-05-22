#include "ictimeedit.h"

#include <QKeyEvent>


//QList<int> numericKeys_(QList<int>()<<Qt::Key_0<<Qt::Key_1<<Qt::Key_2
//                                    <<Qt::Key_3<<Qt::Key_4<<Qt::Key_5<<Qt::Key_6
//                                    <<Qt::Key_7<<Qt::Key_8<<Qt::Key_9<<Qt::Key_0
//                                   <<Qt::Key_Backspace);

const static int kTimeLength = 4;
static QString FormatToTimeText(const QString& text)
{
    if(text.size() > kTimeLength)
    {
        return text;
    }
    QString ret = text;
    while(ret.size() < kTimeLength)
    {
        ret = "0" + ret;
    }
    ret = ret.insert(2, ":");
    return ret;

}

ICTimeEdit::ICTimeEdit(QWidget *parent) :
    ICLineEdit(parent)
{
    SetTime(FormatToTimeText("0000"));
    setAlignment(Qt::AlignRight);
    connect(this,
            SIGNAL(editingFinished()),
            this,
            SLOT(OnEditFinished()));
    setFocusPolicy(Qt::NoFocus);
}


void ICTimeEdit::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_Return)
    {
        //        selectAll();
        setSelection(0, text().length());
        emit editingFinished();
        e->accept();
        return;
    }
    if(!numericKeys_.contains(key))
    {
        setSelection(0, text().length());
        emit editingFinished();
        e->ignore();
        return;
    }
    if(this->hasSelectedText())
    {
        localText_.clear();
    }
    if(key == Qt::Key_Backspace)
    {
        localText_.chop(1);
    }
    else if(localText_.size() == 4)
    {
        return;
    }
    else
    {
        localText_ += QString::number(key - Qt::Key_0);
    }
    setText(FormatToTimeText(localText_));
    if(localText_ == "0000")
    {
        localText_.clear();
    }
}

void ICTimeEdit::focusInEvent(QFocusEvent *e)
{
    setSelection(0, text().length());
    QLineEdit::focusInEvent(e);
}

void ICTimeEdit::focusOutEvent(QFocusEvent *e)
{
    if(Hour() > 23)
    {
        localText_ = localText_.remove(0, 2);
        localText_  = "23" + localText_;
    }
    if(Minute() > 59)
    {
        localText_.chop(2);
        localText_ += "59";
    }
    setText(FormatToTimeText(localText_));
    QLineEdit::focusOutEvent(e);
}

QString ICTimeEdit::Time() const
{
    QString ret = localText_;
    while(ret.size() < kTimeLength)
    {
        ret = "0" + ret;
    }
    return ret;
}

void ICTimeEdit::SetTime(const QString &timeText)
{
    localText_ = timeText;
    oldText_ = localText_;
    setText(FormatToTimeText(localText_));
}

void ICTimeEdit::SetHour(const QString &hourText)
{
    localText_ = localText_.remove(0, 2);
    localText_  = hourText + localText_;
//    localText_ = FormatToTimeText(localText_);
    oldText_ = localText_;
    setText(FormatToTimeText(localText_));
}

void ICTimeEdit::SetHour(int hour)
{
    QString ret = QString::number(hour);
    while(ret.size() < 2)
    {
        ret = "0" + ret;
    }
    SetHour(ret);
}

void ICTimeEdit::SetMinute(const QString &minText)
{
    localText_.chop(2);
    localText_ += minText;
//    localText_ = FormatToTimeText(localText_);
    oldText_ = localText_;
    setText(FormatToTimeText(localText_));
}

void ICTimeEdit::SetMinute(int minute)
{
    QString ret = QString::number(minute);
    while(ret.size() < 2)
    {
        ret = "0" + ret;
    }
    SetMinute(ret);
}

void ICTimeEdit::OnEditFinished()
{
    if(oldText_ != localText_)
    {
        oldText_ = localText_;
        emit ValueTextChanged(localText_);
    }
}

void ICTimeEdit::SetValue(int value)
{
    SetHour(value / 100);
    SetMinute(value % 100);
}

QString ICTimeEdit::LastValueStr() const
{
    return FormatToTimeText(oldText_);
}
