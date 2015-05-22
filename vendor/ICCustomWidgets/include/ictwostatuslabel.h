#ifndef ICTWOSTATUSLABEL_H
#define ICTWOSTATUSLABEL_H

#include <QLabel>
#include "ICCustomWidgets_global.h"

class QTimer;

class ICCUSTOMWIDGETSSHARED_EXPORT ICTwoStatusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ICTwoStatusLabel(QWidget *parent = 0);
    virtual ~ICTwoStatusLabel();

    QPixmap OnStatus() const { return onStatus_;}
    void SetOnStatus(const QPixmap& pic) { onStatus_ = pic;}
    QPixmap OffStatus() const { return offStatus_;}
    void SetOffStatus(const QPixmap& pic) { offStatus_ = pic;}
    void ShowOnStatus() { this->setPixmap(OnStatus());}
    void ShowOffStatus() { this->setPixmap(OffStatus());}
    void Start(int count);
    void Stop();

signals:

public slots:

private slots:
    void OnTimerOut();

private:
    QPixmap onStatus_;
    QPixmap offStatus_;
    QTimer* timer_;
    int count_;
    int endCount_;
};

#endif // ICTWOSTATUSLABEL_H
