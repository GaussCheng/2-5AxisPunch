#ifndef ICFRAMEBUTTON_H
#define ICFRAMEBUTTON_H

#include "icbutton.h"
#include "ICCustomWidgets_global.h"
#include <QDebug>


class ICCUSTOMWIDGETSSHARED_EXPORT ICFrameButton : public QPushButton
{
    Q_OBJECT
    Q_ENUMS(ICConfigLevel)
    Q_PROPERTY(ICConfigLevel level READ level WRITE setLevel)
    Q_PROPERTY(QString pageName READ pageName WRITE setPageName)
    Q_PROPERTY(QString childMenuBar READ childMenuBar WRITE setChildMenuBar)

public:
    enum ICConfigLevel{
        Level_Basic,
        Level_Function,
        Level_Machine,
        Level_System
    };
    ICConfigLevel level() { return _level;}
    void setLevel(ICConfigLevel level){_level = level;}

    QString pageName() { return _pageName;}
    void setPageName(QString pageName) {   _pageName  = pageName;}

    QString childMenuBar(){ return _childMenuBar;}
    void setChildMenuBar(QString  childMenuBar) { _childMenuBar = childMenuBar;}
private:
    QString _pageName;
    QString _childMenuBar;
    ICConfigLevel _level;
public:
    explicit ICFrameButton(QWidget *parent = 0);
    ICFrameButton(const QString& text, QWidget *parent = 0);
signals:


private:
    void Init_();
};

//inline void ICFrameButton::SetTextFont(const QFont &font)
//{
//    font_ = font;
//    font_.setPixelSize((font_.pointSize() << 2) / 3);
//}

#endif // ICFRAMEBUTTON_H
