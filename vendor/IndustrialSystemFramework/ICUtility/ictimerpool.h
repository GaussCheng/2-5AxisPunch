///////////////////////////////////////////////////////////
//  ICTimerPool.h
//  Implementation of the Class ICTimerPool
//  Created on:      05-九月-2011 10:05:53
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#if !defined(EA_FCCA4848_BA70_457b_A961_BE421B64C3EA__INCLUDED_)
#define EA_FCCA4848_BA70_457b_A961_BE421B64C3EA__INCLUDED_

/**
 * 定时器池，用于帮助管理框架中的定时器
 * @author GaussCheng
 * @version 1.0
 * @updated 05-九月-2011 10:21:19
 */

#include "ICUtility_global.h"

#include <QVarLengthArray>
#include <QTimer>
#include <QScopedPointer>
#include <QBitArray>
#include <QDebug>

/**
 * 定时器池，用于帮助管理框架中的定时器
 * @author GaussCheng
 * @version 1.0
 * @updated 06-九月-2011 10:34:34
 */
class ICUTILITYSHARED_EXPORT ICTimerPool
{

public:
	virtual ~ICTimerPool();
    static ICTimerPool* Instance();
    int Start(uint timeout, const QObject * receiver, const char* handler);
    void Stop(int timerID, const QObject * receiver, const char* handler);

    const static int RefreshTime = 20;

private:
    QTimer* GetTimer_(int &id);
    static QScopedPointer<ICTimerPool> instance_;
	/**
	 * 定时器池
	 */
    const static uint DefaultSize = 32;
	/**
	 * 定时器池
	 */
    QVarLengthArray<QTimer*, DefaultSize> timerPool_;
    QBitArray flags_;
    ICTimerPool();

};

/**
 * 获取定时器池的实例
 */
inline ICTimerPool* ICTimerPool::Instance()
{
    return instance_.data();
}

/**
 * 分配并启动一个定时器，启动一个定时器
 * 
 * @param timeout    定时器时间
 * @param receiver
 * @param handler    定时器时间到后的处理函数
 */
inline int ICTimerPool::Start(uint timeout, const QObject * receiver, const char* handler)
{
    int retID = -1;
    if(receiver != NULL && handler != NULL)
    {
        QTimer* timer = GetTimer_(retID);
        timer->connect(timer, SIGNAL(timeout()), receiver, handler);
        timer->start(timeout);
//        return retID;
    }
    return retID;
}

/**
 * 停止ID为timerID的定时器
 * 
 * @param timerID    定时器ID
 */
inline void ICTimerPool::Stop(int timerID, const QObject * receiver, const char* handler)
{
    Q_ASSERT_X(timerID >= 0 && timerID < timerPool_.size(), "ICTimerPool::Stop", "timerID is not vail!");
    Q_ASSERT_X(timerPool_[timerID] != NULL, "ICTimerPool::Stop", "The timerID is not sign to a vail timer!");
    if(timerID >= 0 && timerID < timerPool_.size())
    {
        if(timerPool_[timerID] != NULL)
        {
            timerPool_[timerID]->stop();
            timerPool_[timerID]->disconnect(receiver, handler);
            flags_.clearBit(timerID);
        }
    }
}
#endif // !defined(EA_FCCA4848_BA70_457b_A961_BE421B64C3EA__INCLUDED_)
