#ifndef ICKEYBOARDBUTTON_H
#define ICKEYBOARDBUTTON_H
#include <QPushButton>
#include <QKeyEvent>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICKeyboardButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ICKeyboardButton(QWidget *parent = 0);
    explicit ICKeyboardButton(const QString &text, QWidget *parent=0);
    ICKeyboardButton(const QIcon& icon, const QString &text, QWidget *parent=0);

    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);
protected:
    void keyPressEvent(QKeyEvent *e);

private:
    QFont font_;
};

inline void ICKeyboardButton::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
}


#endif // ICKEYBOARDBUTTON_H
