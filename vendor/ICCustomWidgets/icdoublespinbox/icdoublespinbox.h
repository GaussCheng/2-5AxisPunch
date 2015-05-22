#ifndef ICDOUBLESPINBOX_H
#define ICDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QList>
#include <qmath.h>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit ICDoubleSpinBox(QWidget *parent = 0);

    QSize sizeHint() const { return QSize(82, 37);}
    QSize minimumSizeHint() const { return sizeHint();}

    double FormatInt(int value){ return value / qPow(10, decimals());}

public slots:
    void setMinimum(double min);
    void setMaximum(double max);

    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);

    double LastValue() const { return oldValue_;}

    void OnLineEditTextChanged(const QString& text);
    void OnFinished();

signals:

protected:
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void UpdateStatus();
private:
    QFont font_;
    double oldValue_;
    static QList<int> validKeys_;

};

inline void ICDoubleSpinBox::setMinimum(double min)
{
    QDoubleSpinBox::setMinimum(min);
    UpdateStatus();
}

inline void ICDoubleSpinBox::setMaximum(double max)
{
    QDoubleSpinBox::setMaximum(max);
    UpdateStatus();
}

inline void ICDoubleSpinBox::UpdateStatus()
{
    setStatusTip(tr("Max %1 Min %2").arg(maximum()).arg(minimum()));
}

inline void ICDoubleSpinBox::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
}

#endif // ICDOUBLESPINBOX_H
