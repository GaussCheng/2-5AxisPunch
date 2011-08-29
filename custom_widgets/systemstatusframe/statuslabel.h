#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include <QLabel>
#include <QPixmap>

class StatusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StatusLabel(QWidget *parent = 0);

    enum DisplayStatus{ONSTATUS, OFFSTATUS, CLEAR};

    void SetOnStatusAttribute(const QString & statusPicture);
    void SetOffStatusAttibute(const QString & statusPicture);

    void SetStatus(const DisplayStatus & displayStatus);
    DisplayStatus currentStatus() const { return currentStatus_;}

signals:

public slots:

private:
    QPixmap onStatusPixmap_;
    QPixmap offStatusPixmap_;
    DisplayStatus currentStatus_;
};

inline void StatusLabel::SetOnStatusAttribute(const QString & statusPicture)
{
    QPixmap onStatusPixmap(statusPicture);
    onStatusPixmap_ = onStatusPixmap;
}

inline void StatusLabel::SetOffStatusAttibute(const QString & statusPicture)
{
    QPixmap offStatusPixmap(statusPicture);
    offStatusPixmap_ = offStatusPixmap;
}
#endif // STATUSLABEL_H
