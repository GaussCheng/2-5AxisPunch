#ifndef ICCLOCKLABEL_H
#define ICCLOCKLABEL_H

#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICClockLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ICClockLabel(QWidget *parent = 0);
    bool timer_status() const  {return timer_.isActive();}

signals:
    void ClockLableClicked();
public slots:

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void mouseReleaseEvent(QMouseEvent *ev);

private slots:
    void ShowTime();

private:
    QTimer timer_;
    QDateTime dataTime_;
};

#endif // ICCLOCKLABEL_H
