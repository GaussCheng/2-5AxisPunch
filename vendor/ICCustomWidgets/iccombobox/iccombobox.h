#ifndef ICCOMBOBOX_H
#define ICCOMBOBOX_H

#include <QComboBox>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ICComboBox(QWidget *parent = 0);
    QSize sizeHint() const { return QSize(95, 32);}
    QSize minimumSizeHint() const { return sizeHint();}

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    int LastValue() const { return oldValue_;}
    void SetCurrentIndex(int index);
    void SetComboBoxSize(QSize &size);

signals:

public slots:
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    int oldValue_;
};

inline void ICComboBox::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

inline void ICComboBox::SetCurrentIndex(int index)  //这个函数的意义是什么???
{
    if(index != this->currentIndex())
    {
        this->setCurrentIndex(index);
    }
}

#endif // ICCOMBOBOX_H
