#ifndef ICTIMEEDIT_H
#define ICTIMEEDIT_H

#include "iclineedit.h"
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICTimeEdit : public ICLineEdit
{
    Q_OBJECT
public:
    explicit ICTimeEdit(QWidget *parent = 0);
    QSize sizeHint() const { return QSize(76, 32);}
    QSize minimumSizeHint() const { return sizeHint();}
    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    int Hour() const { return text().left(2).toUInt();}
    int Minute() const { return text().right(2).toUInt();}
    QString Time() const;
    void SetHour(const QString& hourText);
    void SetHour(int hour);
    void SetMinute(const QString& minText);
    void SetMinute(int minute);
    void SetTime(const QString& timeText);

    int Value() const { return Hour() * 100 + Minute();}

    void SetDecimals(int d){}

//    double LastValue() const { return oldText_;}
    QString LastValueStr() const;

    
signals:
    void ValueTextChanged(const QString&);
    
public slots:
    void SetValue(int value);

private slots:
    void OnEditFinished();

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:
    QString localText_;
    QString oldText_;
    
};

inline void ICTimeEdit::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}


#endif // ICTIMEEDIT_H
