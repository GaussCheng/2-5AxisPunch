#ifndef ICKEYBOARD_H
#define ICKEYBOARD_H

#if defined(WIN32) && !defined(__CYGWIN__)
#define NATIVE_WIN32
#endif /* win32 and no cygwin */

#include <QObject>
#include <QScopedPointer>
#include <QAbstractButton>
#include <QMutex>

#define icKeyboard ICKeyboard::Instace()
class ICKeyboard : public QObject
{
    Q_OBJECT
public:
    enum ICVirtualKeyValue
    {
        FB_NULL = 0,
        KS_ManualStatu = 1,
        KS_StopStatu = 4,
        KS_AutoStatu = 6,
        VFB_Run,
        FB_Stop,//8
        FB_Origin,
        FB_Reset,
        FB_Up,//11
        FB_Down,
        VFB_X1Add   = 13,
        VFB_X1Sub   = 14,
        VFB_Y1Sub   = 16,
        VFB_Y1Add   = 24,
        VFB_ZSub    = 25,
        VFB_ZAdd    = 26,
        VFB_Pose_Horizontal  = 17,
        VFB_Pose_Vertical   = 27,
        VFB_SP1 = 19,
        VFB_SP2 = 23,
        VFB_X2Sub   = 37,
        VFB_X2Add   = 28,
        VFB_Y2Sub   = 15,
        VFB_Y2Add   = 21,
        FB_F1       = 29, //29
        FB_F2       = 30,
        FB_F3       = 31,
        FB_F4       = 32,
        FB_F6       = 33,
        FB_F5       = 34,
//        VFB_ARM_UP,//13
//        VFB_ARM_DOWN,
//        VFB_ARM_BACKWARD,
//        VFB_ARM_FORWARD,//16
//        VFB_ARM_GRIP,
//        VFB_ARM_RELEASE,
//        VFB_OP1,//19
//        VFB_OP2,
//        VFB_On,
//        VFB_Off,
//        VFB_TRAVEL_IN,//23
//        VFB_TRAVEL_OUT,//24
//        VFB_ARM_SELECT,
//        VFB_SUCTION_HOLD,
//        FB_F1, //29
//        FB_F2,
//        FB_F3,
//        FB_F4,
//        FB_F5,
//        FB_F6,
//        FB_NULL = 0xC0,
//        VFB_Run,
//        FB_Stop,//8
//        FB_Up,//11
//        FB_Down,
//        FB_F1, //29
//        FB_F2,
//        FB_F3,
//        FB_F4,
//        FB_F5,
//        FB_F6,
//        VFB_X1Sub,
//        VFB_X1Add,
//        VFB_Y1Sub,
//        VFB_Y1Add,
//        VFB_ZSub,
//        VFB_ZAdd,
//        VFB_PoseHorizontal,
//        VFB_PoseVertical,
//        VFB_SP1,
//        VFB_SP2,
//        VFB_X2Sub,
//        VFB_X2Add,
//        VFB_Y2Sub,
//        VFB_Y2Add,
//        FB_Origin,
//        FB_Reset,
    };

//    enum ICKeyValue
//    {
//        Real_NULL,
//        Real_Run,
//        Real_Stop,//8
//        Real_Up,//11
//        Real_Down,
//        Real_F1, //29
//        Real_F2,
//        Real_F3,
//        Real_F4,
//        Real_F5,
//        Real_F6,
//        Real_X1Sub,
//        Real_X1Add,
//        Real_Y1Sub,
//        Real_Y1Add,
//        Real_ZSub,
//        Real_ZAdd,
//        Real_PoseHorizontal,
//        Real_PoseVertical,
//        Real_SP1,
//        Real_SP2,
//        Real_X2Sub,
//        Real_X2Add,
//        Real_Y2Sub,
//        Real_Y2Add,
//        Real_Origin,
//        Real_Reset,
//    };

    ~ICKeyboard();
    static ICKeyboard* Instace()
    {
        if(instance_.isNull())
        {
            instance_.reset(new ICKeyboard());
        }
        return instance_.data();
    }

    void Receive();
    int TakeKeyValue();
    void SetKeyValue(int value);

    int TakeSwitchValue();
    void SetSwitchValue(int value);

    int TakePulleyValue();
    bool SetPulleyValue(int value);

    bool IsPressed() const { return isPressed_;}
    void SetPressed(bool isPressed) { isPressed_ = isPressed;}

    int CurrentSwitchStatus() const { return currentSwitchValue_;}
Q_SIGNALS:
//    void Keypressed(int);
//    void KnobSwitched(int);
private:
    static QScopedPointer<ICKeyboard> instance_;
    class ICKeyboardPrivate;
    QScopedPointer<ICKeyboardPrivate> pimpl_;
    explicit ICKeyboard(QObject *parent = 0);
    int keyValue_;
    int switchValue_;
    int pulleyValue_;
    bool isTaken_;
    bool isSwitchTaken_;
    bool isPressed_;
    //    bool isPulleyTaken_;
    mutable QMutex pulleyMutex_;
    int currentSwitchValue_;
//    mutable QMutex keyMutex_;
//    mutable QMutex switchMutex_;

};

inline void ClickButtonHelper(QAbstractButton *button)
{
    button->setChecked(true);
    button->setFocus();
}

#endif // ICKEYBOARD_H
