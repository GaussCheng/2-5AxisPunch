#include "icpulleyreceiver.h"

#include <QApplication>
#include <QKeyEvent>
#include <QWheelEvent>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include "ickeyboard.h"

#include "mainframe.h"

#include <QDebug>

ICPulleyReceiver::ICPulleyReceiver(QObject *parent)
    : QThread(parent),
    checkStatus_(false)
{
}

void ICPulleyReceiver::run()
{
    int fd = open("/dev/szhc_pulley", O_RDONLY);
    qDebug() << "is run";
    char pulleyValue[2];
    if(fd < 0)
    {
        qDebug("open szhc_pulley fail!");
        return;
    }
//    int count;
    ICKeyboard* keyboard = ICKeyboard::Instace();
    while(1)
    {
        memset(pulleyValue, 0, 2);
        read(fd, pulleyValue, 2);

        qDebug()<<"pulley"<<(int)pulleyValue[0]<<(int)pulleyValue[1];
        if(pulleyValue[1] == 1)
        {
            qDebug("Set 1");
            keyboard->SetPulleyValue(1);
        }
        else if(pulleyValue[1] == 2)
        {
            qDebug("Set2");
            keyboard->SetPulleyValue(-1);
        }
    }
    ::close(fd);
}
