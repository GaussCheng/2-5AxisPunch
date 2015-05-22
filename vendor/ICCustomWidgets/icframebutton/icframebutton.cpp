#include <QPainter>

#include "icframebutton.h"

//QScopedPointer<QPixmap> ICFrameButton::background_(NULL);
//QScopedPointer<QPixmap> ICFrameButton::checkedBackgroud_(NULL);
ICFrameButton::ICFrameButton(QWidget *parent) :
    QPushButton(parent)
{
    _level = Level_Basic;
    _pageName = QString();
    _childMenuBar = QString();
    setPageName("test");
    setFocusPolicy(Qt::NoFocus);
    Init_();
}

ICFrameButton::ICFrameButton(const QString &text, QWidget *parent):
    QPushButton(parent)
{
    _pageName = "";
    _childMenuBar = "";
    Init_();
    setText(text);
}

//void ICFrameButton::paintEvent(QPaintEvent *e)
//{
//    QPainter painter;
//    painter.begin(this);
//    if(isChecked())
//    {
//        painter.drawPixmap(this->rect(), *checkedBackgroud_);
//    }
//    else
//    {
//        painter.drawPixmap(this->rect(), *background_);
//    }
//    if(!icon().isNull())
//    {
//        painter.drawPixmap((this->width() - iconSize().width()) >> 1,
//                           (this->height() - iconSize().height()) >> 1,
//                           icon().pixmap(iconSize()));
//    }
//    else
//    {
//        painter.save();
////        if(isChecked())
////        {
////            painter.setPen(Qt::green);
////        }
////        else
////        {
////            painter.setPen(QColor(25, 226, 228));
////        }
//        painter.setFont(font_);
//        painter.drawText(this->rect(), text(), Qt::AlignHCenter | Qt::AlignVCenter);
//        painter.restore();
//    }
//    painter.end();

//    QWidget::paintEvent(e);
//}

void ICFrameButton::Init_()
{
    this->setFocusPolicy(Qt::NoFocus);
//    QFont font = TextFont();
//    TextFont().setPointSize(15);
//    SetTextFont(font);
//    font_.setPointSize(15);
//    SetTextFont(font_);
//    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setMaximumSize(sizeHint());
//    setMinimumSize(sizeHint());
//    if(background_.isNull())
//    {
//        background_.reset(new QPixmap(":/resource/menubutton_normal.png"));
//        checkedBackgroud_.reset(new QPixmap(":/resource/menubutton_checked.png"));
//    }
}
