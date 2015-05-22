#ifndef ICBLINKINGLABEL_H
#define ICBLINKINGLABEL_H

#include "iclabel.h"
#include <QTimer>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICBlinkingLabel : public ICLabel
{
    Q_OBJECT
public:
    explicit ICBlinkingLabel(QWidget *parent = 0);

    void BlinkingColor(QColor color, int times);
    void BlinkingBackground(QColor color, int times);
    
signals:
    
public slots:

private slots:
    void BlinkColor();
    void BlinkBackGround();

private:
    int textBlinkingTimes_;
    int backgroundBlinkingTimes_;
    QColor blinkColor_;
    QColor blinkBackground_;
    QTimer textBlinkingTimer_;
    QTimer backgroundBlinkingTimer_;
    QString oldStyle_;

    
};

#endif // ICBLINKINGLABEL_H
