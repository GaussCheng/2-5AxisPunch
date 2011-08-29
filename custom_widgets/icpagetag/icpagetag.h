#ifndef ICPAGETAG_H
#define ICPAGETAG_H

#include <QAbstractButton>
#include <QPixmap>

class ICPageTag : public QAbstractButton
{
    Q_OBJECT

public:
    enum IconSide{IconOnLeft, IconOnRight};
    explicit ICPageTag(QWidget *parent = 0);
    ~ICPageTag();

    IconSide Direction() const {return iconSide_;}
    void SetDirection(IconSide iconSide) { iconSide_ = iconSide;}

    QSize sizeHint() const { return QSize(430, 55);}
    QSize minimumSizeHint() const { return sizeHint();}

protected:
    void paintEvent(QPaintEvent *e);

private:
    IconSide iconSide_;
    QPixmap leftBackground_;
    QPixmap rightBackground_;
    QPixmap leftPressBG_;
    QPixmap rightPressBG_;
};

#endif // ICPAGETAG_H
