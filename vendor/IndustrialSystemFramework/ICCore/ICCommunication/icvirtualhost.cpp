///////////////////////////////////////////////////////////
//  icvirtualhost.cpp
//  Implementation of the Class ICVirtualHost
//  Created on:      27-七月-2011 16:38:16
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#include <QRunnable>
#include <QThreadPool>
#include <QtCore/qmath.h>

//#include "icutility.h"
#include "icvirtualhost.h"


QMap<uint64_t, ICVirtualHostPtr> ICVirtualHostManager::hostAddrs_;
ICVirtualHost::ICVirtualHost(uint64_t hostId, QObject *parent)
    :QObject(parent),
      hostId_(hostId),
      communicateInterval_(10),
      transceiver_(NULL),
      currentAlarmBitIndex_(0),
//      timerID_(-1),
      commErrCount_(0)
{
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(RefreshStatus()));
}

ICVirtualHost::~ICVirtualHost()
{
//    if(timerID_ >= 0)
//    {
////        ICTimerPool::Instance()->Stop(timerID_, this, SLOT(RefreshStatus()));
//        killTimer(timerID_);
//    }
    timer_.stop();
}

void ICVirtualHost::RefreshStatus()
{
    Communicate();
}
