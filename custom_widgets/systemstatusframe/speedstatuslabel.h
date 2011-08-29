#ifndef SPEEDSTATUSLABEL_H
#define SPEEDSTATUSLABEL_H

#include <QLabel>

#include <QString>

class SpeedStatusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SpeedStatusLabel(QWidget *parent = 0);

signals:

public:
    QString CurrentSpeed() const;

public slots:
    void SetCurrentSpeed(const QString & speed);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
};

inline void SpeedStatusLabel::SetCurrentSpeed(const QString & speed)
{
    this->setText(speed);
}

inline QString SpeedStatusLabel::CurrentSpeed() const
{
    return this->text();
}

#endif // SPEEDSTATUSLABEL_H
