///////////////////////////////////////////////////////////
//  ICTimerPool.h
//  Implementation of the Class ICTimerPool
//  Created on:      05-����-2011 10:05:53
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#if !defined(EA_FCCA4848_BA70_457b_A961_BE421B64C3EA__INCLUDED_)
#define EA_FCCA4848_BA70_457b_A961_BE421B64C3EA__INCLUDED_

/**
 * ��ʱ���أ����ڰ����������еĶ�ʱ��
 * @author GaussCheng
 * @version 1.0
 * @updated 05-����-2011 10:21:19
 */

#include "ICUtility_global.h"

#include <QVarLengthArray>
#include <QTimer>
#include <QScopedPointer>
#include <QBitArray>
#include <QDebug>

/**
 * ��ʱ���أ����ڰ����������еĶ�ʱ��
 * @author GaussCheng
 * @version 1.0
 * @updated 06-����-2011 10:34:34
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
	 * ��ʱ����
	 */
    const static uint DefaultSize = 32;
	/**
	 * ��ʱ����
	 */
    QVarLengthArray<QTimer*, DefaultSize> timerPool_;
    QBitArray flags_;
    ICTimerPool();

};

/**
 * ��ȡ��ʱ���ص�ʵ��
 */
inline ICTimerPool* ICTimerPool::Instance()
{
    return instance_.data();
}

/**
 * ���䲢����һ����ʱ��������һ����ʱ��
 * 
 * @param timeout    ��ʱ��ʱ��
 * @param receiver
 * @param handler    ��ʱ��ʱ�䵽��Ĵ�����
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
 * ֹͣIDΪtimerID�Ķ�ʱ��
 * 
 * @param timerID    ��ʱ��ID
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
