#ifndef ICCHECKEDBUTTON_H
#define ICCHECKEDBUTTON_H

#include <QPushButton>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICCheckedButton : public QPushButton
{
    Q_OBJECT

    /*
     *属性
    */
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString checkedName READ checkedName WRITE setCheckedName)

public:
    explicit ICCheckedButton(QWidget *parent = 0);

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    QString name(){ return _name;}
    void setName(QString name){ _name = name;setText(name);}

    QString checkedName(){return _checkedName;}
    void setCheckedName(QString checkedName){ _checkedName = checkedName;}

signals:

public slots:

private slots:
    void OnButtonToggled(bool);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    QString  _name;
    QString _checkedName;

};

inline void ICCheckedButton::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

#endif // ICBUTTON_H
