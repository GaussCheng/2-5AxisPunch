#ifndef ICSPINBOX_H
#define ICSPINBOX_H

#include <QSpinBox>
#include <QList>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit ICSpinBox(QWidget *parent = 0);
    QSize sizeHint() const { return QSize(82, 37);}
    QSize minimumSizeHint() const { return sizeHint();}
    void setMinimum(int min);
    void setMaximum(int max);

    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);

    int LastValue() const { return oldValue_;}

signals:

public slots:

protected:
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    QFont font_;
    int oldValue_;
    void UpdateStatus();
    static QList<int> validKeys_;
};

inline void ICSpinBox::setMinimum(int min)
{
    QSpinBox::setMinimum(min);
    UpdateStatus();
}

inline void ICSpinBox::setMaximum(int max)
{
    QSpinBox::setMaximum(max);
    UpdateStatus();
}

inline void ICSpinBox::UpdateStatus()
{
    setStatusTip(tr("Max %1 Min %2").arg(maximum()).arg(minimum()));
}

inline void ICSpinBox::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
}

#endif // ICSPINBOX_H
