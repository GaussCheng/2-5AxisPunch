/* If win32 and no cygwin, suppose it's MinGW or any other native windows compiler. */
#if defined(WIN32) && !defined(__CYGWIN__)
#define NATIVE_WIN32
#endif /* win32 and no cygwin */

#ifndef NATIVE_WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstdlib>
#endif

#include <cstdio>
#include <QApplication>
#include <QKeyEvent>
#include <QTime>
#include <QList>
#include <QDebug>
#include "ickeyboardreceiver.h"
#include "mainframe.h"
#include "ickeyboard.h"
#include "icparameterssave.h"
#include "icupdatesystem.h"

#define BEEP_ON 0
#define BEEP_OFF 1

ICKeyboardReceiver::ICKeyboardReceiver(QObject *parent) :
    QThread(parent),
    specialKeyCount_(0)
{
    oldKeyValue_ = -1;
#ifndef NATIVE_WIN32
    beepFD_ = open("/dev/szhc_beep", O_WRONLY);
    if(beepFD_ < 0)
    {
        qDebug("can't not open szhc_beep");
    }
    timer_.setSingleShot(true);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(BeepStop()));
    connect(this, SIGNAL(finished()), SLOT(on_finished()));
    connect(this, SIGNAL(terminated()), SLOT(on_finished()));
#endif
}
ICKeyboardReceiver::~ICKeyboardReceiver()
{
#ifndef NATIVE_WIN32
    ::close(beepFD_);
#endif
}

