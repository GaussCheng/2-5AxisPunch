#ifndef ICGROUPBOX_H
#define ICGROUPBOX_H

#include <QGroupBox>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ICGroupBox(QWidget *parent = 0);
    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

signals:

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
};

inline void ICGroupBox::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

#endif // ICGROUPBOX_H
