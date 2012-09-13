#ifndef ICKEYBOARDRECEIVER_H
#define ICKEYBOARDRECEIVER_H

#include <QThread>
#include <QTimer>
#include <QTime>

class ICKeyboardReceiver : public QThread
{
Q_OBJECT
public:
    explicit ICKeyboardReceiver(QObject *parent = 0);
    ~ICKeyboardReceiver();

Q_SIGNALS:
    void Keypressed(int);

public slots:
    void on_finished();
protected:
    virtual void run();

private Q_SLOTS:
    void BeepStop();
private:
    int oldKeyValue_;
    int beepFD_;
    QTimer timer_;
    int specialKeyCount_;
    QTime time_;

};

#endif // ICKEYBOARDRECEIVER_H
