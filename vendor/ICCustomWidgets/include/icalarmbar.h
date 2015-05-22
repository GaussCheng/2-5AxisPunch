#ifndef ICALARMBAR_H
#define ICALARMBAR_H

#include <QLabel>
#include <QStringList>
#include <QTimer>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICAlarmBar : public QLabel
{
    Q_OBJECT
public:
    explicit ICAlarmBar(QWidget *parent = 0);
    virtual ~ICAlarmBar(){timer_.stop();}
    void SetAlarmStrings(const QStringList& alarms);

    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);

signals:

public slots:

private slots:
    void ShowNextAlarm();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    QStringList alarmStrings_;
    QTimer timer_;
    int currentAlarmIndex_;
    QFont font_;

};

inline void ICAlarmBar::SetAlarmStrings(const QStringList &alarms)
{
    currentAlarmIndex_ = 0;
    alarmStrings_ = alarms;
}

inline void ICAlarmBar::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
}

#endif // ICALARMBAR_H
