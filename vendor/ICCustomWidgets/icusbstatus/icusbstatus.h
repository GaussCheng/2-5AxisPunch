#ifndef ICUSBSTATUS_H
#define ICUSBSTATUS_H

#include <QLabel>
#include <QPixmap>
#include "ICCustomWidgets_global.h"

class QFileSystemWatcher;

class ICCUSTOMWIDGETSSHARED_EXPORT ICUSBStatus : public QLabel
{
    Q_OBJECT
public:
    explicit ICUSBStatus(QWidget *parent = 0);
    ~ICUSBStatus();

    void SetUsbStatus(bool attached);
    void SetMonitorPath(const QString& path);
    void BindOnPic(const QPixmap& pic) {connectedPic_ = pic;}
    void BindOffPic(const QPixmap& pic) {disconnectedPic_ = pic;setPixmap(disconnectedPic_);}

signals:

public slots:
private slots:
    void OnUsbStatusChanged(const QString& dir);

private:
    QPixmap connectedPic_;
    QPixmap disconnectedPic_;
//    QFileSystemWatcher* watcher_;
    bool currentStatus_;
};

#endif // ICUSBSTATUS_H
