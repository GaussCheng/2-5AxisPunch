///////////////////////////////////////////////////////////
//  icmainframe.cpp
//  Implementation of the Class ICMainFrame
//  Created on:      21-七月-2011 19:00:53
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#include <QWSScreenSaver>
#include "icmainframe.h"

#ifdef Q_WS_QWS
class ICDefaultScreenSaver:public QWSScreenSaver
{
public:
    ~ICDefaultScreenSaver()
    {
        if(screenFunctionObject_ != NULL)
        {
            delete screenFunctionObject_;
        }
    }

    void restore()
    {
        qDebug("SimpleScreenSaver::restore");
        QWSServer::instance()->refresh();
//        ICPeripherals::ICBacklightOn();
        if(screenFunctionObject_ != NULL)
        {
            screenFunctionObject_->ScreenRestore();
        }
    }

    bool save(int level)
    {
        Q_UNUSED(level)
        qDebug("SimpleScreenSaver::save");
//        ICPeripherals::ICBacklightOff();
//        ICPeripherals::ICShowScreenSaver();
        if(screenFunctionObject_ != NULL)
        {
            screenFunctionObject_->ScreenSave();
        }
        return true;
    }

    void SetScreenFunction(ScreenFunctionObject* o) {screenFunctionObject_ = o;}

private:
    ScreenFunctionObject* screenFunctionObject_;
};
#endif

ICMainFrame::ICMainFrame(QWidget *parent)
    :QWidget(parent)
{
#ifdef Q_WS_QWS
    screenSaver_ = new ICDefaultScreenSaver();
    ScreenFunctionObject* fo = new ScreenFunctionObject();
    screenSaver_->SetScreenFunction(fo);
    connect(fo,
            SIGNAL(ScreenSaved()),
            SIGNAL(ScreenSave()));
    connect(fo,
            SIGNAL(ScreenRestored()),
            SIGNAL(ScreenRestore()));
    QWSServer::setScreenSaver(screenSaver_);
    QWSServer::setScreenSaverBlockLevel(0);
#endif
}


ICMainFrame::~ICMainFrame()
{
#ifdef Q_WS_QWS
    delete screenSaver_;
#endif
}


/**
 * 设定屏保触发时间，时间为ms
 * 
 * @param intervals    屏保触发时间，单位为ms
 */
void ICMainFrame::SetScreenSaverInterval(int intervals)
{
#ifdef Q_WS_QWS
    QWSServer::setScreenSaverInterval(intervals);
#else
    Q_UNUSED(intervals)
#endif
}
