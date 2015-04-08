#ifndef ICINPUTMETHODEDIT_H
#define ICINPUTMETHODEDIT_H

#include <QLineEdit>
#include "icinputmethodkeyboard.h"

class ICInputMethodEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ICInputMethodEdit(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e);
signals:

public slots:

private:
    ICInputMethodKeyboard* keyboard_ ;

};

#endif // ICINPUTMETHODEDIT_H
