#include "iccheckedbutton.h"
#include <QKeyEvent>

ICCheckedButton::ICCheckedButton(QWidget *parent) :
    QPushButton(parent),
    _name(""),
    _checkedName("")
{
    QFont f = font();
    f.setPointSize(12);
    SetTextFont(f);
    setCheckable(true);
    connect(this,SIGNAL(toggled(bool)),
            this,SLOT(OnButtonToggled(bool)));
    setFocusPolicy(Qt::NoFocus);
}

void ICCheckedButton::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {
        this->click();
        e->accept();
    }
    else
    {
        QPushButton::keyPressEvent(e);
    }
}

void ICCheckedButton::OnButtonToggled(bool toggled)
{
    if(toggled){
        setText(_checkedName);
    }
    else{
        setText(_name);
    }

}
