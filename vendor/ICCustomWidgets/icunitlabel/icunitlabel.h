#ifndef ICUNITLABEL_H
#define ICUNITLABEL_H

#include <QLabel>
#include <qmath.h>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICUnitLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString suffix READ Suffix WRITE setSuffix)


public:
    explicit ICUnitLabel(QWidget *parent = 0);
    QString Suffix() const { return suffix_;}

    void SetValue(double value, int decimal);

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    void ChangeText(const QString& text);

signals:

public slots:
    void setSuffix(const QString& text);

protected:
//    void paintEvent(QPaintEvent *e);

private:
    double oldValue_;
    QString suffix_;

};
inline void ICUnitLabel::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

inline void ICUnitLabel::setSuffix(const QString &text)
{
    setText(this->text().left(this->text().size() - Suffix().size()) + text);
    suffix_ = text;
}

inline void ICUnitLabel::SetValue(double value, int decimal)
{
    if(decimal < 0)
    {
        decimal = 0;
    }
    if(oldValue_ != value)
    {
        if(decimal == 0)
        {
            setText(QString("%1").arg(value) + Suffix());
        }
        else
        {
            setText(QString("%1").arg(value / qPow(10, 1), 0, 'f', decimal) + suffix_);
        }
        oldValue_ = value;
    }
}

inline void ICUnitLabel::ChangeText(const QString &text)
{
    QString toshow = text + Suffix();
    if(this->text() != (toshow))
    {
        setText(toshow);
    }
}

#endif // ICUNITLABEL_H
