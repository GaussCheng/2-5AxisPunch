#ifndef ICPULLEYRECEIVER_H
#define ICPULLEYRECEIVER_H

#include <QThread>

class ICPulleyReceiver : public QThread
{
    Q_OBJECT
public:
    explicit ICPulleyReceiver(QObject *parent = 0);

protected:
    virtual void run();

private:
    bool checkStatus_;
};

#endif // ICPULLEYRECEIVER_H
