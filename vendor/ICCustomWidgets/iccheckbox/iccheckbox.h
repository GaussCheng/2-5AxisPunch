#ifndef ICCHECKBOX_H
#define ICCHECKBOX_H

#include <QCheckBox>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit ICCheckBox(QWidget *parent = 0);
    explicit ICCheckBox(const QString &text, QWidget *parent = 0);

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    QSize sizeHint() const
    {
        QSize ret((this->text().isEmpty()) ? QSize(32, 32) : QSize(100, 32));
        return ret;
    }

signals:
    void FocusOut();
public slots:

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);

};

inline void ICCheckBox::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

#endif // ICCHECKBOX_H
