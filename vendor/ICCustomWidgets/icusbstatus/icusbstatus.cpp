#include "icusbstatus.h"

#include <QFileSystemWatcher>
#include <QFile>
#include <QDebug>

ICUSBStatus::ICUSBStatus(QWidget *parent) :
    QLabel(parent),
    currentStatus_(false)
{
    setPixmap(disconnectedPic_);//
    //    watcher_ = new QFileSystemWatcher();
    //    connect(watcher_,
    //            SIGNAL(fileChanged(QString)),
    //            this,
    //            SLOT(OnUsbStatusChanged(QString)));
}

ICUSBStatus::~ICUSBStatus()
{
//    delete watcher_;
}

void ICUSBStatus::OnUsbStatusChanged(const QString &file)
{
    Q_UNUSED(file)
//    qDebug("usb cheanged");
//    QFile usbfile(file);
//    usbfile.open(QFile::ReadOnly);
//    if(usbfile.readAll().isEmpty())
//    {
//        setPixmap(disconnectedPic_);
//    }
//    else
//    {
//        setPixmap(connectedPic_);
//    }
}

void ICUSBStatus::SetMonitorPath(const QString &path)
{
    Q_UNUSED(path)
//    if(!watcher_->files().isEmpty())
//    {
//        watcher_->removePaths(watcher_->files());
//    }
//    watcher_->addPath(path);
}

void ICUSBStatus::SetUsbStatus(bool attached)
{
    if(attached != currentStatus_)
    {
        currentStatus_ = attached;
        if(attached)
        {
            setPixmap(connectedPic_);
        }
        else
        {
            setPixmap(disconnectedPic_);
        }
    }
}
