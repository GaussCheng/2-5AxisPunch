#ifndef NATIVE_WIN32
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#endif

#include <cstdio>
#include <QDebug>
#include "icknobreceiver.h"
#include "ickeyboard.h"

ICKnobReceiver::ICKnobReceiver(QObject *parent) :
        QThread(parent)
{
}

void ICKnobReceiver::run()
{
#ifndef NATIVE_WIN32
    int fd = open("/dev/szhc_knob", O_RDONLY);
//    int fakeKeyboard = open("/dev/fake_keyboard", O_WRONLY);
    char keyValue[5];
    if(fd > 0)
    {
        ICKeyboard *keyboard = ICKeyboard::Instace();
        while(1)
        {
            bzero(keyValue, 5);
            read(fd, keyValue, 5);
//            system("echo keypress > /dev/fake_keyboard");
//            write(fakeKeyboard, "keypress", sizeof("keypress"));
            keyboard->SetSwitchValue(QString(keyValue).toInt());
//            emit KnobSwitch(QString(keyValue).toInt());
        }
    }
#endif
}
