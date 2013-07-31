#ifndef SPEEDSTATUSLABEL_H
#define SPEEDSTATUSLABEL_H

#include <QLabel>

#include <QString>
class QTimer;

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
    void mousePressEvent(QMouseEvent *ev);


private slots:
    void OnTimerOut();
private:
    void Start(int count);
    void Stop();
    QTimer* timer_;
    QString oldSytle_;
    int count_;
    int endCount_;
};

inline QString SpeedStatusLabel::CurrentSpeed() const
{
    return this->text();
}

#endif // SPEEDSTATUSLABEL_H
