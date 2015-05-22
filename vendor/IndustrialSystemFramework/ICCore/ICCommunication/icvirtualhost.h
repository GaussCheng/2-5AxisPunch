///////////////////////////////////////////////////////////
//  icvirtualhost.h
//  Implementation of the Class ICVirtualHost
//  Created on:      27-七月-2011 16:38:16
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#if !defined(EA_75BAF6EC_F913_4663_97EC_2AC09E64A638__INCLUDED_)
#define EA_75BAF6EC_F913_4663_97EC_2AC09E64A638__INCLUDED_

#include "ICCore_global.h"
#include <QObject>
#include <QMap>
#include <QSharedPointer>

#include "icoptimize.h"

#include "ictimerpool.h"
#include "iccommunictioncommandqueue.h"
#include "ictransceiverbase.h"
#include "icaddrwrapper.h"
/**
 * 手控与主机进行通讯的接口类。提供通讯的基本支持
 * @author GaussCheng
 * @version 1.0
 * @updated 02-九月-2011 14:18:30
 */

typedef QByteArray (*HostStatusFormator)(quint32 value);
class ICVirtualHost;
typedef  QSharedPointer<ICVirtualHost> ICVirtualHostPtr;

class ICCORESHARED_EXPORT ICVirtualHost : public QObject
{
    Q_OBJECT
public:
    ICVirtualHost(uint64_t hostId, QObject* parent = 0);
	virtual ~ICVirtualHost();

    bool InitConfigs(const QVector<QPair<quint32, quint32> >& configList, int startAddr = 2);
    bool ReConfigs(const QVector<QPair<quint32, quint32> >& configList, int startAddr = 2);
    uint64_t HostID() const { return hostId_;}
    int CommunicateInterval() const  { return communicateInterval_;}
    void SetCommunicateInterval(int ms);

    virtual QByteArray HostStatus(const ICAddrWrapper* addr) const = 0;
    virtual quint32 HostStatusValue(const ICAddrWrapper* addr) const = 0;
    QBitArray Alarms() const { return AlarmsImpl();}
    bool IsInputOn(int index) const { return IsInputOnImpl(index);}
    bool IsOutputOn(int index) const { return IsOutputOnImpl(index);}
    virtual bool SetAddrValue(const ICAddrWrapper* addr, int value);
    void  SetTransceiver(ICTransceiverBase* transceiver) { transceiver_ = transceiver;}
    ICTransceiverBase* Transceiver() { return transceiver_;}
    bool IsCommunicateDebug() const { return (transceiver_!= NULL) ? transceiver_->IsCommunicateDebug() : false;}
    void SetCommunicateDebug(bool debug){(transceiver_ != NULL) ? transceiver_->SetCommunicateDebug(debug) : Q_UNUSED(debug) ;}
    uint32_t CommunicateErrCount() const { return commErrCount_;}
    void IncreaseCommunicateErrCount() { ++commErrCount_;}
    void ClearCommunicateErrCount() {commErrCount_ = 0;}

    void AddCommunicationFrame(const ICTransceiverData* data) { queue_.EnQueue(data);}
    void AddFastCommunicationFrame(const ICTransceiverData* data) { queue_.InsertQueue(data);}
    int CommunicationFrameSize() const { return queue_.Size();}
    void Communicate(){CommunicateImpl();}

    virtual void StopCommunicate(){ timer_.stop();/*ICTimerPool::Instance()->Stop(timerID_, this, SLOT(RefreshStatus()));*/}
    virtual void StartCommunicate(){ SetCommunicateInterval(CommunicateInterval());}

//    virtual bool FrameNewSendPLC(int startaddr,QByteArray command) = 0;

protected:
//    void timerEvent(QTimerEvent *) {RefreshStatus();}
private slots:
    void RefreshStatus();

protected:
    ICCommunictionCommandQueue queue_;
    virtual bool InitConfigsImpl(const QVector<QPair<quint32, quint32> >& configList, int startAddr) = 0;
    virtual bool IsInputOnImpl(int index) const = 0;
    virtual bool IsOutputOnImpl(int index) const = 0;
    virtual void CommunicateImpl() = 0;
    virtual void InitStatusMap_() = 0;
    virtual void InitStatusFormatorMap_() = 0;
    virtual QBitArray AlarmsImpl() const = 0;

private:

private:
    uint64_t hostId_;
    int communicateInterval_;
    ICTransceiverBase* transceiver_;
    bool isInit_;
    mutable int currentAlarmBitIndex_;
//    int timerID_;
    QTimer timer_;
    uint32_t commErrCount_;
};

class ICCORESHARED_EXPORT ICVirtualHostManager
{
public:
    template<class T> static ICVirtualHostPtr GetVirtualHost(uint64_t hostAddr);
private:
    static QMap<uint64_t, ICVirtualHostPtr> hostAddrs_;
};

inline QByteArray GeneralStatusFormator(quint32 value)
{
    return QByteArray::number(value);
}

inline QByteArray OneDecimalStatusFormator(quint32 value)
{
    return QByteArray::number(value / (qreal)(10.0), 'f', 1);
}

inline QByteArray TwoDecimalStatusFormator(quint32 value)
{
    return QByteArray::number(value / qreal(100.0), 'f', 2);
}

inline QByteArray MinusOneDecimalStatusFormator(quint32 value)
{
    return QByteArray::number(static_cast<qint32>(value) / qreal(10.0), 'f', 1);
}

inline QByteArray MinusTwoDecimalStatusFormator(quint32 value)
{
    return QByteArray::number(static_cast<qint32>(value) / qreal(100.0), 'f', 2);
}

template<class T>
inline ICVirtualHostPtr ICVirtualHostManager::GetVirtualHost(uint64_t hostAddr)
{
    ICVirtualHostPtr ret = hostAddrs_.value(hostAddr, ICVirtualHostPtr());
    if(unlikely(ret == NULL))
    {
        ret = ICVirtualHostPtr(new T(hostAddr));
        hostAddrs_.insert(hostAddr, ret);
    }
    return ret;

}


/**
 * 设定主机地址addr的值为value
 * 
 * @param value    值
 * @param addr    地址
 */
inline bool ICVirtualHost::SetAddrValue(const ICAddrWrapper *addr, int value)
{
    Q_UNUSED(value)
    Q_UNUSED(addr)
	return false;
}

inline bool ICVirtualHost::InitConfigs(const QVector<QPair<quint32, quint32> > &configList, int startAddr)
{
    bool ret = InitConfigsImpl(configList, startAddr);
    if(ret)
    {
//        timerID_ = ICTimerPool::Instance()->Start(communicateInterval_, this, SLOT(RefreshStatus()));
//        timerID_ = startTimer(communicateInterval_);
        timer_.start(communicateInterval_);
    }
    return ret;
}

inline bool ICVirtualHost::ReConfigs(const QVector<QPair<quint32, quint32> > &configList, int startAddr)
{
    return InitConfigsImpl(configList, startAddr);
//    queue_.Clear();
}

inline void ICVirtualHost::SetCommunicateInterval(int ms)
{
    communicateInterval_ = ms;
//    if(timerID_ >= 0)
    {
//        ICTimerPool::Instance()->Stop(timerID_, this, SLOT(RefreshStatus()));
//        killTimer(timerID_);
//        timerID_ = ICTimerPool::Instance()->Start(communicateInterval_, this, SLOT(RefreshStatus()));
//        timerID_ = startTimer(communicateInterval_);
        timer_.stop();
        timer_.start(communicateInterval_);
    }
}

#endif // !defined(EA_75BAF6EC_F913_4663_97EC_2AC09E64A638__INCLUDED_)
