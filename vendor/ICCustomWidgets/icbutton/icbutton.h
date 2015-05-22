#ifndef ICBUTTON_H
#define ICBUTTON_H

#include <QPushButton>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICButton : public QPushButton
{
    Q_OBJECT

    /*
     *属性
    */
    Q_PROPERTY(QString instruction READ Instruction WRITE setInstruction)
    Q_PROPERTY(QString parameter READ Parameter WRITE setParameter)

public:
    explicit ICButton(QWidget *parent = 0);

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    QString Instruction(){ return _instruction;}
    void setInstruction(QString instruction){ _instruction = instruction;}

    QString Parameter(){return _paramter;}
    void setParameter(QString parameter){ _paramter = parameter;}

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *e);
private:
    QString  _instruction;
    QString _paramter;

};

inline void ICButton::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

#endif // ICBUTTON_H