void ICKeyboardReceiver::run()
{
#ifndef NATIVE_WIN32
    int fd = open("/dev/szhc_keyboard", O_RDONLY);
    //    int fakeKeyboard = open("/dev/fake_keyboard", O_WRONLY);
    char keyValue[5];
    ICParametersSave *config = ICParametersSave::Instance();
    if(fd > 0)
    {
        int key;
        //            time_.start();
        ICKeyboard* keyboard = ICKeyboard::Instace();
        while(1)
        {
            bzero(keyValue, 5);
//            ::system("echo before readkey >> keylog");
            read(fd, keyValue, 5);
//            ::system("echo after readkey >> keylog");
            key = QString(keyValue).toInt();
            qDebug()<<"key"<<key;
            if(config->KeyTone())
            {
                if(key == 0)
                {
                    BeepStop();
                }
                else
                {
                    ioctl(beepFD_, BEEP_ON, NULL);
                    timer_.start(200);
                }
            }
            //            system("echo keypress > /dev/fake_keyboard");
            //            write(fakeKeyboard, "keypress", sizeof("keypress"));
            if(key == ICKeyboard::VFB_X1Add ||
                    key == ICKeyboard::VFB_X1Sub ||
                    key == ICKeyboard::VFB_Y1Add ||
                    key == ICKeyboard::VFB_Y1Sub ||
                    key == ICKeyboard::VFB_ZAdd ||
                    key == ICKeyboard::VFB_ZSub ||
                    key == ICKeyboard::VFB_X2Add ||
                    key == ICKeyboard::VFB_X2Sub ||
                    key == ICKeyboard::VFB_Y2Add ||
                    key == ICKeyboard::VFB_Y2Sub ||
                    key == ICKeyboard::VFB_AAdd ||
                    key == ICKeyboard::VFB_ASub ||
                    key == ICKeyboard::VFB_BSub ||
                    key == ICKeyboard::VFB_BAdd ||
                    key == ICKeyboard::VFB_CSub ||
                    key == ICKeyboard::VFB_CAdd ||
                    key == ICKeyboard::VFB_Pose_Horizontal ||
                    key == ICKeyboard::VFB_Pose_Vertical ||
                    key == ICKeyboard::VFB_SingleStep)
            {
                //                if(time_.restart() > 5)
                //                {
                //                    keyboard->SetKeyValue(key);
                //                    qDebug("run key");
                //                }
                //                continue;
                if(key != oldKeyValue_)
                {
                    keyboard->SetPressed(true);
                    oldKeyValue_ = key;
                    keyboard->SetKeyValue(key);
                    icMainFrame->KeyToInstructEditor(key);
                }
//                msleep(100);
//                ::system("echo after readkey--VFB_X-P >> keylog");
                continue;
            }
//            if(key == ICKeyboard::FB_Up ||
//               key == ICKeyboard::FB_Down)
//            {
//                if(key != oldKeyValue_)
//                {
//                    keyboard->SetPressed(true);
//                    oldKeyValue_ = key;
//                    keyboard->SetKeyValue(key);
//                }
//                msleep(200);
//                continue;
//            }
            if(key != oldKeyValue_)
            {
                oldKeyValue_ = key;
                if(key == ICKeyboard::FB_NULL)
                {
                    keyboard->SetPressed(false);
//                    ::system("echo after readkey--NULL >> keylog");
                    continue;
                }
                if(key >= ICKeyboard::FB_F1 && key <= ICKeyboard::FB_F5)
                {
                    bool isBackLight = icMainFrame->IsBackLightOff();
                    icMainFrame->SetHasInput(true);
                    if(!isBackLight)
                    {
                        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
                        qApp->postEvent(icMainFrame, event);
                    }
//                    msleep(100);
//                    ::system("echo after readkey-F1--F5 >> keylog");
                    continue;
                }
                //                if(key == ICKeyboard::FB_Down || key == ICKeyboard::FB_Up)
                //                {
                //                    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
                //                    qApp->postEvent(icUpdateSystem, event);
                //                }

                keyboard->SetKeyValue(key);
                //                    emit Keypressed(key);
            }
            else
            {
                if(key == ICKeyboard::FB_F5)
                {
                    ++specialKeyCount_;
                    if(specialKeyCount_ == 500)
                    {
                        ::system("cd /home/szhc && echo recal >>recal");
                        specialKeyCount_ = 0;
                    }
                }
            }
//            ::system("echo end readkey >> keylog");
//            msleep(100);
        }
    }
    ::close(fd);
#endif
    //    QList<ICKeyboard::ICVirtualKeyValue> keySqueue;
    //    //    keySqueue.append(ICKeyboard::FB_Stop);
    //    //    keySqueue.append(ICKeyboard::FB_NULL);
    //    keySqueue.append(ICKeyboard::KS_AutoStatu);
    //    keySqueue.append(ICKeyboard::KS_ManualStatu);
    //    keySqueue.append(ICKeyboard::KS_StopStatu);
    //    //    keySqueue.append(ICKeyboard::KS_ManualStatu);
    //    //    keySqueue.append(ICKeyboard::VFB_Run);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_SELECT);
    //    //    keySqueue.append(ICKeyboard::FB_NULL);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_SELECT);
    //    //    keySqueue.append(ICKeyboard::VFB_TRAVEL_IN);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_DOWN);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_FORWARD);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_UP);
    //    //    keySqueue.append(ICKeyboard::VFB_ARM_BACKWARD);
    //    //    keySqueue.append(ICKeyboard::VFB_TRAVEL_OUT);
    //    int i = 0;
    //    ICKeyboard* keyboard = ICKeyboard::Instace();
    //    while(1)
    //    {
    //        sleep(5);
    //        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, keySqueue.at(i), Qt::NoModifier);
    //        qApp->postEvent(icMainFrame, event);
    //        //        emit Keypressed(keySqueue.at(i));
    //        if(keySqueue.at(i) > 0 && keySqueue.at(i) <= 6)
    //        {
    //            keyboard->SetSwitchValue(keySqueue.at(i));
    //            //            qDebug()<<keySqueue.at(i);
    //        }
    //        else
    //        {
    //            keyboard->SetKeyValue(keySqueue.at(i));
    //        }
    //        ++i;
    //        if(i == keySqueue.size())
    //        {
    //            i = 0;
    //        }
    //    }
}

void ICKeyboardReceiver::BeepStop()
{
#ifndef NATIVE_WIN32
    ioctl(beepFD_, BEEP_OFF, NULL);
#endif
}

void ICKeyboardReceiver::on_finished()
{
    ::system("touch key_finished");
}
