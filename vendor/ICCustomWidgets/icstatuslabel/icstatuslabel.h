#ifndef ICSTATUSLABEL_H
#define ICSTATUSLABEL_H

#include <QFrame>
#include <QStaticText>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICStatusLabel : public QFrame
{
    Q_OBJECT
public:
    explicit ICStatusLabel(QWidget *parent = 0);

    QString Text() const;
    void SetText(const QString& text);
    QString UnitText() const { return unitText_.text();}
    void SetUnitText(const QString& unitText) { unitText_.setText(unitText);}
    QPixmap Icon() const { return icon_;}
    void SetIcon(const QPixmap &pixmap);//
    QSize IconSize() const { return iconSize_;}
    void SetIconSize(QSize size) {iconSize_ = size;}
    QFont UnitFont() const { return unitFont_;}
    void SetUnitFont(const QFont& font);//
    QFont ValueFont() const { return valueFont_;}
    void SetValueFont(const QFont& font);//

    QSize sizeHint() const { return QSize(80, 32);}
    QSize minimumSizeHint() const { return sizeHint();}

    QString SpecialText() const { return specialText_;}
    void SetSpecialText(const QString& text) { specialText_ = text;}
    bool EnSpecialText() const { return enSpecialText_;}
    void SetEnSpecialText(bool isEnable) { enSpecialText_ = isEnable; update();}

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *e);
private:
    QString text_;
    QStaticText unitText_;
    QPixmap icon_;
    QSize iconSize_;
    QFont unitFont_;
    QFont valueFont_;
    QString specialText_;
    bool enSpecialText_;

};

inline void ICStatusLabel::SetUnitFont(const QFont &font)
{
    unitFont_ = font;
    unitFont_.setPixelSize((unitFont_.pointSize() << 2) / 3);
    unitText_.prepare(QTransform(), unitFont_);
}

inline void ICStatusLabel::SetValueFont(const QFont &font)
{
    valueFont_ = font;
    valueFont_.setPixelSize((valueFont_.pointSize() << 2) / 3);
}

inline void ICStatusLabel::SetIcon(const QPixmap& pixmap)
{
    icon_ = pixmap;
    iconSize_ = pixmap.size();
}

inline QString ICStatusLabel::Text() const
{
    if(!EnSpecialText())
    {
        return text_;
    }
    return specialText_;
}

inline void ICStatusLabel::SetText(const QString &text)
{
    if(EnSpecialText())
    {
        return;
    }
    if(text != Text())
    {
        text_ = text;
        update();
    }
}

#endif // ICSTATUSLABEL_H
