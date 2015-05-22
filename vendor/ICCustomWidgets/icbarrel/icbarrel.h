#ifndef ICBARREL_H
#define ICBARREL_H

#include <QFrame>
#include <QVarLengthArray>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICBarrel : public QFrame
{
    Q_OBJECT
public:
    explicit ICBarrel(QWidget *parent = 0);

    int BarrelCount() const { return barrelCount_;}
    void SetBarrelCount(int count);

    QString BarrelText(int index) { return QString::number(BarrelValue(index), 'f', 1);}
    double BarrelValue(int index);
    void SetBarrelValue(int index, double value);
    void SetBarrelText(int index, const QString& text);

    void SetBarrelBackgroundColor(int index, QColor color);
    void SetTailPic(const QPixmap& pic) { tail_ = pic; setMinimumHeight(head_.height());update();}
    void SetHeadPic(const QPixmap& pic) { head_ = pic; setMinimumHeight(head_.height());update();}
    void SetMidPic(const QPixmap& pic) { mid_ = pic; setMinimumHeight(head_.height());update();}

    QSize sizeHint() const { return QSize(70, 60);}
    QSize minimumSizeHint() const { return sizeHint();}

    void BindSpeicalValueText(int index, double val, const QString& text);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    int barrelCount_;
    const static int DefaultBarrelCount = 6;
    QVarLengthArray<double, DefaultBarrelCount> barrelValues_;
    QPixmap head_;
    QPixmap tail_;
    QPixmap mid_;
    QVarLengthArray<double, DefaultBarrelCount> specialValues_;
    QVarLengthArray<QString, DefaultBarrelCount> specialValueTexts_;
    QVarLengthArray<QColor, DefaultBarrelCount> colors_;

};

inline double ICBarrel::BarrelValue(int index)
{
    return index < BarrelCount() ? barrelValues_.at(index) : 0;
}

inline void ICBarrel::SetBarrelValue(int index, double value)
{
    if(index < BarrelCount())
    {
        if(barrelValues_[index] != value)
        {
            barrelValues_[index] = value;
            update();
        }
    }
}

inline void ICBarrel::SetBarrelText(int index, const QString &text)
{
    if(index < BarrelCount())
    {
        double var = text.toDouble();
        if(barrelValues_[index] != var)
        {
            barrelValues_[index] = var;
            update();
        }
    }
}

inline void ICBarrel::BindSpeicalValueText(int index, double val, const QString &text)
{
    if(index < BarrelCount())
    {
        specialValues_[index] = val;
        specialValueTexts_[index] = text;
    }
}

inline void ICBarrel::SetBarrelBackgroundColor(int index, QColor color)
{
    if(index < BarrelCount())
    {
        if(color != colors_[index])
        {
            colors_[index] = color;
            update();
        }
    }
}
#endif // ICBARREL_H
