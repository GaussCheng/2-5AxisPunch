#ifndef ICKNOBRECEIVER_H
#define ICKNOBRECEIVER_H

#include <QThread>

class ICKnobReceiver : public QThread
{
    Q_OBJECT
public:

    explicit ICKnobReceiver(QObject *parent = 0);

Q_SIGNALS:
    void KnobSwitch(int);

protected:
    virtual void run();

public slots:

};

#endif // ICKNOBRECEIVER_H
