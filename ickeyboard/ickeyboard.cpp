#include <QThread>
#include <QMutexLocker>
#include <QDebug>

#include "ickeyboard.h"
#include "ickeyboardreceiver.h"
#include "icknobreceiver.h"
#include "icpulleyreceiver.h"
#include "iccommandprocessor.h"

class ICKeyboard::ICKeyboardPrivate
{
public:
    ~ICKeyboardPrivate();
    ICKnobReceiver knobReceiver_;
    ICKeyboardReceiver keyboardReceiver_;
    ICPulleyReceiver pulleyReceiver_;
};

ICKeyboard::ICKeyboardPrivate::~ICKeyboardPrivate()
{
}

QScopedPointer<ICKeyboard> ICKeyboard::instance_;
ICKeyboard::ICKeyboard(QObject *parent) :
    QObject(parent),
    keyValue_(-1),
    switchValue_(-1),
    pulleyValue_(0),
    isTaken_(true),
    isSwitchTaken_(true),
    isPressed_(false),
    currentSwitchValue_(KS_StopStatu),
    currentTuneSpeedType_(-1)
{
    pimpl_.reset(new ICKeyboardPrivate());
    //    connect(&pimpl_->knobReceiver_,
    //            SIGNAL(KnobSwitch(int)),
    //            SIGNAL(Keypressed(int)));
    //    connect(&pimpl_->keyboardReceiver_,
    //            SIGNAL(Keypressed(int)),
    //            SIGNAL(Keypressed(int)));
}

ICKeyboard::~ICKeyboard()
{
}

void ICKeyboard::Receive()
{
    pimpl_->keyboardReceiver_.start();
    pimpl_->knobReceiver_.start();
    pimpl_->pulleyReceiver_.start();
}
void ICKeyboard::Stop()
{
    pimpl_->keyboardReceiver_.quit();
    pimpl_->knobReceiver_.quit();
    pimpl_->pulleyReceiver_.quit();
}

int ICKeyboard::TakeKeyValue()
{
    //    QMutexLocker locker(&keyMutex_);
//    if(!keyMutex_.tryLock())
//    {
//        return -1;
//    }
    if(isTaken_)
    {
        return -1;
    }
    int keyValue = keyValue_;
    keyValue_ = -1;
    isTaken_ = true;
//    keyMutex_.unlock();
    return keyValue;
    //    }
}

void ICKeyboard::SetKeyValue(int value)
{
//    QMutexLocker locker(&keyMutex_);
        if(isTaken_)
        {
    keyValue_ = value;
            isTaken_ = false;
        }
}

int ICKeyboard::TakeSwitchValue()
{
    //    QMutexLocker locker(&switchMutex_);
    if(!switchMutex_.tryLock())
    {
        return -1;
    }
    int switchValue = switchValue_;
    switchValue_ = -1;
    switchMutex_.unlock();
    //    isSwitchTaken_ = true;
    return switchValue;
}

int ICKeyboard::CurrentSwitchStatus() const
{
    QMutexLocker locker(&switchMutex_);
    if(currentSwitchValue_ == ICKeyboard::KS_AutoStatu || currentSwitchValue_ == ICKeyboard::KS_ManualStatu)
    {
        return currentSwitchValue_;
    }
    return ICKeyboard::KS_StopStatu;
}

void ICKeyboard::SetSwitchValue(int value)
{
    QMutexLocker locker(&switchMutex_);
    //    if(isSwitchTaken_)
    //    {
    switchValue_ = value;
    currentSwitchValue_ = value;
    //        isSwitchTaken_ = false;
    //    }
}

int ICKeyboard::TakePulleyValue()
{
    if(!pulleyMutex_.tryLock())
    {
        return 0;
    }
    int pulleyValue = pulleyValue_;
    pulleyValue_ = 0;
    pulleyMutex_.unlock();
    return pulleyValue;

}

bool ICKeyboard::SetPulleyValue(int value)
{
    if(!pulleyMutex_.tryLock())
    {
        return false;
    }
    pulleyValue_ += value;
    pulleyMutex_.unlock();
    return true;
}
